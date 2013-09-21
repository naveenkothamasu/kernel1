#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<errno.h>
#include<string.h>


#include "my402threads.h"

#define handle_errors(err, exception) \
	do{errno = err; perror(exception); exit(EXIT_FAILURE); }while(0)

/**	warmup2 [-lambda lambda] [-mu mu] \
        [-r r] [-B B] [-P P] [-n num] \
        [-t tsfile]
	
  * Global variables, reside in data segment. Thus visible to every thread
**/

void parseLine(char *buf);

My402List listQ1, listQ2;
FILE *fp;


sigset_t 
blockSIGINTforme()
{
	int rMask = 1;
	sig_set_t set;

	sigemptyset(&set);
	sigaddset(SIGINT, &set);
	//I am not interested in SIGINT
	rMask = pthread_sigmask(SIG_BLOCK, set, NULL /*not interested in old set*/);
	if(rMask != 0){
		handle_errors(rMast, "pthread_sigmask");
	}
	
	return set;
}
int
main(int argc, char *argv[]){


	int isCreated = 1, nextToken = 0;
	pthread_t arrival, token, service;

	fp = fopen("tfile","r");
	if(fp == NULL){
		perror("fopen");
		exit(EXIT_FAILURE);	
	}
	//I do not need a lock here, threads are not yet created.
	My402FilterData filterData;
	My402FilterData *pFilterData = &filterData;
	memset(pFilterData, '\0', sizeof(filterData));
	My402List listQ1;
	pFilterData->pListQ1 = &listQ1;
	My402ListInit(pFilterData->pListQ1);
	My402List listQ2;
	pFilterData->pListQ2 = &listQ2;
	My402ListInit(pFilterData->pListQ2);

	sigset_t set = blockSIGINTforme();	
	printf("main: About to create threads...\n");
	/**
	 * TODO: Keep everything (data strctures, resouces) ready before the threads are created
	**/
	//create arrivals thread
	isCreated = pthread_create(&arrival, NULL, (void *) arrivalManager, (void *) &set);
	if(isCreated != 0){
		//handle create failure
		handle_errors(isCreated, "pthread_create");
	}
	printf("main: created arrival thread %u\n", (unsigned) arrival);
	//create tokens thread
	isCreated = pthread_create(&token, NULL, (void *) tokenManager, (void *) &set);
	if(isCreated != 0){
		//handle create failure
		handle_errors(isCreated, "pthread_create");
	}
	printf("main: created token thread %u\n", (unsigned) token);
	//create service thread
	isCreated = pthread_create(&service, NULL, (void *) serviceManager, (void *) &set);
	if(isCreated != 0){
		//handle create failure
		handle_errors(isCreated, "pthread_create");
	}

	printf("main: created service thread %u\n", (unsigned) service);

	printf("main: Just finished creatng threads, I am going to wait now\n");
	//wait for the other threads to terminate
	pthread_join(arrival, NULL);
	pthread_join(token, NULL);
	pthread_join(service, NULL);
	
	printf("main:I waited for everyone to terminate and I am quitting now\n");
	
	//TODO:Move to clean up handler
	if(fp != NULL){
		fclose(fp);
	}
	return 0;
}


void *
arrivalManager(void *arg){

	printf("arrival: This is arrival thread %u\n", (unsigned) pthread_self());
	sigset_t pSet = (sigset_t *)arg;
	int sig,rWait =1;
	//keep reading while there are more packets
	char buf[1024] = {'\0'};
	char *str = malloc(1024*sizeof(char));
	memset(str, '\0', 1024);
	int isFirstLine = TRUE, num_packets = 0;	
	//TODO: validate tfile
	while( ( str=fgets(buf, sizeof(buf), fp) ) != NULL){
		//FIXME: improve the logic
		if(isFirstLine == TRUE){
			
			num_packets = atoi(buf);
			isFirstLine = FALSE;
			continue;
		}	
		parseLine(str);
	}	
	printf("arrival: the number of packets %d\n", num_packets);
	
	for(;;){

		/**
		  * if Ctrl+C received, handle it
		**/
		rWait = sigwait(pSet, sig);
		if(rWait != 0){
			handle_errors(rWait, "sigwait");
		}
		printf("arrival: received a signal %d\n", sig);		
		//sleep for appropriate time
		//wakes up, create a packet object, lock mutex
		//enqueue the packet to Q1
		//moves the first packet from Q1 to Q2, if there are enough tokens [if token requirement is too large, drop it] 
										 //[arrival and serivce compete for Q2]
		//if Q2 was empty before, need to signal or broad cast a queue-not-empty condition [so that service thread can wake up and serve now for te packet which the arrival thread is inserting into Q2]
		//unlocks the mutex
		//goes back to sleep for the "right" amount
			
	}
	//FIXME: do you need to return?
	return (void *)0;
}

void parseLine(char *buf){

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
	
	printf("arrival: inter arrival time: %s\n", input[0]);
	printf("arrival: token required: %s\n", input[1]);
	printf("arrival: service time: %s\n", input[2]);
}

void *
tokenManager(void *arg){

	//int *pNextToken = (int *)arg;
	printf("This is token thread %u\n", (unsigned int) pthread_self());
	
	blockSIGINTforme();	
	
	for(;;){
		
		//sleep for an interval trying to match the given inter arrival time for the token	
		//wakes up, locks the mutex, try to increment tocken count [if bucket is full, drop it]
		//check if it can move first packet from Q1 to Q2 [Q1 is a shared resource]
		//if a packet is added to Q2 and Q2 was empty before, signal or broadcast queue-not-empty condition
		//unlocks the mutex
		//goes back to sleep for the "right" amount
	}
	return (void *)0;
}

void *
serviceManager(void *arg){
	
	printf("This is service thread %u\n", (unsigned int) pthread_self());
	
	blockSIGINTforme();	
	//lock the mutex, if Q2 is empty, wait for the queue-not-empty condition to be signaled
	//when unblocked, mutex is locked
	//if Q2 is not empty, dequeues the packet and unlcoks the mutex
		//sleeps for an interval matching the service time of the packet; afterwards eject the packet from the system
		//lock mutex, check if Q2 is empty etc
	//if Q2 is empty, go wait for the queue-not-empty condition to be signaled

	//FIXME: do you need to return?
	return (void *)0;
}

