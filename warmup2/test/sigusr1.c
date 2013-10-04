#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<string.h>

void *thread1();
void clean_up(){
	printf("inside clean up\n");
	pthread_exit(NULL);
}
int
main(){
	pthread_t thread;
	pthread_create(&thread, NULL, (void *)thread1, NULL);
	pthread_kill(thread, SIGUSR1);
	pthread_join(thread, NULL);
	
}

void *thread1(){
	/*
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	*/
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	struct sigaction act;
	memset(&act, '\0', sizeof(struct sigaction));
	act.sa_sigaction = clean_up;
	sigaction(SIGUSR1, &act, NULL);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	/*
	pthread_cleanup_push((void *) clean_up, NULL);
	printf("thread1: I will be sleeping for 1000ms now...\n");
	sleep(1000);
	printf("thread1:this will never be printed..\n");
	pthread_cleanup_pop(1);
	*/

}
