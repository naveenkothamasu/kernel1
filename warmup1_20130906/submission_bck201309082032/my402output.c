#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#include "my402list.h"
#include "cs402.h"
#include "my402sortlist.h"

void printOutput(My402List *pList);
void formatEachField(My402SortElem *, char *, long long *);

void printOutput(My402List *pList){

	int line_number = 0;
	int i=0;
	long long *pBalance = 0;
	My402ListElem *current = NULL;
	char **output = (char **)malloc(pList->num_members*sizeof(**output));
	memset(output, '\0', pList->num_members*sizeof(**output));	
	for(; i<pList->num_members; i++){
		output[i] = (char *)malloc(80*sizeof(char));	
		memset(output[i], '\0', 80*sizeof(char));	
	}
	char *str = (char *)malloc(sizeof(80*char));
	printf("\n");	
	printf("+-----------------+--------------------------+----------------+----------------+\n");
	printf("|%7cDate%6c|%1cDescription%14c|%9cAmount%1c|%8cBalance%1c|\n",32,32,32,32,32,32,32,32);
	printf("+-----------------+--------------------------+----------------+----------------+\n");
	for(current=My402ListFirst(pList);
		current!=NULL;
		current=My402ListNext(pList, current)){
		
		printf("\nInside priting..");	
		if(output[line_number] == NULL){
			printf("\n sdsadasdadd");	
		}
		formatEachField((My402SortElem *)current->obj, str, pBalance);
		line_number++;	
		printf("|%1c15%1c|%1c24%c|%c14%1c|%c14%c|\n",32,32,32,32,32,32,32,32);	
	}	
	printf("+-----------------+--------------------------+----------------+----------------+\n");
	
}

void formatEachField(My402SortElem *currentElem, char *pCurrentLine, long long *pBalance){

	int sign = 1;
	//store the balance that is printed
	char printDesc[25] = "\0"; //+1 for terminating char
	char printAmount[15] = "\0"; //+1 for termiating char
	char printBalance[15] = "\0"; //+1 for terminating char
	//FIXME: what if sizeof(unsigned int) = 2 bytes?	
	printf("\nPrint begins...");	
	unsigned int currentAmount = currentElem->transAmount; 
	//Process Date
	printf("%s ",ctime(currentElem->transTime) );	
	//Porcess Description
	if(strlen(currentElem->transDesc) > 24){
		strncpy(printDesc,currentElem->transDesc,24);	
	}else{
	}	
	printf("%s", printDesc);
	//Porcess Amount
	if('-' == currentElem->transType){
		sign = -1;	
	}
	strncpy(printAmount,toCurrency(currentElem->transAmount),15);	
	printf("%s", printAmount);
	//Process Balance
	*pBalance = *pBalance+ sign*currentAmount;
	strncpy(printBalance,toCurrency(pBalance),15);
	printf("%s", printBalance);
}
//convert the number to currency
//place brackets if necessary
//format according to the limit > 10M
//append enough leading spaces
