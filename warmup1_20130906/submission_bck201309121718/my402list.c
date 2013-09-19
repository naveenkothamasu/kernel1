
/*
 * Author:	Naveen Kumar Kothamasu (kothamas@usc.edu)
 *
*/

#include "my402list.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*
#define UNPRINT 1

#ifndef UNPRINT
#define COMMENT_OPEN  // 
#define COMMENT_CLOSE //     
#else
#define COMMENT_CLOSE */
#define COMMENT_OPEN /*
#endif
*/

int My402ListLength(My402List *pList){
	
	int length = 0;
	if( pList != NULL){
		length = pList->num_members;	
	}	
	return length;	
} 


int My402ListEmpty(My402List *pList){


	int isListEmpty = TRUE;
	if( pList != NULL && pList->num_members > 0){
		isListEmpty = FALSE;	
	}
	return isListEmpty;	
}

int My402ListAppend(My402List *pList, void *obj){
	
	int isSuccess = FALSE;	
	My402ListElem *current = NULL;
	My402ListElem *ptrToAnchor = &(pList->anchor);
	My402ListElem *pLastElem = NULL;
	current = (My402ListElem *)malloc(sizeof(My402ListElem));
	if( current == NULL){
		return isSuccess;
	}	
	isSuccess = TRUE;
	current->obj = obj;
	current->prev = NULL;
	current->next = NULL; 
	
	pLastElem = My402ListLast(pList);		
	if(pLastElem == NULL){
	
		current->next = ptrToAnchor; //1st
		ptrToAnchor->next = current; //2nd 			
		current->prev = ptrToAnchor; //4th	
		ptrToAnchor->prev = current; //3rd 			
	}else{	
		current->next = ptrToAnchor; //1st
		pLastElem->next = current;
		current->prev = pLastElem;
		ptrToAnchor->prev = current;
	}	
	pList->num_members = pList->num_members+1;
				
	return isSuccess;		
}

int My402ListPrepend(My402List *pList, void *obj){
	
	int isSuccess = FALSE;
	My402ListElem *ptrToAnchor = &(pList->anchor);
	My402ListElem *pFirstElem = NULL;	
	//Allocate the mem for node
	My402ListElem *current = NULL;
	current = (My402ListElem *)malloc(sizeof(My402ListElem));
	if(current == NULL){
		return isSuccess;	
	}
	isSuccess = TRUE;
	current->obj = obj;
	current->prev = NULL;
	current->next = NULL;		
	//printf("\n My402ListPrepend: Linking for %d\n", *((int *)current->obj));
	pFirstElem = My402ListFirst(pList);			
	if(pFirstElem == NULL ){
		//printf("\n My402ListPrepend: pFirstElem is NULL case \n");		
		current->next = ptrToAnchor;
		ptrToAnchor->next = current;
		current->prev = ptrToAnchor;
		ptrToAnchor->prev = current;	
		//printf("\n anchor prev val: %d\n", *((int *)ptrToAnchor->prev->obj));
	}else{
		//printf("\n pFirstElem: %d\n", *((int *)pFirstElem->obj));			
		current->next = pFirstElem;		
		ptrToAnchor->next = current;
		pFirstElem->prev = current;
		current->prev = ptrToAnchor;	
	}	
	pList->num_members = pList->num_members+1; 		
	return isSuccess;
}
	
My402ListElem *My402ListFirst(My402List *pList){

	My402ListElem *pFirstElem = NULL;
	if(My402ListEmpty(pList) == FALSE){  

		//printf("\n My402ListFirst: inside if condition\n");	
		pFirstElem = pList->anchor.next;	
		//printf("\n My402ListFirst: pFirstElem: %d\n", *((int *)pFirstElem->obj));
	}
	return pFirstElem;
}	
My402ListElem *My402ListNext(My402List *pList, My402ListElem *pElem){
	
	My402ListElem *pNextElem = NULL;
	if(My402ListLast(pList) != pElem){
		//printf("\n My402ListNext: inside if condition\n");	
		pNextElem = pElem->next;
	}	
	return pNextElem;
}
My402ListElem *My402ListPrev(My402List *pList, My402ListElem *pElem){
	
	My402ListElem *pPrevElem = NULL;
	if(My402ListFirst(pList) != pElem){
		//printf("\n My402ListPrev: inside First is not the passed argument\n");	
		pPrevElem = pElem->prev;	
	}	
	return pPrevElem;
}

