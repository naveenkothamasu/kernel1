#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<errno.h>
#include<string.h>
#include<sys/select.h>


#include "my402threads.h"

#define handle_errors(err, exception) \
	do{errno = err; perror(exception); exit(EXIT_FAILURE); }while(0)

/**	warmup2 [-lambda lambda] [-mu mu] \
        [-r r] [-B B] [-P P] [-n num] \
        [-t tsfile]
	
  * Global variables, reside in data segment. Thus visible to every thread
**/

void parseLine(char *buf, My402Packet *);

FILE *fp;
My402FilterData *pFilterData;
pthread_t service = 0; //FIXME: what if user presses ^C before the service thread is created?

pthread_mutex_t mutex_on_filterData = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_on_stdout;
pthread_cond_t queue_not_empty = PTHREAD_COND_INITIALIZER;

sigset_t 
blockSIGINTforme()
{
	int rMask = 1;
	sigset_t set;
	sigset_t old_set;

	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	//I am not interested in SIGINT
	rMask = pthread_sigmask(SIG_BLOCK, &set, &old_set);
	if(rMask != 0){
		handle_errors(rMask, "pthread_sigmask");
	}
	
	return old_set;
}


int
main(int argc, char *argv[]){


	int isCreated = 1;//, nextToken = 0;
	pthread_t arrival, token;

	fp = fopen("tfile","r");
	if(fp == NULL){
		perror("fopen");
		exit(EXIT_FAILURE);	
	}
	//I do not need a lock here, threads are not yet created.
	/**
	 * Allocate on heap and ensure to free at the right place. Avoids scope problems if allocated on individual stacks
	**/
	pFilterData = malloc(sizeof(My402FilterData));
	memset(pFilterData, '\0', sizeof(pFilterData));
	//FIXME: move to proper init method
	pFilterData->isMorePackets = TRUE;

	pFilterData->pListQ1 = malloc(sizeof(My402List));
	My402ListInit(pFilterData->pListQ1);
	
	pFilterData->pListQ2 = malloc(sizeof(My402List));
	My402ListInit(pFilterData->pListQ2);

	sigset_t old_set = blockSIGINTforme();
	//printf("main: About to create threads...\n");
	/**
	 * TODO: Keep everything (data strctures, resouces) ready before the threads are created
	**/
	//create arrivals thread
	printf("00000000.000ms: emulation begins\n");
	isCreated = pthread_create(&arrival, NULL, (void *) arrivalManager, (void *) &old_set);
	if(isCreated != 0){
		//handle create failure
		handle_errors(isCreated, "pthread_create");
	}
	//printf("main: created arrival thread %u\n", (unsigned) arrival);
	//create tokens thread
	isCreated = pthread_create(&token, NULL, (void *) tokenManager, (void *) &old_set/*TODO*/);
	if(isCreated != 0){
		//handle create failure
		handle_errors(isCreated, "pthread_create");
	}
	//printf("main: created token thread %u\n", (unsigned) token);
	//create service thread
	isCreated = pthread_create(&service, NULL, (void *) serviceManager, (void *) &old_set);
	if(isCreated != 0){
		//handle create failure
		handle_errors(isCreated, "pthread_create");
	}

	//printf("main: created service thread %u\n", (unsigned) service);

	//printf("main: Just finished creatng threads, I am going to wait now\n");
	//wait for the other threads to terminate
	pthread_join(arrival, NULL);
	pthread_join(token, NULL);
	pthread_join(service, NULL);
	//printList(pFilterData->pListQ1);
	//printList(pFilterData->pListQ2);
	
	//printf("main:I waited for everyone to terminate and I am quitting now\n");
	
	//TODO:Move to clean up handler
	if(fp != NULL){
		fclose(fp);
	}
	return 0;
}

//TODO: make the code async-safe, if you are dealing with an already mutex'ed object
void sig_handler(int sig){
	
	int s;
	//printf("arrival: I just receoved signl: %d\n", sig);
	//FIXME: get lock here!!
	s = pthread_mutex_lock(&mutex_on_filterData);
	if(s != 0){
		handle_errors(s, "pthread_mute_lock");	
	}
		pFilterData->isStopNow = TRUE;
	s = pthread_mutex_unlock(&mutex_on_filterData);
	if(s != 0){
		handle_errors(s, "pthread_mute_unlock");	
	}
	
	//token thread can check the stopNow on its own but the server can't if it is sleeping
	pthread_cancel(service);
	//TODO: no need to clean up(Ref spec), but stop further processing. 
	//TODO: Do I need to wait for the service to get terminated
	//printf("arrival: I am terminating myself\n");
	pthread_exit(NULL);
	/*		
	if(rWait != 0){
				handle_errors(rWait, "sigwait");
			}else{
				printf("arrival: received a signal %d\n", sig);
			}		
	*/
}
void tvcpy(struct timeval dest, struct timeval src){

	dest.tv_sec = src.tv_sec;
	dest.tv_usec = src.tv_usec;
}

