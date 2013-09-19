#include "my402list.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define NL printf("\n");
#define NUM_MEMBERS 5 
#define KEY 11
void printList(My402List *);
void insertionSort(My402List *pList, char *pKey){
 	printf("\n processing %c", *pKey); 
      My402ListElem *current = My402ListLast(pList);
        My402ListElem *prevCurrent = current;
        while(current != NULL &&
                current->obj != NULL &&  
                (*(char *)current->obj) > *pKey){
                prevCurrent = current;
                current = My402ListPrev(pList, current);    
        }   
        printf("\ninsertionSort:....");
        if(current == NULL){
                My402ListInsertBefore(pList, pKey, prevCurrent);
        }else{
                My402ListInsertAfter(pList, pKey, current);
        }   
}
int main(){
	
	char i='A';
	int j=0;
	int temp = -1;
	int *pKey = &temp;
	My402List list;
	My402ListElem *current = NULL; 
	My402List *pList = NULL;	
	pList = &list;		

	My402ListInit(pList);	
	/*char *a=(char *)malloc(NUM_MEMBERS*sizeof(*a));		
	if(a == NULL){
		NL	
		printf("Can not allocate memory for the data");	
		NL
	}*/
	My402ListInit(pList);
	char a[5]={'Q','W','E','R','T'};
	for(j=NUM_MEMBERS; j>0; j--){
		
		//a[NUM_MEMBERS-j] = i;	
		//printf("Iteration : %d", NUM_MEMBERS-j);
		/*	
		My402ListInsertBefore(pList, (void *)(a+NUM_MEMBERS-j) , current);	
		i= i-1;
		if(j== NUM_MEMBERS){
			current = My402ListLast(pList);
		}else{
			current = My402ListPrev(pList, current);	
		}*/
		insertionSort(pList,a+NUM_MEMBERS-j);
		i++;
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
	//Unlink start here
	My402ListElem *pLastElem = My402ListLast(pList);
	//My402ListUnlink(pList ,pLastElem);
	My402ListUnlinkAll(pList);			
	NL
	printf("Last element is unlinked and now printing....");	
	*/
	NL
	printf("The number of elements in the list = %d", pList->num_members);
	NL
	printf("------------------------------------------------");
	printList(pList);	
	
}

void printList(My402List *pList){
	
		
	My402ListElem *current = NULL;
	NL
	for(current=My402ListFirst(pList);
		current != NULL;
		current=My402ListNext(pList,current)	
		){
		printf("%c -->", *((char *)current->obj));	
	}
	printf("anchor -->");
	//My402ListElem *prevElem = My402ListPrev(pList,&(pList->anchor));
	//printf("[repeated %d -->]...",*((int *)prevElem->obj));	
	NL	
	
}