My402ListElem *My402ListLast(My402List *pList){
	
	My402ListElem *pLastElem = NULL;
	if(My402ListEmpty(pList) == FALSE){  
		//printf("\n Inside if condition \n");
		pLastElem = pList->anchor.prev;
	}
	return pLastElem;
}

int My402ListInsertBefore(My402List *pList,void *obj, My402ListElem *pElem){
	
	//Allocate memory for the obj elem
	My402ListElem *current = NULL;
	int isSuccess = FALSE;
	current = (My402ListElem *)malloc(sizeof(My402ListElem));
	if(current == NULL){
		return FALSE;	
	}
	current->obj = obj;
	if(pElem == NULL){
		//printf("\nMy402ListInsertBefore: inside pEem is NULL \n");	
		isSuccess = My402ListPrepend(pList,obj);	
	}else{
		//FIXME: normal case is handled. Look for boundary ones
		current->next = pElem;
		pElem->prev->next = current;
		current->prev = pElem->prev; 
		pElem->prev = current;
		isSuccess = TRUE;
		pList->num_members = pList->num_members+1;
	}
	
	return isSuccess;
}

int My402ListInsertAfter(My402List *pList,void *obj, My402ListElem *pElem){
	
	//Allocate memory for the obj elem
	My402ListElem *current = NULL;
	int isSuccess = FALSE;
	current = (My402ListElem *)malloc(sizeof(My402ListElem));
	if(current == NULL){
		return FALSE;	
	}
	current->obj = obj;
	//pElem is NULL, call Append
	if(pElem == NULL){
		isSuccess = My402ListAppend(pList,obj);	
	}else{
		//FIXME: normal case is handled. Look for boundary ones
		current->next = pElem->next;
		pElem->next =current;
		current->next->prev = current;
		current->prev = pElem;
		isSuccess = TRUE;
		pList->num_members = pList->num_members+1;
	}	
	
	return isSuccess;
}

int My402ListInit(My402List *pList){
	
	int isSuccess = FALSE;
	//initialize the structure
	memset(pList,0,sizeof(*pList));	
	My402ListElem *ptrToAnchor = &(pList->anchor);	
	ptrToAnchor->obj = NULL;	
	ptrToAnchor->next = ptrToAnchor;
	ptrToAnchor->prev = ptrToAnchor;
	return isSuccess;	
}

void My402ListUnlink(My402List *pList, My402ListElem *pElem){
	
	//FIXME: pElem points to Anchor? Falls under the  list is empty case 	
	//List empty case
	if(My402ListEmpty(pList) == FALSE){
	
		//normal scenario
		My402ListElem *pred = pElem->prev;
		My402ListElem *succ = pElem->next;
	
		pred->next = succ;
		succ->prev = pred;
		pElem->prev = NULL;
		pElem->next = NULL;
	
		free(pElem);
		pList->num_members = pList->num_members-1;
	}		
}

void My402ListUnlinkAll(My402List *pList){
	
	My402ListElem *current = My402ListLast(pList);	
	My402ListElem *currentPrev = current;	
	while(currentPrev != NULL){
		
		//obtain the prev node first and then unlink	
		currentPrev = My402ListPrev(pList,current);	
		My402ListUnlink(pList, current);	
		current = currentPrev;
	}
}

My402ListElem *My402ListFind(My402List *pList, void *obj){
	
	My402ListElem *current = NULL;
	//FIXME: what if obj is NULL?
	//printf("\nMy402ListFind: obj has %d\n",*(int *)obj);	

	//COMMENT_CLOSE
	
	for(current = My402ListFirst(pList);
		current != NULL;
		current = My402ListNext(pList, current)){
		
		if(current->obj == obj ){
			break;	
		}	
	}	
	
	return current;
}
