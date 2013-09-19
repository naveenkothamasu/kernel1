#include "my402list.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h> //For memset and string funcs
#define NL printf("\n");
int main(){
	
	int i=100;
	int j=0;
	My402List list;
	//========== initialization part starts==========	
	memset(&list,0,sizeof(My402List));	
	My402List *pList = NULL;	
	pList = &list;		
	My402ListElem *ptrToAnchor = &(pList->anchor);
	ptrToAnchor->next = ptrToAnchor;
	ptrToAnchor->prev = ptrToAnchor;
	//=========== initialization part ends============	
	My402ListElem *current = ptrToAnchor;
	for(; j<10;j++){
		My402ListInsertBefore(&list,(void *)&i,current);	
		i= i+10;
		current = current->prev;
	}
	NL
	printf("Appneded the whole list");
	NL	
		
	for(current=My402ListLast(pList);
		current != NULL;
		current=My402ListPrev(pList,current)	
		){
		
		printf("%d -->", *((int *)current->obj));	
	}	
	NL
}
