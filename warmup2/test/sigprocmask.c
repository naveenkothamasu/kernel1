#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<pthread.h>
#include<string.h>

void sig_handler(int sig);
void *
helper(void *arg){

	struct sigaction act;
	memset(&act, '\0', sizeof(struct sigaction));		
	
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigprocmask(SIG_UNBLOCK, &set, NULL);
	
	act.sa_handler = sig_handler;
	sigaction(SIGINT, &act, NULL);
	printf("handler: 1 - I can be printed\n");	
	//sigprocmask(SIG_BLOCK, &set, NULL);
		printf("handler: I can NOT be printed\n");	
	//sigprocmask(SIG_UNBLOCK, &set, NULL);
	printf("handler: 2 - I can be printed\n");	
	//printf("sigint is not blocked here..\n");
	while(1){
	 ;	
	}
	
}

void sig_handler(int sig){

	printf("helper: I just received the signal = %d\n", sig);
	pthread_exit(NULL);
}

int 
main(){

	pthread_t thread;
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	pthread_sigmask(SIG_BLOCK, &set, NULL);
	pthread_create(&thread, NULL, (void *)helper, &set);

	printf("main: I am just waiting for the child to terminate\n");
	pthread_join(thread, NULL);
	return 0;
}

