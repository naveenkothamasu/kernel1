#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<errno.h>
#include<string.h>
#include<sys/select.h>


#include "my402util.h"

double lambda= 0.5 , mu= 0.35, r=1.5;
int B = 10, P = 3, n = 20;
char *t = NULL;
 
void parseLine(char *buf, My402Packet *);
int isDeterministicMode(int, char **);

FILE *fp;
My402FilterData *pFilterData;
int deterministic = TRUE;
pthread_t service = 0; //FIXME: what if user presses ^C before the service thread is created?

pthread_mutex_t mutex_on_filterData = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_on_stopNow = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_on_stdout = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_on_startTimeStamp = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_not_empty = PTHREAD_COND_INITIALIZER;

void sig_handler(int sig);
int stopNow = FALSE;

struct timeval startTimeStamp;

My402Stats *stats;
My402ArrivalStats *aStats;
My402TokenStats *tStats;
My402ServiceStats *sStats;

int
main(int argc, char *argv[]){

	sigset_t set, old_set;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);

	struct sigaction act;
	memset(&act, '\0', sizeof(struct sigaction));
	act.sa_sigaction  = (void *) sig_handler;
	
	aStats = (My402ArrivalStats *) malloc(sizeof(My402ArrivalStats));
	if(aStats == 0){
		fprintf(stderr, "malloc() failed, unable to allocate memory\n");	
		exit(EXIT_FAILURE);
	}
	memset(aStats, '\0', sizeof(My402ArrivalStats));
	tStats = (My402TokenStats *) malloc(sizeof(My402TokenStats));
	if(tStats == 0){
		fprintf(stderr, "malloc() failed, unable to allocate memory\n");	
		exit(EXIT_FAILURE);
	}
	memset(tStats, '\0', sizeof(My402TokenStats));
	sStats = (My402ServiceStats *) malloc(sizeof(My402ServiceStats));
	if(sStats == 0){
		fprintf(stderr, "malloc() failed, unable to allocate memory\n");	
		exit(EXIT_FAILURE);
	}
	memset(sStats, '\0', sizeof(My402ServiceStats));

	int isCreated = 1;
	pthread_t arrival, token;
	t = (char *)malloc(50*sizeof(char));
        if(t == NULL){
        	fprintf(stderr, "malloc() failed - unable to allocate memory\n");
        }
	memset(t, '\0', 50*sizeof(char));
	deterministic = isDeterministicMode(argc, argv);
	if(deterministic == FALSE){
		fp = fopen(t,"r");
		if(fp == NULL){
			perror("fopen");
			exit(EXIT_FAILURE);	
		}
	}
	/**
	 * Allocate on heap and ensure to free at the right place. Avoids scope problems if allocated on individual stacks
	**/
	pFilterData = malloc(sizeof(My402FilterData));
	memset(pFilterData, '\0', sizeof(pFilterData));
	//FIXME: move to proper init method
	pFilterData->isMorePackets = TRUE;
	pFilterData->tokenCount = 0;

	pFilterData->pListQ1 = malloc(sizeof(My402List));
	My402ListInit(pFilterData->pListQ1);
	
	pFilterData->pListQ2 = malloc(sizeof(My402List));
	My402ListInit(pFilterData->pListQ2);

	/**
	 * TODO: Keep everything (data strctures, resouces) ready before the threads are created
	**/
	
	//create arrivals thread
	printf("00000000.000ms: emulation begins\n");
	pthread_sigmask(SIG_BLOCK, &set, NULL); //FIXME: Is this the right place to block? ^C before threads got created?

	isCreated = pthread_create(&arrival, NULL, (void *) arrivalManager, (void *) &old_set);
	if(isCreated != 0){
		handle_errors(isCreated, "pthread_create");
	}

	//create tokens thread
	isCreated = pthread_create(&token, NULL, (void *) tokenManager, (void *) &old_set);
	if(isCreated != 0){
		handle_errors(isCreated, "pthread_create");
	}

		
	//create service thread
	isCreated = pthread_create(&service, NULL, (void *) serviceManager, (void *) &old_set);
	if(isCreated != 0){
		handle_errors(isCreated, "pthread_create");
	}
	
	pthread_sigmask(SIG_UNBLOCK, &set, NULL); //FIXME: Is this the right place to block? ^C before threads got created?
	sigaction(SIGINT, &act, NULL);
	
	pthread_join(arrival, NULL);
	pthread_join(token, NULL);
	pthread_join(service, NULL);

	runStats(aStats, tStats, sStats);	
	//TODO:Move to clean up handler
	if(fp != NULL){
		fclose(fp);
	}
	return 0;
}

