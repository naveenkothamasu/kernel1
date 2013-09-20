#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
#include<signal.h>

#define handle_error(er,str) \
	do{errno=er; perror(str); exit(EXIT_FAILURE); }while(0)
void *
sig_handler(void *arg){

	sigset_t *pSet = (sigset_t *)arg;
	int w, sig;
	for(;;){
		w = sigwait(pSet, &sig); // I am waiting for the signal
		if(w != 0){
			handle_error(w, "sigwait");
		}
		printf("sig_handler thread received a signal %d\n", sig);
	}
}

int
main(){

	pthread_t thread;
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGQUIT);
	sigaddset(&set, SIGUSR1);
	//TODO:sigadd(&set, SIGUSER)
	//block the signals in set for main thread
	pthread_sigmask(SIG_BLOCK/* make an union*/, &set, NULL/* not interested in old sigmask*/);
	int t = pthread_create(&thread, NULL, (void *)sig_handler, &set);
	if(t != 0){
		handle_error(t, "pthread_create");
	}
	//Just keep waiting for the other thread to terminate
	pthread_join(thread, NULL);
	return 0;
}
