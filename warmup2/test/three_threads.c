#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<string.h>

#define TRUE 1
#define FALSE 0
void *method_thread_1(void *);
void *method_thread_2(void *);
void *method_thread_3(void *);
pthread_t thread_3;
void sig_handler(int sig);
int isStopNow = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv;
pthread_t thread_1, thread_2;
int
main(){

	sigset_t set, old_set;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);	
	pthread_sigmask(SIG_BLOCK, &set, NULL);
	printf("main: going to create threads\n");
	pthread_create(&thread_1, NULL, (void *)method_thread_1, &old_set);
	pthread_create(&thread_2, NULL, (void *)method_thread_2, &old_set);
	pthread_create(&thread_3, NULL, (void *)method_thread_3, &old_set);
	pthread_sigmask(SIG_UNBLOCK, &set, NULL);
	struct sigaction act;
	memset(&act, '\0', sizeof(act));
	act.sa_sigaction = (void *)sig_handler;
	sigaction(SIGINT,&act, NULL);
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	pthread_join(thread_3, NULL);
	return 0;
}
void sig_handler(int sig){

	printf("I just received the signal\n");
	pthread_mutex_lock(&m);
		//printf("thread1 has the lock\n");
		isStopNow = 1;
	pthread_mutex_unlock(&m);
}
void *method_thread_1(void *arg){
	/*
	sigset_t *pSet = (sigset_t *)arg;	
	pthread_sigmask(SIG_SETMASK, pSet, NULL);
	struct sigaction act;
	act.sa_handler = sig_handler;
	sigaction(SIGINT, &act, NULL);
	*/
	for(;;){
		pthread_mutex_lock(&m);
			if(isStopNow == 1){
				printf("thread1: I am terminating myself..\n");
				//some condition to see if the server thread is waiting
				pthread_mutex_unlock(&m);
				pthread_cancel(thread_3);
				printf("thread1: unlocked the mutex, sent a cancellation request to thread_3\n");
				pthread_exit(NULL);
			}

		pthread_mutex_unlock(&m);
		//sleep(10);
		printf("thrad1: I just woke up\n");	
		pthread_mutex_lock(&m);
			if(isStopNow == 1){
				printf("thread1: I am terminating myself..\n");
				//some condition to see if the server thread is waiting
				pthread_mutex_lock(&m);
				pthread_cancel(thread_3);
				printf("thread1: unlocked the mutex, sent a cancellation request to thread_3\n");
				pthread_exit(NULL);
			}
		pthread_mutex_unlock(&m);
	}

}
void *method_thread_2(void *arg){
	for(;;){
		pthread_mutex_lock(&m);	
		if(isStopNow == 1){
			printf("thread2:I am going to be terminated now\n");
			pthread_mutex_unlock(&m);	
			printf("thread2:unlocked the mutex..about to die\n");
			pthread_exit(NULL);	
		}		
		pthread_mutex_unlock(&m);	
		//sleep(10);
		printf("thrad2: I jst work up\n");
		pthread_mutex_lock(&m);	
		if(isStopNow == 1){
			printf("thread2:I am going to be terminated now\n");
			pthread_mutex_unlock(&m);	
			printf("thread2:unlocked the mutex..about to die\n");
			pthread_exit(NULL);	
		}		
		pthread_mutex_unlock(&m);	

	}
}
void *clean(int *i){

	printf("clean: i=%d\n",*i);
	pthread_mutex_unlock(&m);
}
void *method_thread_3(void *arg){
	int i=1;
	int localStop = FALSE;
	for(;;){
		//pthread_testcancel();	
		pthread_mutex_lock(&m);
		//printf("thread3 has the lock\n");
		pthread_cleanup_push((void *)clean,(void *)&i);
		while(1){
			pthread_cond_wait(&cv, &m);
		}
		pthread_cleanup_pop(1);
		pthread_mutex_lock(&m);	
		//printf("thread2 has the lock\n");
		if(isStopNow == 1){
			localStop = TRUE;
		}		
		pthread_mutex_unlock(&m);	
		//sleep(10);
		printf("thrad3: I jst work up\n");
		//we don't want to quit here, the current packet should be processed.
		//after the processing is done
		if(localStop == TRUE){
			pthread_mutex_unlock(&m);
			printf("thread3:I am going to be terminated now\n");
			pthread_exit(NULL);
		}
	}
}