void *
arrivalManager(void *arg){

	int s, packet_num = 1;//, num_packets = 0;
	//printf("arrival: This is arrival thread %u\n", (unsigned) pthread_self());
	struct timeval timeStamp;
	memset(&timeStamp, '\0', sizeof(struct timeval));

	My402ListElem *pFirstListElem = NULL;	
	sigset_t *pSet = (sigset_t *)arg;
	sigset_t set;
	int rSigAction =1, isWakeService = FALSE;
	My402Packet *pCurrentPacket = NULL;	
	//keep reading while there are more packets
	char buf[1024] = {'\0'};
	char *str = malloc(1024*sizeof(char));
	memset(str, '\0', 1024);
	int isFirstLine = TRUE;	

	pthread_sigmask(SIG_SETMASK, pSet ,NULL);	
	/**
	  * if Ctrl+C received, handle it
	**/
	//TODO: examine the example in the slides
	struct sigaction act;
	memset(&act, '\0', sizeof(act));
	act.sa_handler = sig_handler;
	rSigAction = sigaction(SIGINT, &act, NULL); //NOTE:TWO ^C case: sigset adds this sig to the mask of the caller
	if(rSigAction != 0){
		handle_errors(rSigAction, "sigaction");	
	}
	//TODO: validate tfile
	while( fgets(buf, sizeof(buf), fp)  != NULL){
		//FIXME: improve the logic
		if(isFirstLine == TRUE){
			
			//num_packets = atoi(buf);
			//printf("arrival: the number of packets %d\n", num_packets);
			isFirstLine = FALSE;
			continue;
		}
		//TODO: validate the line
		pCurrentPacket = (My402Packet *) malloc(sizeof(My402Packet)); 	
		if(pCurrentPacket == NULL){
			//handle_errors(pCurrentPacket, "malloc()");//NOTE: malloc returns NULL if there is an error, so I guess errno is not set	
			fprintf(stderr, "malloc() failed, unable to allocate memory\n");
		}
		parseLine(buf, pCurrentPacket);

		
	
		//for(;;){ //TODO: enable, when deterministic model is implemented
			pCurrentPacket->packet_num = packet_num++ ;
    			//00000503.112ms: p1 arrives, needs 3 tokens, inter-arrival time = 503.112ms
			gettimeofday(&timeStamp, NULL);
			pthread_mutex_lock(&mutex_on_stdout);
				printf("%08d.%dms: p%lld arrives, needs %d tokens, inter-arrival time = %lldms\n", (int) timeStamp.tv_sec/1000,(int)timeStamp.tv_sec%1000, pCurrentPacket->packet_num, pCurrentPacket->tokens, pCurrentPacket->inter_arrival_time);
			pthread_mutex_unlock(&mutex_on_stdout);
			//===== validate the packet -- begins====
			if(pCurrentPacket->tokens > B){
		
				//drop the packet and go to the next packet
				free(pCurrentPacket);
				continue;
			}
			//===== validate the packet -- ends====
			
			//sleep for appropriate time
			timeStamp.tv_sec = (long) pCurrentPacket->inter_arrival_time / 1000;
			timeStamp.tv_usec =(long) (pCurrentPacket->inter_arrival_time % 1000)*1000;
			select(0, NULL, NULL, NULL, &timeStamp);
			//wakes up, create a packet object, lock mutex
			//enqueue the packet to Q1	
			gettimeofday(&timeStamp, NULL);
			tvcpy(pCurrentPacket->q1_begin_time,timeStamp);
			//about to get lock, so mask ^c signal
			//=======mask ^c begins======	
			sigemptyset(&set);
			sigaddset(&set, SIGINT);
			sigprocmask(SIG_BLOCK, &set, NULL);
			pthread_mutex_lock(&mutex_on_filterData); 
				My402ListAppend(pFilterData->pListQ1, pCurrentPacket);
				//00000503.376ms: p1 enters Q1
				
			pthread_mutex_lock(&mutex_on_stdout);
				printf("%08d.%dms: p%lld enters Q1\n",(int) pCurrentPacket->q1_begin_time.tv_sec/1000,(int) pCurrentPacket->q1_begin_time.tv_sec%1000, pCurrentPacket->packet_num);
			pthread_mutex_unlock(&mutex_on_stdout);
				//TODO: error checking
				pFirstListElem = My402ListFirst(pFilterData->pListQ1);
				pCurrentPacket = (My402Packet *) pFirstListElem->obj;
				//printList(pFilterData->pListQ1);
				if(pCurrentPacket->tokens <=  pFilterData->tokenCount ){
					//currentPacket is eligible for transmission
					pFilterData->tokenCount = pFilterData->tokenCount-pCurrentPacket->tokens;
					//unlink from listQ1
					My402ListUnlink(pFilterData->pListQ1, pFirstListElem);
					gettimeofday(&timeStamp, NULL);
					tvcpy(pCurrentPacket->q1_end_time,timeStamp);
    					//00000751.186ms: p1 leaves Q1, time in Q1 = 247.810ms, token bucket now has 0 token
					
					pthread_mutex_lock(&mutex_on_stdout);
						printf("%08d.%dms: p%lld leaves Q1, time in Q1 = 247.810ms, token bucket now has %d tokens\n",(int) pCurrentPacket->q1_end_time.tv_sec/1000,(int) pCurrentPacket->q1_end_time.tv_sec%1000, pCurrentPacket->packet_num, pFilterData->tokenCount );
					pthread_mutex_unlock(&mutex_on_stdout);
					if(My402ListEmpty(pFilterData->pListQ2) == TRUE){
						//need to signal service thread, but insert this and then wake him
						isWakeService = TRUE;
					}
					gettimeofday(&timeStamp, NULL);
					tvcpy(pCurrentPacket->q2_begin_time,timeStamp);
					My402ListAppend(pFilterData->pListQ2, pCurrentPacket);
					//00000752.716ms: p1 enters Q2
					
					pthread_mutex_lock(&mutex_on_stdout);
						printf("%08d.%dms: p%lld enters Q2\n", (int) pCurrentPacket->q2_begin_time.tv_sec/1000,(int) pCurrentPacket->q2_begin_time.tv_sec%1000, pCurrentPacket->packet_num);
					pthread_mutex_unlock(&mutex_on_stdout);
					//printf("arrival:starts printing..\n");
					//printList(pFilterData->pListQ2);
					//printf("arrival:ends printing..\n");
				//Now, wake him up, if he is sleeping!
			//moves the first packet from Q1 to Q2, if there are enough tokens [if token requirement is too large, drop it] 
											 //[arrival and serivce compete for Q2]
			//if Q2 was empty before, need to signal or broad cast a queue-not-empty condition [so that service thread can wake up and serve now for te packet which the arrival thread is inserting into Q2]
			//unlocks the mutex
			//goes back to sleep for the "right" amount
				}
			pthread_mutex_unlock(&mutex_on_filterData);
			sigprocmask(SIG_UNBLOCK, &set, NULL);
			//=======mask ^c ends======	
			if(isWakeService == TRUE ){
				pthread_cond_signal(&queue_not_empty);
			}	
			
			
		//} //TODO: enable, when deterministic model is implemented
	}
	
	//FIXME: do you need to return?
	//No more packets to read
	s= pthread_mutex_lock(&mutex_on_filterData);
	if(s != 0){
		handle_errors(s, "pthread_mutex_lock");	
	}
		pFilterData->isMorePackets = FALSE;
		//printf("arrival: I am done with reading, so pFlilterData->isMorePackets=%d\n", pFilterData->isMorePackets);
		if(My402ListEmpty(pFilterData->pListQ2) == TRUE){
			printf("arrival: Asking service to shutdown as there are no packets to read or process\n");
			s = pthread_cancel(service);	
			
			if(s != 0){
				handle_errors(s, "pthread_mutex_unlock");	
			}
		}
	s = pthread_mutex_unlock(&mutex_on_filterData);
	if(s != 0){
		handle_errors(s, "pthread_mutex_unlock");	
	}
	return (void *)0;
}

