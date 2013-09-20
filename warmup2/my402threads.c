#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>

#include "my402threads.h"
#include "my402list.h"
#include "cs402.h"

#define handle_errors(err, exception) /
	do{ errno = err; perror(err); exit(EXIT_FAILURE); }while(0)
int
main(int argc, char *argv[]){


	int isCreated = 1;
	pthread_t arrival, token, service;

	//create arrivals thread
	isCreated = pthread_create(&arrival, NULL, (void *) arrivalManager, listQ1);
	if(isCreated != 0){
		//handle create failure
		handle_errors(isCreated, "pthread_create");
	}
	//create tokens thread
	isCreated = pthread_create(&token, NULL, (void *) tokenManager, nextToken);
	if(isCreated != 0){
		//handle create failure
		handle_errors(isCreated, "pthread_create");
	}
	//create service thread
	isCreated = pthread_create(&service, NULL, (void *) serviceManager, listQ2);
	if(isCreated != 0){
		//handle create failure
		handle_errors(isCreated, "pthread_create");
	}



	//wait for the other threads to terminate
	pthread_join(arrival, NULL);
	pthread_join(token, NULL);
	pthread_join(service, NULL);


	return 0;
}

void *arrivalManager(void *arg){

	My402List *listQ1 = (My402List *)arg;
}

void *tokenManager(void *arg){

	int *pNextToken = (int *)arg;
}

void *serviceManager(void *arg){
	
	My402List *listQ2 = (My402List *)arg;
}


