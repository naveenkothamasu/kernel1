#include "my402list.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define NL printf("\n");
#define NUM_MEMBERS 50 
void printList(My402List *);
int main(){
	
	int i=100;
	int j=0;
	int num_members = NUM_MEMBERS; 
	My402List list;
	My402List *pList = NULL;	
	My402ListElem *current = NULL;
	pList = &list;		

	int *a=(int *)malloc(num_members*sizeof(*a));		
	if(a == NULL){
		NL	
		printf("Can not allocate memory for the data");	
		NL
	}
	My402ListInit(pList);
	for(j=num_members; j>0; j--){
		My402ListAppend(&list,(void *)(a+num_members-j));
		//My402ListAppend(&list,(void *)(&j));
		a[num_members-j] = i;	
		i= i+10;
	}
	NL
	printf("Appneded the whole list");
	NL
	printf("The number of elements in the list = %d", pList->num_members);
	NL	
	printf("------------------------------------------------");
	printList(pList);	
	printf("------------------------------------------------");
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
	printf("anchor-->");
	My402ListElem *nextElem = My402ListNext(pList,&(pList->anchor));
	if(My402ListPrev(pList,&(pList->anchor))){
		printf("[repeated %d -->]...",*((int *)nextElem->obj));	
	}		
	NL	
	
}