//TODO: make the code async-safe, if you are dealing with an already mutex'ed object
void sig_handler(int sig){
	
	printf("received signal %d\n",sig);
	int s;
	s = pthread_mutex_lock(&mutex_on_stopNow);
	if(s != 0){
		handle_errors(s, "pthread_mute_lock");	
	}
		stopNow = TRUE; //FIXME: handle FAQ pthread_kill()
		//pthread_kill(arrival, SIGUSR1);
	s = pthread_mutex_unlock(&mutex_on_stopNow);
	if(s != 0){
		handle_errors(s, "pthread_mute_unlock");	
	}

	//token thread can check the stopNow on its own but the server can't if it is sleeping
	//pthread_cancel(service);
	//TODO: no need to clean up(Ref spec), but stop further processing. 
	//TODO: Do I need to wait for the service to get terminated
	//printf("arrival: I am terminating myself\n");
	//pthread_exit(NULL);
}

void *
arrivalManager(void *arg){

	int s, packet_num = 1, num_packets = 0;
	struct timeval timeStamp;
	memset(&timeStamp, '\0', sizeof(struct timeval));
	printtime pTimeStamp;
	memset(&pTimeStamp, '\0', sizeof(printtime));
	struct timeval sleep_time;
	memset(&sleep_time, '\0', sizeof(struct timeval));	
	struct timeval inter_arrival_time;
	memset(&inter_arrival_time, '\0', sizeof(struct timeval));	
	struct timeval current_time;
	memset(&current_time, '\0', sizeof(struct timeval));
	struct timeval prev_arrival_time;
	memset(&prev_arrival_time, '\0', sizeof(struct timeval));
	printtime pArrivalStamp;
	memset(&pArrivalStamp, '\0', sizeof(printtime));
	printtime time_in_Q1;
	memset(&time_in_Q1, '\0', sizeof(printtime));
	printtime actual_inter_arrival;
	memset(&actual_inter_arrival, '\0', sizeof(printtime));

	My402ListElem *pFirstListElem = NULL;	
	int isWakeService = FALSE;
	struct timeval tv;
	memset(&tv, '\0', sizeof(struct timeval));
	struct timeval tv_q1_begintime;
	memset(&tv_q1_begintime, '\0', sizeof(struct timeval));
	struct timeval tv_q1_endtime;
	memset(&tv_q1_endtime, '\0', sizeof(struct timeval));
	My402Packet *pCurrentPacket = NULL;	
	char buf[1024] = {'\0'};
	char *str = malloc(1024*sizeof(char));
	memset(str, '\0', 1024);
	int isFirstLine = TRUE;	
	struct timeval zero_stamp;
	memset(&zero_stamp, '\0', sizeof(struct timeval));
	double cInter_arrival_time = 0;
	int cTokens = 0;
	int line_num = 1;
	char *nlptr = NULL;
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set,SIGUSR1);
	//TODO: examine the example in the slides
	//TODO: validate tfile
	for(;;){

		
		aStats->current_packets = packet_num;
			
		
		if(deterministic == TRUE){
			if(packet_num == n+1){
				break;
			}
		}else{
			if(fgets(buf, sizeof(buf), fp)  == NULL){
				break;
			}
			nlptr  = strchr(buf,'\n');	
			if(nlptr != NULL){
				*nlptr = '\0';
			}
			//FIXME: improve the logic
			if(isFirstLine == TRUE){
				
				if(!isPositiveInt(buf)){ //TODO: check zero case, especially
					fprintf(stderr, "The number of packets should be a positive integer, but given as %s\n", buf);
					exit(EXIT_FAILURE);
				}
				num_packets = strtol(buf, NULL, 10);
				isFirstLine = FALSE;
				continue;
			}
			if(line_num > num_packets+1){
				fprintf(stderr, "The number of lines in the tsfile must be exacly n+1, but found %d\n", line_num);
				exit(EXIT_FAILURE);
			}
			line_num++;
		}
		//TODO: validate the tfile line
		pCurrentPacket = (My402Packet *) malloc(sizeof(My402Packet)); 	
		if(pCurrentPacket == NULL){
			fprintf(stderr, "malloc() failed, unable to allocate memory\n");
		}
		memset(pCurrentPacket, '\0', sizeof(My402Packet));
		if(deterministic == FALSE){
			parseLine(buf, pCurrentPacket);
		}
			inter_arrival_time.tv_sec = 0;
			if(deterministic == FALSE){
				inter_arrival_time.tv_usec = pCurrentPacket->inter_arrival_time * 1000;	
				cTokens = pCurrentPacket->tokens;
			}else{
				cInter_arrival_time = (double)1/lambda;
				inter_arrival_time.tv_usec = cInter_arrival_time * 1000000  ;	
					cTokens = P;
			}
			//===== validate the packet -- begins====
			if(cTokens > B){
		
				pthread_mutex_lock(&mutex_on_startTimeStamp);
					if(startTimeStamp.tv_sec == 0 && startTimeStamp.tv_usec == 0){
						gettimeofday(&startTimeStamp, NULL); 	
					}
				pthread_mutex_unlock(&mutex_on_startTimeStamp);
				//drop the packet and go to the next packet
				gettimeofday(&timeStamp, NULL);
				sub_printtime(&pTimeStamp, timeStamp, startTimeStamp);	
				printf("%08d.%03dms: packet p%d arrives, needs %d tokens, dropped\n",pTimeStamp.intPart, pTimeStamp.decPart, packet_num, cTokens);
				prev_arrival_time.tv_sec = 0;
				prev_arrival_time.tv_usec = pTimeStamp.actual_num;
				aStats->packets_dropped = aStats->packets_dropped + 1;
				packet_num = packet_num + 1;
				aStats->avg_inter_arrival_time = getNewAvgByNewNum(aStats->avg_inter_arrival_time, pTimeStamp.actual_num, packet_num);
				continue;
			}
			//===== validate the packet -- ends====
				
			//FIXME: sending time in microseconds
			pthread_mutex_lock(&mutex_on_startTimeStamp);
				if(startTimeStamp.tv_sec == 0 && startTimeStamp.tv_usec == 0){
					gettimeofday(&startTimeStamp, NULL); 	
				}
			pthread_mutex_unlock(&mutex_on_startTimeStamp);
			gettimeofday(&current_time, NULL);
			sub_timeval(&current_time, current_time, startTimeStamp);
			
			add_timeval(&timeStamp, prev_arrival_time, inter_arrival_time);
			sub_timeval(&sleep_time, timeStamp, current_time);
				
			pthread_sigmask(SIG_BLOCK, &set, NULL);
			s = pthread_mutex_lock(&mutex_on_stopNow);
			
			if(s != 0){
				handle_errors(s, "pthread_mutex_lock");	
			}
			
			if(stopNow == TRUE){
				printf("arrival: stopNow global var is set\n");
				s = pthread_mutex_unlock(&mutex_on_stopNow);
				if(s != 0){
					handle_errors(s, "pthread_mutex_unlock");	
				}
				//before exiting, check if the server is waiting on Q2 empty
				pthread_mutex_lock(&mutex_on_filterData);
					if(My402ListEmpty(pFilterData->pListQ2) == TRUE){
						if(service != 0){
							pthread_cancel(service);
						}
					}
				pthread_mutex_unlock(&mutex_on_filterData);
				pthread_exit(NULL);	
			} //FIXME: handle FAQ pthread_kill()
			s = pthread_mutex_unlock(&mutex_on_stopNow);
			if(s != 0){
				handle_errors(s, "pthread_mute_unlock");	
			}
			
			
			if(isPositive_timeval(sleep_time) == TRUE){
				select(0, NULL, NULL, NULL, &sleep_time);
			}
			
			
			pthread_sigmask(SIG_UNBLOCK, &set, NULL);
			s = pthread_mutex_lock(&mutex_on_stopNow);
			if(s != 0){
				handle_errors(s, "pthread_mute_lock");	
			}
			if(stopNow == TRUE){
				printf("arrival: stopNow global var is set\n");
				s = pthread_mutex_unlock(&mutex_on_stopNow);
				if(s != 0){
					handle_errors(s, "pthread_mute_unlock");	
				}
				pthread_mutex_lock(&mutex_on_filterData);
					if(My402ListEmpty(pFilterData->pListQ2) == TRUE){
						if(service != 0){
							pthread_cancel(service);
						}
					}
				pthread_mutex_unlock(&mutex_on_filterData);
				pthread_exit(NULL);	
			} //FIXME: handle FAQ pthread_kill()
			s = pthread_mutex_unlock(&mutex_on_stopNow);
			if(s != 0){
				handle_errors(s, "pthread_mute_unlock");	
			}
			
			//else get on with the business right away
			gettimeofday(&timeStamp, NULL);
			sub_printtime(&(pCurrentPacket->arrivalStamp), timeStamp, startTimeStamp);	
			pCurrentPacket->packet_num = packet_num++ ;
				timeStamp.tv_sec = 0;
				timeStamp.tv_usec = (pCurrentPacket->arrivalStamp).actual_num;
				sub_printtime(&actual_inter_arrival, timeStamp, prev_arrival_time);
				
				//FIXME: waiting for 2 mutex locks?
				pthread_mutex_lock(&mutex_on_stdout);
					printf("%08d.%03dms: p%lld arrives, needs %d tokens, inter-arrival time = %d.%03dms\n", (pCurrentPacket->arrivalStamp).intPart, (pCurrentPacket->arrivalStamp).decPart, pCurrentPacket->packet_num, cTokens, actual_inter_arrival.intPart, actual_inter_arrival.decPart);
				prev_arrival_time.tv_sec = 0;
				prev_arrival_time.tv_usec = (pCurrentPacket->arrivalStamp).actual_num;
				pthread_mutex_unlock(&mutex_on_stdout);
				aStats->avg_inter_arrival_time = getNewAvgByNewNum(aStats->avg_inter_arrival_time, actual_inter_arrival.actual_num, pCurrentPacket->packet_num);

				gettimeofday(&tv, NULL);
				sub_printtime(&(pCurrentPacket->q1_begin_time),tv, startTimeStamp);
				
				pthread_mutex_lock(&mutex_on_filterData); 
					My402ListAppend(pFilterData->pListQ1, pCurrentPacket);
					
				pthread_mutex_lock(&mutex_on_stdout);
					printf("%08d.%03dms: p%lld enters Q1\n", pCurrentPacket->q1_begin_time.intPart, pCurrentPacket->q1_begin_time.decPart, pCurrentPacket->packet_num);
				pthread_mutex_unlock(&mutex_on_stdout);
					//TODO: error checking
					pFirstListElem = My402ListFirst(pFilterData->pListQ1);
					pCurrentPacket = (My402Packet *) pFirstListElem->obj;
					if( cTokens <=  pFilterData->tokenCount ){
						//currentPacket is eligible for transmission
						pFilterData->tokenCount = pFilterData->tokenCount-cTokens;
						//unlink from listQ1
						My402ListUnlink(pFilterData->pListQ1, pFirstListElem);
						
						gettimeofday(&tv, NULL);
						sub_printtime(&(pCurrentPacket->q1_end_time),tv,startTimeStamp);
						
						tv_q1_endtime.tv_sec = 0;
						tv_q1_endtime.tv_usec= pCurrentPacket->q1_end_time.actual_num;
						tv_q1_begintime.tv_sec = 0;
						tv_q1_begintime.tv_usec= pCurrentPacket->q1_begin_time.actual_num;
						sub_printtime(&time_in_Q1, tv_q1_endtime, tv_q1_begintime);
						pthread_mutex_lock(&mutex_on_stdout);
							printf("%08d.%03dms: p%lld leaves Q1, time in Q1 = %d.%dms, token bucket now has %d tokens\n",
	pCurrentPacket->q1_end_time.intPart, pCurrentPacket->q1_end_time.decPart, pCurrentPacket->packet_num,time_in_Q1.intPart,time_in_Q1.decPart, pFilterData->tokenCount);
						pthread_mutex_unlock(&mutex_on_stdout);
						//keept it in micro-seconds for accuracy
						aStats->time_spent_q1 = aStats->time_spent_q1 + time_in_Q1.actual_num;

						if(My402ListEmpty(pFilterData->pListQ2) == TRUE){
							//need to signal service thread, but insert this and then wake him
							isWakeService = TRUE;
						}

						gettimeofday(&tv, NULL);
						sub_printtime(&(pCurrentPacket->q2_begin_time),tv, startTimeStamp);

						My402ListAppend(pFilterData->pListQ2, pCurrentPacket);
						pthread_mutex_lock(&mutex_on_stdout);
							printf("%08d.%03dms: p%lld enters Q2\n", (int) pCurrentPacket->q2_begin_time.intPart, pCurrentPacket->q2_begin_time.decPart, pCurrentPacket->packet_num);
						pthread_mutex_unlock(&mutex_on_stdout);
					}
				pthread_mutex_unlock(&mutex_on_filterData);
				if(isWakeService == TRUE ){
					pthread_cond_signal(&queue_not_empty);
				}	
		}
		
		//No more packets to read
		s= pthread_mutex_lock(&mutex_on_filterData);
		if(s != 0){
			handle_errors(s, "pthread_mutex_lock");	
		}
			pFilterData->isMorePackets = FALSE;
			if(My402ListEmpty(pFilterData->pListQ1) == TRUE 
				&& My402ListEmpty(pFilterData->pListQ2) == TRUE){
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
	//The 3 fields are separated by space or tab characters. There must be no leading or trailing space or tab characters in a line
	void parseLine(char *buf, My402Packet *pCurrentPacket){

		char *start_ptr = buf;
		char *ptr = NULL;
		char *tab_ptr = NULL;
		int tabNumber = 0;
		char **input = calloc(3, sizeof(**input));
		
		input[0] = malloc(50*sizeof(**input));
		if(input[0] == NULL){
			fprintf(stderr, "malloc() failed, unable to allocate memory\n");
		}
		memset(input[0], '\0', 50*sizeof(**input));
		input[1] = malloc(50*sizeof(**input));
		if(input[1] == NULL){
			fprintf(stderr, "malloc() failed, unable to allocate memory\n");
		}
		memset(input[1], '\0', 50*sizeof(**input));
		input[2] = malloc(50*sizeof(**input));
		if(input[2] == NULL){
			fprintf(stderr, "malloc() failed, unable to allocate memory\n");
		}
		memset(input[2], '\0', 50*sizeof(**input));

		for(; tabNumber<2; tabNumber++){
			ptr= start_ptr;
			
			tab_ptr = strchr(start_ptr,'\t');
			if(tab_ptr == NULL){
				//din't see the tab, looking for space..
				tab_ptr = strchr(start_ptr, ' ');
				if(tab_ptr == NULL){
					fprintf(stderr, "The intput file is not in the correct format\n");
					exit(EXIT_FAILURE);	
				}
			}
			
			if(tab_ptr != NULL){
				*tab_ptr = '\0';
			}else{
				fprintf(stderr, "input file is not in the correct format\n");
				exit(EXIT_FAILURE);
			}
			
			ptr = tab_ptr+1;
			while(*ptr == '\t' || *ptr == ' '){
				ptr++;
			}
			
			//FIXME: check the length -- too big
			strncpy(input[tabNumber],start_ptr, 50 );                
			start_ptr = ptr;
		}
		strncpy(input[tabNumber], start_ptr, 50);                
		
		//TODO:After all the validations,
		if(!isPositiveInt(input[0])){
			fprintf(stderr, "inter arrival time of a packet should be a positive integer, but given as %s\n", input[0]);	
			exit(EXIT_FAILURE);
		}
		if(!isPositiveInt(input[1])){
			fprintf(stderr, "inter arrival time of a packet should be a positive integer, but given as %s\n", input[0]);	
			exit(EXIT_FAILURE);
		}
		if(!isPositiveInt(input[2])){
			fprintf(stderr, "inter arrival time of a packet should be a positive integer, but given as %s\n", input[0]);	
			exit(EXIT_FAILURE);
		}
		//TODO: max_int chek pending...
		pCurrentPacket->inter_arrival_time = strtol(input[0], NULL, 10);
	pCurrentPacket->tokens = strtol(input[1], NULL, 10);
	pCurrentPacket->service_time = strtol(input[2], NULL, 10);

}
/*
void validate(char *buf, int line_num){

	
	//should have exactly n+1 lines
	//each line should be terminated with \n
	//Line 1 contains a positive integer (=n)
//other lines
	//a positive integer<>a positive integer<>a positive integer
	//(there must be no leading no traling spaces nor tabs in the lines) 

}
*/

void *
tokenManager(void *arg){

	long long current_token = 1;
	int cTokens = P;
	int s, isWakeService = FALSE;
	struct timeval timeStamp;
	memset(&timeStamp, '\0', sizeof(struct timeval));
	struct timeval tv;
	memset(&tv, '\0', sizeof(struct timeval));
	printtime pTimeStamp;
	memset(&pTimeStamp, '\0', sizeof(printtime));
	My402ListElem *pFirstListElem = NULL;
	My402Packet *pCurrentPacket = NULL;
	struct timeval prev_arrival_time;
	memset(&prev_arrival_time, '\0', sizeof(struct timeval));
	struct timeval sleep_time;
	memset(&sleep_time, '\0', sizeof(struct timeval));
	struct timeval current_time;
	memset(&current_time, '\0', sizeof(struct timeval));
	struct timeval tv_q1_endtime;
	memset(&tv_q1_endtime, '\0', sizeof(struct timeval));
	struct timeval tv_q1_begintime;
	memset(&tv_q1_begintime, '\0', sizeof(struct timeval));
	printtime time_in_Q1;
	memset(&time_in_Q1, '\0', sizeof(printtime));

	for(;;current_token++){

		tStats->current_tokens = current_token;	
			
		
		timeStamp.tv_sec = 0;
		timeStamp.tv_usec = ((double)1000000/r);

		pthread_mutex_lock(&mutex_on_startTimeStamp);
			if(startTimeStamp.tv_sec ==0 && startTimeStamp.tv_usec == 0){
				gettimeofday(&startTimeStamp, NULL);	
			}
		pthread_mutex_unlock(&mutex_on_startTimeStamp);
		gettimeofday(&current_time, NULL);
		sub_timeval(&current_time, current_time, startTimeStamp);
		add_timeval(&timeStamp, prev_arrival_time, timeStamp);
		sub_timeval(&sleep_time, timeStamp, current_time);
		
		s = pthread_mutex_lock(&mutex_on_stopNow);
		if(s != 0){
			handle_errors(s, "pthread_mute_lock");	
		}
			if(stopNow == TRUE){
				printf("token: stopNow global var is set\n");
				s = pthread_mutex_unlock(&mutex_on_stopNow);
				if(s != 0){
					handle_errors(s, "pthread_mutex_unlock");	
				}
				pthread_exit(NULL);	
			} //FIXME: handle FAQ pthread_kill()
		s = pthread_mutex_unlock(&mutex_on_stopNow);
		if(s != 0){
			handle_errors(s, "pthread_mute_unlock");	
		}
		if(isPositive_timeval(sleep_time)){
			select(0, NULL, NULL, NULL, &sleep_time);
		}
		
		
		s = pthread_mutex_lock(&mutex_on_stopNow);
		if(s != 0){
			handle_errors(s, "pthread_mute_lock");	
		}
			if(stopNow == TRUE){
				printf("token: stopNow global var is set\n");
				s = pthread_mutex_unlock(&mutex_on_stopNow);
				if(s != 0){
					handle_errors(s, "pthread_mute_unlock");	
				}
				pthread_exit(NULL);	
			} //FIXME: handle FAQ pthread_kill()
		s = pthread_mutex_unlock(&mutex_on_stopNow);
		if(s != 0){
			handle_errors(s, "pthread_mute_unlock");	
		}
		s = pthread_mutex_lock(&mutex_on_filterData);
		if(s != 0){
			handle_errors(s, "pthread_mutex_lock");	
		}
			if( pFilterData->isMorePackets == FALSE 
					&& My402ListEmpty(pFilterData->pListQ1) == TRUE /* listempty check is needed, if arrival thread is too quick*/
			        )
			  {
				
				pthread_mutex_unlock(&mutex_on_filterData); //FIXME: Does releasing lock here make sense?
				pthread_exit(NULL); //TODO: clean up?
			   }
			
			//sleep for an interval trying to match the given inter arrival time for the token	
			//wakes up, locks the mutex, try to increment tocken count [if bucket is full, drop it]
			tStats->current_tokens = current_token; 	
			gettimeofday(&tv, NULL);
			sub_printtime(&pTimeStamp, tv, startTimeStamp);
			if(isTokenBucketFull( pFilterData->tokenCount, B) == FALSE){
					pFilterData->tokenCount = pFilterData->tokenCount + 1;
					if(pFilterData->tokenCount == 1){
						pthread_mutex_lock(&mutex_on_stdout);
							printf("%08d.%03dms: token t%lld arrives, token bucket now has %d token\n",  pTimeStamp.intPart, pTimeStamp.decPart, current_token, pFilterData->tokenCount);
						pthread_mutex_unlock(&mutex_on_stdout);
					}else{
						
						pthread_mutex_lock(&mutex_on_stdout);
							printf("%08d.%03dms: token t%lld arrives, token bucket now has %d token\n", pTimeStamp.intPart, pTimeStamp.decPart, current_token, pFilterData->tokenCount);
						pthread_mutex_unlock(&mutex_on_stdout);
					}
			}else{
				pthread_mutex_lock(&mutex_on_stdout);
					printf("%08d.%03dms: token t%lld arrives, dropped\n",pTimeStamp.intPart, pTimeStamp.decPart, current_token);	
				pthread_mutex_unlock(&mutex_on_stdout);
				tStats->tokens_dropped = tStats->tokens_dropped+1; 
			}
			prev_arrival_time.tv_sec = 0;
			prev_arrival_time.tv_usec = pTimeStamp.actual_num;
			pFirstListElem = My402ListFirst(pFilterData->pListQ1);
			if(pFirstListElem != NULL){
				pCurrentPacket = (My402Packet *)pFirstListElem->obj;	
			}
			if(pCurrentPacket != NULL){
				//check if it can move first packet from Q1 to Q2 [Q1 is a shared resource]
				if(deterministic == FALSE){
					cTokens = pCurrentPacket->tokens;
				}
				if( cTokens <= pFilterData->tokenCount){
					//currentPacket is eligible for transmission
					pFilterData->tokenCount = pFilterData->tokenCount-cTokens;
					//unlink from listQ1
					My402ListUnlink(pFilterData->pListQ1, pFirstListElem);	
    
                                        gettimeofday(&tv, NULL);
                                        sub_printtime(&(pCurrentPacket->q1_end_time),tv,startTimeStamp);
    
                                        tv_q1_endtime.tv_sec = 0;
                                        tv_q1_endtime.tv_usec= pCurrentPacket->q1_end_time.actual_num;
                                        tv_q1_begintime.tv_sec = 0;
                                        tv_q1_begintime.tv_usec= pCurrentPacket->q1_begin_time.actual_num;
                                        sub_printtime(&time_in_Q1, tv_q1_endtime, tv_q1_begintime);
                                        pthread_mutex_lock(&mutex_on_stdout);
                                                printf("%08d.%03dms: p%lld leaves Q1, time in Q1 = %d.%dms, token bucket now has %d tokens\n",pCurrentPacket->q1_end_time.intPart, pCurrentPacket->q1_end_time.decPart, pCurrentPacket->packet_num, time_in_Q1.intPart,time_in_Q1.decPart , pFilterData->tokenCount );
                                        pthread_mutex_unlock(&mutex_on_stdout);
					
					tStats->time_spent_q1 = tStats->time_spent_q1 + time_in_Q1.actual_num;

					if(My402ListEmpty(pFilterData->pListQ2) == TRUE){
						isWakeService = TRUE;
					}
					gettimeofday(&tv, NULL);
					sub_printtime(&(pCurrentPacket->q2_begin_time),tv,startTimeStamp);
					
					My402ListAppend(pFilterData->pListQ2, pCurrentPacket);
 					pthread_mutex_lock(&mutex_on_stdout);
                                                printf("%08d.%03dms: p%lld enters Q2\n", (int) pCurrentPacket->q2_begin_time.intPart, pCurrentPacket->q2_begin_time.decPart, pCurrentPacket->packet_num);
                                        pthread_mutex_unlock(&mutex_on_stdout);

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

	gettimeofday(&(sStats->emulation_time), NULL);
	sub_timeval(&(sStats->emulation_time), sStats->emulation_time, startTimeStamp);

	pthread_mutex_unlock(&mutex_on_filterData);
	
}
void *
serviceManager(void *arg){
	
	struct timeval timeStamp;
	memset(&timeStamp, '\0', sizeof(struct timeval));
	struct timeval tv;
	memset(&tv, '\0', sizeof(struct timeval));
	struct timeval tv_q2_end_time;
	memset(&tv, '\0', sizeof(struct timeval));
	struct timeval tv_q2_begin_time;
	memset(&tv, '\0', sizeof(struct timeval));
	
	printtime pTimeStamp;
	memset(&pTimeStamp, '\0', sizeof(printtime));
	printtime time_in_Q2;
	memset(&time_in_Q2, '\0', sizeof(printtime));
	printtime actual_s_time;
	memset(&actual_s_time, '\0', sizeof(printtime));
	printtime system_time;
	memset(&system_time, '\0', sizeof(printtime));
	struct timeval arrivalStamp;
	memset(&arrivalStamp, '\0', sizeof(struct timeval));
	double cServiceTime;
	int localStopNow = FALSE;
	My402ListElem *pFirstElem = NULL;
	My402Packet *pCurrentPacket = NULL;
	//lock the mutex, if Q2 is empty, wait for the queue-not-empty condition to be signaled
	//when unblocked, mutex is locked
	pthread_cleanup_push( (void *) clean_up , (void *)NULL);	
	int s;
	for(;;){
	
		s = pthread_mutex_lock(&mutex_on_stopNow);
		if(s != 0){
			handle_errors(s, "pthread_mute_lock");	
		}
			if(stopNow == TRUE){
				printf("service: stopNow global var is set\n");
				s = pthread_mutex_unlock(&mutex_on_stopNow);
				if(s != 0){
					handle_errors(s, "pthread_mute_unlock");	
				}
				gettimeofday(&(sStats->emulation_time), NULL);
				sub_timeval(&(sStats->emulation_time), sStats->emulation_time, startTimeStamp);
				pthread_exit(NULL);	
			} //FIXME: handle FAQ pthread_kill()
		s = pthread_mutex_unlock(&mutex_on_stopNow);
		if(s != 0){
			handle_errors(s, "pthread_mute_unlock");	
		}	
		//check if I need to stop before obtainging the lock?
		//if a user presses ^C while the service thread is waiting in the mutex queue, 
		// pthread_mutex_lock is not a cancellation point, in our case - service will eventually get a lock,
		//so either it will be cancelled in conition wait or while sleep	
		pthread_mutex_lock(&mutex_on_filterData);
			//FIXME: what if service went ahead of arrival and terminates, arrival sends a cancel then?	
			if( My402ListEmpty(pFilterData->pListQ1) == TRUE 
					&& My402ListEmpty(pFilterData->pListQ2) == TRUE
					&& pFilterData->isMorePackets == FALSE){
				pthread_mutex_unlock(&mutex_on_filterData);	
				pthread_exit(NULL);
			}
			
			
			while(My402ListEmpty(pFilterData->pListQ2) == TRUE ){
				pthread_cond_wait(&queue_not_empty, &mutex_on_filterData);
			}
			//if Q2 is not empty, dequeues the packet and unlcoks the mutex
				//lock mutex, check if Q2 is empty etc
			//defer the cancel at this point
			pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
			pFirstElem = My402ListFirst(pFilterData->pListQ2);	
			if(pFirstElem != NULL){
				pCurrentPacket = (My402Packet *)pFirstElem->obj;
			}
			My402ListUnlink(pFilterData->pListQ2, pFirstElem);
			pthread_mutex_unlock(&mutex_on_filterData);
			if(pCurrentPacket != NULL){
				//sleeps for an interval matching the service time of the packet; afterwards eject the packet from the system
				timeStamp.tv_sec = 0;
				if(deterministic == FALSE){
					cServiceTime = pCurrentPacket->service_time;
					timeStamp.tv_usec = cServiceTime *1000;
				}else{
					cServiceTime = (double)1/mu;
					timeStamp.tv_usec = cServiceTime * 1000000;
				}
				tv_q2_begin_time.tv_sec = 0;
				tv_q2_begin_time.tv_usec = pCurrentPacket->q2_begin_time.actual_num;
				gettimeofday(&tv, NULL);
				sub_printtime(&(pCurrentPacket->q2_end_time), tv, startTimeStamp);
				tv_q2_end_time.tv_sec = 0;
				tv_q2_end_time.tv_usec = pCurrentPacket->q2_end_time.actual_num;
				sub_printtime(&time_in_Q2, tv_q2_end_time, tv_q2_begin_time);
				pthread_mutex_lock(&mutex_on_stdout);
					printf("%08d.%03dms: p%lld begin service at S, time in Q2 = %d.%03dms\n", (pCurrentPacket->q2_end_time).intPart, (pCurrentPacket->q2_end_time).decPart, pCurrentPacket->packet_num, time_in_Q2.intPart, time_in_Q2.decPart);
				pthread_mutex_unlock(&mutex_on_stdout);
				
				select(0, NULL, NULL, NULL, &timeStamp);	
				
				gettimeofday(&tv, NULL);
				sub_timeval(&tv, tv, startTimeStamp);
				timeval_to_printtime(&pTimeStamp, tv);
				sub_printtime(&actual_s_time,tv, tv_q2_end_time);
				arrivalStamp.tv_sec = 0;
				arrivalStamp.tv_usec = pCurrentPacket->arrivalStamp.actual_num;
				sub_printtime(&system_time, tv, arrivalStamp);
				pthread_mutex_lock(&mutex_on_stdout);
					printf("%08d.%03dms: p%lld departs from S, service time = %d.%03dms, time in system = %d.%03dms\n", pTimeStamp.intPart , pTimeStamp.decPart, pCurrentPacket->packet_num, actual_s_time.intPart, actual_s_time.decPart, system_time.intPart , system_time.decPart);
				pthread_mutex_unlock(&mutex_on_stdout);
				sStats->packets_served = sStats->packets_served+ 1;
				sStats->system_time = sStats->system_time + system_time.actual_num;
				getStandardDeviation(sStats, system_time.actual_num);
			}
		//TODO: should keep in micro-secs here and conver into secs at the time of printing?
		sStats->avg_service_time = getNewAvgByNewNum(sStats->avg_service_time, actual_s_time.actual_num, sStats->packets_served);
		sStats->emulation_time.tv_usec = pCurrentPacket->q2_end_time.actual_num;
		sStats->time_spent_s = sStats->time_spent_s + actual_s_time.actual_num;
		sStats->time_spent_q2 = sStats->time_spent_q2 + time_in_Q2.actual_num;
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		//if Q2 is empty, go wait for the queue-not-empty condition to be signaled
		if(localStopNow == TRUE){
			pthread_mutex_lock(&mutex_on_filterData); /* need lock here as clean up would unlock it*/
			pthread_exit(NULL);
		}
	}	
	pthread_cleanup_pop(1);
	//FIXME: do you need to return?
	return (void *)0;
}