void parseLine(char *buf, My402Packet *pCurrentPacket){

	char *start_ptr = buf;
	char *tab_ptr = NULL;
	int tabNumber = 0;
	char **input = calloc(3, sizeof(**input));
	
	input[0] = malloc(50*sizeof(**input));
	input[1] = malloc(50*sizeof(**input));
	input[2] = malloc(50*sizeof(**input));

	//char num_tokens_required[50] = {'\0'}; 
	//char service_time[50] = {'\0'};
	for(; tabNumber<2; tabNumber++){
		tab_ptr = strchr(start_ptr,'\t');
                if(tab_ptr != NULL){
                	*tab_ptr = '\0';
                }
		//FIXME: check the length -- too big
		strncpy(input[tabNumber],start_ptr, 50 );                
		start_ptr = tab_ptr+1;
	}
	strncpy(input[tabNumber], start_ptr, 50);                
	
	//printf("arrival: inter arrival time: %s\n", input[0]);
	//printf("arrival: token required: %s\n", input[1]);
	//printf("arrival: service time: %s\n", input[2]);

	//TODO:After all the validations,
	pCurrentPacket->inter_arrival_time = atoi(input[0]);
	pCurrentPacket->tokens = atoi(input[1]);
	pCurrentPacket->service_time = atoi(input[2]);

}

