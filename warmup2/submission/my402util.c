#include<stdio.h>
#include<stdlib.h>

#include "my402threads.h"
#include "my402util.h"

int isTokenBucketFull(int tokensAsOfNow){
	
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
