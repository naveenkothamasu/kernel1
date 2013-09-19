#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#include "my402list.h"
#include "cs402.h"
#include "my402sortlist.h"

void printOutput(My402List *pList);
void formatEachField(My402List *, char *);

void printOutput(My402List *pList){

	int line_number = 0;
	long long *pBalance = 0;
	My402ListElem *current = NULL;
	char *output[pList->num_members] = (char **) malloc(80*sizeof(char));
	memset(output, '\0', 80*sizeof(char));	
	for(){
		output[i] = (char *)malloc(80*sizeof(char));	
	}
	printf("\n");	
	printf("+-----------------+--------------------------+----------------+----------------+\n");
	printf("|%7cDate%6c|%1cDescription%14c|%9cAmount%1c|%8cBalance%1c|\n",32,32,32,32,32,32,32,32);
	printf("+-----------------+--------------------------+----------------+----------------+\n");
	for(current=My402ListFirst(pList);
		current!=NULL;
		current=My402ListNext(pList, current)){
		
		formatEachField((My402SortElem)current->obj, output[line_numer], pBalance);
		line_number++;	
		printf("|%1c15%1c|%1c24%c|%c14%1c|%c14%c|\n",32,32,32,32,32,32,32,32);	
	}	
	printf("+-----------------+--------------------------+----------------+----------------+\n");
	
}

void formatEachField(My402SortElem *currentElem, char *pCurrentLine, long long *pBalance){

	int sign = 1;
	//store the balance that is printed
	char printBalance[15] = "\0"; //+1 for terminating char
	char printDesc[25] = "\0"; //+1 for terminating char
	char printAmount[15] = "\0"; //+1 for termiating char
	char printBalance[15] = "\0"; //+1 for terminating char
	//FIXME: what if sizeof(unsigned int) = 2 bytes?	
	unsigned int currentAmount = currentElem->transAmount; 
	printf("\nPrint begins...");	
	//Process Date
	printf("%s ",ctime(currentElem->transTime));	
	//Porcess Description
	if(strlen(currentElem->transDesc) > 24){
		strncpy(printDesc,24,currentElem->transDesc);	
	}else{
	}	
	printf("%s", printDesc);
	//Porcess Amount
	if('-' == current->transType){
		sign = -1;	
	}
	strncpy(printAmount,15,toCurrency(currentElem->transAmount));	
	printf("%s", printAmount);
	//Process Balance
	*pBalance = *pBalance+ sign*currentAmount;
	strncpy(printBalance,15,toCurrency(pBalance));
	printf("%s", printBalance);
}
//convert the number to currency
//place brackets if necessary
//format according to the limit > 10M
//append enough leading spaces
