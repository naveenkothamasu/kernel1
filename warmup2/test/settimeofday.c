#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>

int 
main(){

	struct timeval *pTimeVal=malloc(sizeof(*pTimeVal));
	pTimeVal->tv_sec =0 ;
	pTimeVal->tv_usec = 0;
	settimeofday(pTimeVal, NULL);
	gettimeofday(pTimeVal, NULL);
	printf("gettimeofday()=%u\n",(unsigned) pTimeVal->tv_usec);
	pTimeVal->tv_sec = 0.001;
	pTimeVal->tv_usec = 0;
	select(0, NULL, NULL, NULL, pTimeVal);
	gettimeofday(pTimeVal, NULL);
	printf("gettimeofday()=%u\n",(unsigned) pTimeVal->tv_usec);
	return 0;
}
