#include<stdio.h>
#include<stdlib.h>

#include "my402threads.h"
#include "my402util.h"

int isTokenBucketFull(int tokensAsOfNow, int B){
	
	return B==tokensAsOfNow ? TRUE : FALSE;
}

void printList(My402List *pList){

	My402ListElem *current = NULL;
	printf("\n====================\n");
	for(current = My402ListFirst(pList);
		current != NULL;
		current = My402ListNext(pList, current)
	   ){
		
		printf("%lld-->", ((My402Packet *)current->obj)->inter_arrival_time);	
	}
	
	printf("\n====================\n");

}
struct printtime 
sub_printtime(struct timeval op1, struct timeval op2){

	struct printtime temp;
	memset(&temp, '\0', sizeof(struct printtime));
	double d1 = op1.tv_sec + op1.tv_usec/1000000;
	double d2 = op2.tv_sec + op2.tv_usec/1000000;
	double res = d1-d2;
	
	int a = (int) (res*1000000);
	int b = (int) (res* (double)1000)*1000;
	temp.intPart = res*1000;
	temp.decPart = a-b;
	return temp;
}

struct timeval sub_timeval(struct timeval op1, struct timeval op2){

	struct timeval temp;
	temp.tv_sec = op1.tv_sec - op2.tv_sec;
	temp.tv_usec = op1.tv_usec - op2.tv_usec;
	
	return temp;	
}

struct timeval add_timeval(struct timeval op1, struct timeval op2){
	
	struct timeval temp;
	memset(&temp, '\0', sizeof(struct timeval));
	temp.tv_sec = op1.tv_sec + op2.tv_sec;
	temp.tv_usec = op1.tv_usec + op2.tv_usec;
	
	return temp;
}

