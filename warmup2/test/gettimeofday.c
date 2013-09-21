#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>

int 
main(){

	struct timeval *pTimeVal=malloc(sizeof(*pTimeVal));
	gettimeofday(pTimeVal, NULL);
	printf("gettimeofday()=%u\n",(unsigned) pTimeVal->tv_usec);
	return 0;
}