void *
tokenManager(void *arg){

	//int *pNextToken = (int *)arg;
	int s, isWakeService = FALSE;
	struct timeval timeStamp;
	memset(&timeStamp, '\0', sizeof(struct timeval));
	My402ListElem *pFirstListElem = NULL;
	My402Packet *pCurrentPacket = NULL;
	//printf("This is token thread %u\n", (unsigned int) pthread_self());
	
	blockSIGINTforme();	
	for(;;){
		
		//printf("token:about to acquire the lock\n");
		s = pthread_mutex_lock(&mutex_on_filterData);
		if(s != 0){
			handle_errors(s, "pthread_mutex_lock");	
		}
		//printf("token: pFilterData->isMorePackets= %d\n", pFilterData->isMorePackets);
			if(pFilterData->isStopNow == TRUE || 
				( pFilterData->isMorePackets == FALSE 
					&& My402ListEmpty(pFilterData->pListQ1) == TRUE /* listempty check is needed, if arrival thread is too quick*/
			        )
			  ){
				
				//printf("token: I was informed to stop now\n");
				//printf("token: I got %d left in my bucket\n", pFilterData->tokenCount);
				pthread_mutex_unlock(&mutex_on_filterData); //FIXME: Does releasing lock here make sense?
				
				//printf("token: I am terminating myself\n");
				pthread_exit(NULL); //TODO: clean up?
			   }
			
			//sleep for an interval trying to match the given inter arrival time for the token	
			//wakes up, locks the mutex, try to increment tocken count [if bucket is full, drop it]
			if(isTokenBucketFull( pFilterData->tokenCount /*TODO: take B as input*/) == FALSE){
					pFilterData->tokenCount = pFilterData->tokenCount + 1;
					gettimeofday(&timeStamp, NULL);
				    	//00000251.726ms: token t1 arrives, token bucket now has 1 token
					if(pFilterData->tokenCount == 1){
						
						pthread_mutex_lock(&mutex_on_stdout);
							printf("%08d.%dms: token t%d arrives, token bucket now has %d token\n",  (int)timeStamp.tv_sec/1000,(int)timeStamp.tv_sec%1000, pFilterData->tokenCount, pFilterData->tokenCount);
						pthread_mutex_unlock(&mutex_on_stdout);
					}else{
						
						pthread_mutex_lock(&mutex_on_stdout);
							printf("%08d.%dms: token t%d arrives, token bucket now has %d token\n", (int) timeStamp.tv_sec/1000,(int) timeStamp.tv_sec%1000, pFilterData->tokenCount, pFilterData->tokenCount);
						pthread_mutex_unlock(&mutex_on_stdout);
					}
			}
			pFirstListElem = My402ListFirst(pFilterData->pListQ1);
			if(pFirstListElem != NULL){
				pCurrentPacket = (My402Packet *)pFirstListElem->obj;	
			}
			if(pCurrentPacket != NULL){
				//check if it can move first packet from Q1 to Q2 [Q1 is a shared resource]
				if(pCurrentPacket->tokens <= pFilterData->tokenCount){
					//currentPacket is eligible for transmission
					pFilterData->tokenCount = pFilterData->tokenCount-pCurrentPacket->tokens;
					//unlink from listQ1
					My402ListUnlink(pFilterData->pListQ1, pFirstListElem);	
					gettimeofday(&timeStamp, NULL);
					tvcpy(pCurrentPacket->q1_end_time,timeStamp);
					if(My402ListEmpty(pFilterData->pListQ2) == TRUE){
						//need to signal service thread, but insert this and then wake him
						isWakeService = TRUE;
					}
					gettimeofday(&timeStamp, NULL);
					tvcpy(pCurrentPacket->q2_begin_time,timeStamp);
					My402ListAppend(pFilterData->pListQ2, pCurrentPacket);
					//printf("token:starts printing..\n");
					//printList(pFilterData->pListQ2);
					//printf("token:ends printing..\n");
					
				}
			}
		//if a packet is added to Q2 and Q2 was empty before, signal or broadcast queue-not-empty condition
		s = pthread_mutex_unlock(&mutex_on_filterData);
		//unlocks the mutex
		if(s != 0){
			handle_errors(s, "pthread_mutex_unlock");	
		}
		if(isWakeService == TRUE){
			s = pthread_cond_signal(&queue_not_empty);
			if(s != 0){
				handle_errors(s, "pthread_cond_signal");	
			}
		}
		//goes back to sleep for the "right" amount
	}
	return (void *)0;
}
void clean_up(){
	printf("inside clean_up\n");
	pthread_mutex_unlock(&mutex_on_filterData);
	
}
void *
serviceManager(void *arg){
	
	//printf("This is service thread %u\n", (unsigned int) pthread_self());
	struct timeval timeStamp;
	memset(&timeStamp, '\0', sizeof(struct timeval));

	My402ListElem *pFirstElem = NULL;
	My402Packet *pCurrentPacket = NULL;
	blockSIGINTforme();	
	//lock the mutex, if Q2 is empty, wait for the queue-not-empty condition to be signaled
	//when unblocked, mutex is locked
	pthread_cleanup_push( (void *) clean_up , (void *)NULL);	
	for(;;){
		//check if I need to stop before obtainging the lock?
		//if a user presses ^C while the service thread is waiting in the mutex queue, 
		// pthread_mutex_lock is not a cancellation point, in our case - service will eventually get a lock,
		//so either it will be cancelled in conition wait or while sleep	
		pthread_mutex_lock(&mutex_on_filterData);
			//FIXME: what if service went ahead of arrival and terminates, arrival sends a cancel then?	
			if(pFilterData->isMorePackets == FALSE && My402ListEmpty(pFilterData->pListQ2) == TRUE){
				//printf("service: I was informed to stop now.\n");
				pthread_mutex_unlock(&mutex_on_filterData);	
				//printf("service: I am terminating myself\n");
				pthread_exit(NULL);
			}
			
			
			while(My402ListEmpty(pFilterData->pListQ2) == TRUE ){
				pthread_cond_wait(&queue_not_empty, &mutex_on_filterData);
			}
			//if Q2 is not empty, dequeues the packet and unlcoks the mutex
				//lock mutex, check if Q2 is empty etc
			//defer the cancel at this point
			//printf("service: I got the lock, I am processing the current packet\n");
			gettimeofday(&timeStamp, NULL);
			pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
			pFirstElem = My402ListFirst(pFilterData->pListQ2);	
			if(pFirstElem != NULL){
				pCurrentPacket = (My402Packet *)pFirstElem->obj;
			}
			if(pCurrentPacket != NULL){
				//sleeps for an interval matching the service time of the packet; afterwards eject the packet from the system
				timeStamp.tv_sec = (long) pCurrentPacket->service_time / 1000;
				timeStamp.tv_usec =(long) (pCurrentPacket->service_time % 1000)*1000;
				select(0, NULL, NULL, NULL, &timeStamp);	
		 		//00000752.932ms: p1 begin service at S, time in Q2 = 0.216ms
				
				pthread_mutex_lock(&mutex_on_stdout);
					printf("%08d.%dms: p%lld begin service at S, time in Q2 = 0.216ms\n",(int) timeStamp.tv_sec/1000,(int) timeStamp.tv_sec%1000, pCurrentPacket->packet_num);
				pthread_mutex_unlock(&mutex_on_stdout);
				gettimeofday(&timeStamp, NULL);
				tvcpy(pCurrentPacket->q2_end_time,timeStamp);
				My402ListUnlink(pFilterData->pListQ2, pFirstElem);
    				//00003612.843ms: p1 departs from S, service time = 2859.911ms, time in system = 3109.731ms
				
				pthread_mutex_lock(&mutex_on_stdout);
					printf("%08d.%dms: p%lld departs from S, service time = 2859.911ms, time in system = 3109.73ms\n", (int) pCurrentPacket->q2_end_time.tv_sec/1000,(int) pCurrentPacket->q2_end_time.tv_sec%1000,  pCurrentPacket->packet_num);
				pthread_mutex_unlock(&mutex_on_stdout);
				//printf("service: I just processed packet:%d\n", pCurrentPacket->tokens);
			}
		pthread_mutex_unlock(&mutex_on_filterData);
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		//if Q2 is empty, go wait for the queue-not-empty condition to be signaled
	}	
	pthread_cleanup_pop(1);
	//FIXME: do you need to return?
	return (void *)0;
}

