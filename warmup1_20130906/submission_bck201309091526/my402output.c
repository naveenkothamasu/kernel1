#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#include "my402list.h"
#include "cs402.h"
#include "my402sortlist.h"

typedef struct{
	char *printDate;
	char *printDesc;
	char *printAmount;
	char *printBalance;
}My402Output;

void printOutput(My402List *pList);
void formatEachField(My402SortElem *currentElem, My402Output *, long long*);
extern char *toCurrency(long long);

void printOutput(My402List *pList){

	int line_number = 0;
	int i=0;
	My402Output output;
	memset(&output, '\0', sizeof(My402Output));
	My402Output *pOutput = &output; 
	
	long long *pBalance = (long long *)malloc(sizeof(long long));
	memset(pBalance, 0, sizeof(long long)) ;
	if(pBalance == NULL){
		fprintf(stderr, "\nUnable to allocate memory");	
	}

	My402ListElem *current = NULL;
	/*for(; i<pList->num_members; i++){
		output[i] = (char *)malloc(80*sizeof(char));	
		memset(output[i], '\0', 80*sizeof(char));	
	}*/
	printf("\n");	
	printf("+-----------------+--------------------------+----------------+----------------+\n");
	printf("|%7cDate%6c|%1cDescription%14c|%9cAmount%1c|%8cBalance%1c|\n",32,32,32,32,32,32,32,32);
	printf("+-----------------+--------------------------+----------------+----------------+\n");
	for(current=My402ListFirst(pList);
		current!=NULL;
		current=My402ListNext(pList, current)){
		
		formatEachField((My402SortElem *)current->obj, pOutput, pBalance);
		line_number++;	
		printf("|%1c%s%1c|%2c%s|%2c%s|%2c%s|\n",32,pOutput->printDate,32,32,pOutput->printDesc,32,pOutput->printAmount,32,pOutput->printBalance);	      
		//printf("01234567890123456789012345678901234567890123456789012345678901234567890123456789\n");
		//printf("0         10        20        30        40        50        60        70       79");	
		//printf("\n");
	}	
	printf("+-----------------+--------------------------+----------------+----------------+\n");
	
}

void formatEachField(My402SortElem *currentElem, My402Output *pOutput, long long *pBalance){

	//printf("\nPrint begins...");	
	int sign = 1;
	int i=0;
	int j=0;
	int count = 0;
	//store the balance that is printed
	char printDesc[25] = "\0"; //+1 for terminating char
	char printDate[16] = "\0"; //+1 for terminating char
	char temp[15] = "\0";
	char printAmount[15] = "\0"; //+1 for termiating char
	char printBalance[15] = "\0"; //+1 for terminating char	
	char *cTime = (char *)malloc(26*sizeof(char));
	memset(cTime, '\0', 26*sizeof(char));
	//FIXME: what if sizeof(unsigned int) = 2 bytes?	
	unsigned int currentAmount = currentElem->transAmount; 
	//Process Date
	cTime = ctime(&currentElem->transTime);
	while(cTime[i] != '\n'){
		
		if(! (i >= 10 && i<=18) ){
			printDate[j] = cTime[i];
			j++;
		}
		i++;
	}	
	printDate[15] = '\0';
	//printf("\n%s ", printDate);
	pOutput->printDate = printDate;
	//Porcess Description
	//printf("%s ", currentElem->transDesc);
	strncpy(printDesc, currentElem->transDesc,24);	
	//printf("\nstrlen-1 = %d",i);
	i=24;	
	while(printDesc[i] == '\0' && i>0){ //although not expecting i=0
		printDesc[i] = ' ';
		i--;	
	}	
	printDesc[24] = '\0';
	//printf("\nDesc%s.....",printDesc);	
	pOutput->printDesc = printDesc;	
	//Porcess Amount
	if('-' == currentElem->transType){
		sign = -1;	
	}
	strncpy(printAmount,toCurrency((long long)currentElem->transAmount),14);
	//FIXME: add leading spaces	
	i=0;
	count = 14-strlen(printAmount);
	while(i<count){ //although not expecting i=0
		//printf("\ninside amoun condition\n");	
		temp[i] = ' ';
		i++;	
	}
	j=0;	
	while(printAmount[j] != '\0'){
		temp[i++] = printAmount[j++];
	}
	temp[14] = '\0';	
	pOutput->printAmount = temp;
	//Process Balance
	
	*pBalance = *pBalance+ sign*currentAmount;
	strncpy(printBalance,toCurrency(*pBalance),14);
	//FIXME: add leading spaces	
	i=0;
	count = 14-strlen(printBalance);
	while(i<count){ //although not expecting i=0
		//printf("\ninside amoun condition\n");	
		temp[i] = ' ';
		i++;	
	}
	j=0;
	while(printBalance[j] != '\0'){
		temp[i++] = printBalance[j++];
	}
	temp[14] = '\0';	
	//printf("%s ", printBalance);
	pOutput->printBalance = temp;

}

//convert the number to currency
//place brackets if necessary
//format according to the limit > 10M
//append enough leading spaces
