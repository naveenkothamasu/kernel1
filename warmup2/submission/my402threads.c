#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<errno.h>

#include "my402threads.h"

#define handle_errors(err, exception) \
	do{errno = err; perror(exception); exit(EXIT_FAILURE); }while(0)
int
main(int argc, char *argv[]){


	int isCreated = 1, nextToken = 0;
	pthread_t arrival, token, service;
	My402List listQ1, listQ2;


	My402ListInit(&listQ1);
	My402ListIni(&listQ2);

	printf("main: About to create threads...\n");
	/*
	//create arrivals thread
	isCreated = pthread_create(&arrival, NULL, (void *) arrivalManager, (void *) &listQ1);
	if(isCreated != 0){
		//handle create failure
		handle_errors(isCreated, "pthread_create");
	}
	//create tokens thread
	isCreated = pthread_create(&token, NULL, (void *) tokenManager, (void *) &nextToken);
	if(isCreated != 0){
		//handle create failure
		handle_errors(isCreated, "pthread_create");
	}
	//create service thread
	isCreated = pthread_create(&service, NULL, (void *) serviceManager, (void *) &listQ2);
	if(isCreated != 0){
		//handle create failure
		handle_errors(isCreated, "pthread_create");
	}


	printf("main: Just finished creatng threads, I am going to wait now\n");
	//wait for the other threads to terminate
	pthread_join(arrival, NULL);
	pthread_join(token, NULL);
	pthread_join(service, NULL);
	*/
	
	return 0;
}

/*

void *
arrivalManager(void *arg){

	//My402List *pListQ1 = (My402List *)arg;
	printf("This is arrival thread %u\n", (unsigned) pthread_self());
	
	//FIXME: do you need to return?
	return (void *)0;
}

void *
tokenManager(void *arg){

	//int *pNextToken = (int *)arg;
	printf("This is token thread %u\n", (unsigned int) pthread_self());
	
	//FIXME: do you need to return?
	return (void *)0;
}

void *
serviceManager(void *arg){
	
	//My402List *pListQ2 = (My402List *)arg;
	printf("This is service thread %u\n", (unsigned int) pthread_self());
	
	//FIXME: do you need to return?
	return (void *)0;
}

*/
