#include "my402list.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h> //For memset and string funcs
#define NL printf("\n");
int main(){
	
	int i=100;
	int j=0;
	My402List list;
	memset(&list,0,sizeof(My402List));	
	My402List *pList = NULL;	
	My402ListElem *current = NULL;
	for(; j<10;j++){
		My402ListPrepend(&list,(void *)&i);	
		i= i+10;
	}
	NL
	printf("Appneded the whole list");
	NL	
	pList = &list;		
		
	for(current=My402ListLast(pList);
		current != NULL;
		current=My402ListPrev(pList,current)	
		){
		
		printf("%d -->", *((int *)current->obj));	
	}	
	NL
}
