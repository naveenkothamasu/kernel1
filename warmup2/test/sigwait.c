#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<signal.h>
#include<pthread.h>
#include<errno.h>

#define handle_errors(err,msg) \
	do{errno = err; perror(msg); exit(EXIT_FAILURE); }while(0)
void *helper(void *);
void sig_handler(int);

int main(){

	//1. By default Ctrl+c works

	sigset_t set;
	sigset_t old_set;
	int s;	
	pthread_t thread;
	int return_child;

	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	s= pthread_sigmask(SIG_BLOCK, &set, &old_set);
	if(s !=0){
		handle_errors(s,"pthread_create");	
	}
	pthread_create(&thread, NULL, (void *)helper, (void *)&old_set);
	/*for(;;){
		printf("main: I will just go on printing this until you tell me ^C\n");
	}*/
	pthread_join(thread, (void *)&return_child);
	printf("main: My child terminated and its return val: %d\n", return_child);
	return 0;
}

void 
sig_handler(int sig){

	printf("helper: I just received the signal %d, I will kill my self :D\n", sig);
	pthread_exit(NULL);
}

void *
helper(void *arg){

	sigset_t *pSet = (sigset_t *)arg;
	printf("helper:pSet is null? %d\n", pSet == NULL?1:0);
	pthread_sigmask(SIG_SETMASK, pSet, NULL);
	int i=0,s;
	sigset(SIGINT, sig_handler);
	for(;;){
		/*	
		if(s !=0){
			handle_errors(s,"sigset");
		}
		*/
		printf("helper:I hold the value i as %d now\n", i++);
	}
}
