#include "my402list.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define NL printf("\n");
#define NUM_MEMBERS 20 
#define KEY 11
void printList(My402List *);
int main(){
	
	int i=100;
	int j=0;
	int temp = -1;
	int *pKey = &temp;
	My402List list;
	My402ListElem *current = NULL; 
	My402List *pList = NULL;	
	pList = &list;		

	My402ListInit(pList);	
	int *a=(int *)malloc(NUM_MEMBERS*sizeof(*a));		
	if(a == NULL){
		NL	
		printf("Can not allocate memory for the data");	
		NL
	}
	My402ListInit(pList);
	for(j=NUM_MEMBERS; j>0; j--){
		
		a[NUM_MEMBERS-j] = i;	
		//printf("Iteration : %d", NUM_MEMBERS-j);
		My402ListInsertBefore(pList, (void *)(a+NUM_MEMBERS-j) , current);	
		i= i+10;
		if(j== NUM_MEMBERS){
			current = My402ListLast(pList);
		}else{
			current = My402ListPrev(pList, current);	
		}
	}
	NL
	printf("InsertBefore'ed the whole list");
	NL
	printf("The number of elements in the list = %d", pList->num_members);
	NL
	printf("------------------------------------------------");
	printList(pList);	
	printf("------------------------------------------------");
	/*	
	NL
	if(My402ListFind(pList,pKey) != NULL ){	
		printf("Found key: %d",*( (int *)My402ListFind(pList, pKey)->obj) );
	}else{
		printf("Unable not find the key");	
	}
	*/
	NL
}

void printList(My402List *pList){
	
		
	My402ListElem *current = NULL;
	NL
	for(current=My402ListFirst(pList);
		current != NULL;
		current=My402ListNext(pList,current)	
		){
		
		printf("%d -->", *((int *)current->obj));	
	}
	printf("anchor -->");
	My402ListElem *prevElem = My402ListPrev(pList,&(pList->anchor));
		printf("[repeated %d -->]...",*((int *)prevElem->obj));	
	NL	
	
}
