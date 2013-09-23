#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<pthread.h>
#include<errno.h>

#define handle_errors(err, msg) \
	do{ errno = err; perror(msg); exit(EXIT_FAILURE); }while(0)

pthread_t thread;
pthread_mutex_t mu;
pthread_cond_t cv;

void 
sig_handler(int sig){

	int s;
	printf("main: I just received a ^c signal\n");
	s = pthread_cancel(thread);
	if(s != 0){
		handle_errors(s, "pthread_cancel");
	}
}

void *
helper(void *arg){
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	
	pthread_sigmask(SIG_BLOCK, &set, NULL);
	pthread_setcancelstate( PTHREAD_CANCEL_ENABLE , NULL);
	printf("helper: hey!!\n");	
	sleep(1000);
	printf("helper: control never reach here\n");
}
int main(){

	sigset_t set;
	void *status;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	pthread_create(&thread, NULL, (void *)helper, NULL);

	sigset(SIGINT, sig_handler);	

	pthread_join(thread, &status);
	if(status == PTHREAD_CANCELED){
		printf("main: child thread cancel state is PTHREAD_CANCELED\n");
	}else{
		printf("main: child thread is not canceled\n");
	}
	printf("main: I waited for the other thread to join, I am quitting now\n");
	return 0;
}
