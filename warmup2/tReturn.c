#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>

void *
child_thread(char *arg){
	return (void *)2;
}

int main(){

	pthread_t thread;
	int *result = (int *)malloc(sizeof(int));
	if(result == NULL){
		fprintf(stderr, "\nUnable to allocate memory");
	}	
	memset(result, 0, sizeof(char));
	int err = 1;
	if(err = pthread_create(&thread, 0, (void *)child_thread, NULL)){
		fprintf(stderr, "\nUnable to create a thread: %s", strerror(err));	
		exit(1);
	}
	pthread_join(thread, (void **) result);
	printf("\nI got the result from child thread:%d", *(int *)result);
	printf("\n");	
	return 0;
}
