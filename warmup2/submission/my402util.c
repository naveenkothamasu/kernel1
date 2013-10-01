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

	long long d1 = op1.tv_sec* 1000000 + op1.tv_usec;
	long long d2 = op2.tv_sec* 1000000 + op2.tv_usec;
	long long res = d1-d2;

	double micro_sec = (double)res/(double)1000;
	result->intPart = (int)micro_sec;
	result->decPart = (res-(result->intPart*1000)) % 1000;
	result->actual_num = res;
}

void  sub_timeval(struct timeval *temp, struct timeval op1, struct timeval op2){

	
	long long d1 = op1.tv_sec* 1000000 + op1.tv_usec;
	long long d2 = op2.tv_sec* 1000000 + op2.tv_usec;
	long long res = d1-d2;
	
	temp->tv_sec = 0;
	temp->tv_usec = res;
}

void add_timeval(struct timeval *temp ,struct timeval op1, struct timeval op2){
	
	long long d1 = op1.tv_sec * 1000000 + op1.tv_usec;
	long long d2 = op2.tv_sec * 1000000 + op2.tv_usec;
	long long res = d1+d2;
	temp->tv_sec = 0;
	temp->tv_usec = res;
	
}

void timeval_to_printtime(printtime *result, struct timeval t){

	double tmp = (double) t.tv_sec + ((double) t.tv_usec / (double) 1000000) ;
	
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

int isPositiveInt(char *str){

	int i=0;
	int num =0;
	int digit = 0;
	while(str[i] != '\0'){
		digit = str[i]-'0';
		if(0 <= digit && digit <= 9){
			num = num + digit;
		}else{
			return FALSE;
		}
		i++;
	}
	if(num == 0){ //This is not the actual number, but for zero check this will do.
		return FALSE;
	}
	
	return TRUE;
}
