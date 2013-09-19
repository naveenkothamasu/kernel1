#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

void *server(char *targv);

int
main(int argc, char *argv[]){

	
	char *targv = (char *) malloc(sizeof(char));
	pthread_t thread;
	if(targv == NULL){
		fprintf(stderr, "\nunable to allocate memory");
		exit(1);
	}
	memset(targv, '\0', sizeof(char));
	int i=0;	
	int isFailure = 1;	
	for( ; i<3; i++){
		targv[0] = i;	
		isFailure = pthread_create(&thread,0,(void *)server,targv); //oops! I can't pass it?	
		if(isFailure == 0){
			printf("\nJust finished creating thread:%ld", (long int)thread);	
			pthread_join(thread ,0);
		}else{
			printf("\nI am unable to create a thread..");	
		}	
	}
	printf("\n");
	return 0;
}

void *
server(char *targv){
	printf("\nmy argument: %s", targv);
}
