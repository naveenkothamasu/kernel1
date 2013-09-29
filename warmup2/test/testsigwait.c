#include<stdio.h>
#include<stdlib.h>

int main(){

	sigmask_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	pthread_sigmask(SIG_BLOCK, &set, NULL);
	pthread_create(thread, NULL, (void *)child, NULL);
	pthread_sigmask(SIG_UNBLOCK, &set, NULL);
	sigwait(&set,);

}
