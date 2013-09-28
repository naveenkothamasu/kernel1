#include<stdio.h>
#include<stdlib.h>

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
void 
sub_printtime(printtime *result, struct timeval op1, struct timeval op2){

	memset(result, '\0', sizeof(printtime));
	double d1 = (double)op1.tv_sec* 1000000 + (double)op1.tv_usec;
	double d2 = (double)op2.tv_sec* 1000000 + (double)op2.tv_usec;
	long res = d1-d2;
	
	int a = (int) (res*1000000);
	int b = (int) (res* (double)1000)*1000;
	result->intPart = res/1000;
	result->decPart = res%1000;
	result->actual_num = res/1000000;
}

void  sub_timeval(struct timeval *temp, struct timeval op1, struct timeval op2){

	double d1 = (double)op1.tv_sec + (double)op1.tv_usec/(double)1000000;
	double d2 = (double)op2.tv_sec + (double)op2.tv_usec/(double)1000000;
	double res = d1-d2;
	
	temp->tv_sec = 0;
	temp->tv_usec = res*1000000;
}

void add_timeval(struct timeval *temp ,struct timeval op1, struct timeval op2){
	
	temp->tv_sec = op1.tv_sec + op2.tv_sec;
	temp->tv_usec = op1.tv_usec + op2.tv_usec;
	
}

void timeval_to_printtime(printtime *result, struct timeval t){

	double tmp = t.tv_sec + ( t.tv_usec / 1000000) ;
	
	int a = (int) (tmp*1000000);
	int b = (int) (tmp* (double)1000)*1000;
	result->intPart = tmp*1000;
	result->decPart = a-b;
	result->actual_num = tmp;
}

int isPositive_timeval(struct timeval t){

	double tmp = t.tv_sec + ( t.tv_usec / 1000000) ;
	int isPositive = TRUE;

	if( tmp < 0){
		isPositive = FALSE;	
	}
	return isPositive;
}

//FIXME: assumtion tmp is sec
void double_to_timeval(struct timeval *t, double tmp){

	t->tv_sec = 0;
	t->tv_usec = tmp*1000;
}

void getcurrenttime(printtime *p, struct timeval start){
	
	struct timeval temp;
	memset(&temp, '\0', sizeof(struct timeval));
	gettimeofday(&temp, NULL);
	sub_printtime(p, temp, start);
}

