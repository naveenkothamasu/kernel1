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
		printf("|%1c%s%1c|%1c%s%c|%c%s%1c|%c%s%c|\n",32,pOutput->printDate,32,32,pOutput->printDesc,32,32,pOutput->printAmount,32,32,pOutput->printBalance,32);	
	}	
	printf("+-----------------+--------------------------+----------------+----------------+\n");
	
}

void formatEachField(My402SortElem *currentElem, My402Output *pOutput, long long *pBalance){

	//printf("\nPrint begins...");	
	int sign = 1;
	int i=0;
	int j=0;
	//store the balance that is printed
	char printDesc[25] = "\0"; //+1 for terminating char
	char printDate[16] = "\0"; //+1 for terminating char
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
	printDate[16] = '\0';
	//printf("\n%s ", printDate);
	pOutput->printDate = printDate;
	//Porcess Description
	//printf("%s ", currentElem->transDesc);
	strncpy(printDesc, currentElem->transDesc,24);	
	printDesc[25] = '\0';	
	pOutput->printDesc = printDesc;	
	//Porcess Amount
	if('-' == currentElem->transType){
		sign = -1;	
	}
	strncpy(printAmount,toCurrency((long long)currentElem->transAmount),15);	
	//printf("%s ", printAmount);
	pOutput->printAmount = printAmount;
	//Process Balance
	
	*pBalance = *pBalance+ sign*currentAmount;
	strncpy(printBalance,toCurrency(*pBalance),15);
	//printf("%s ", printBalance);
	pOutput->printBalance = printBalance;

}

//convert the number to currency
//place brackets if necessary
//format according to the limit > 10M
//append enough leading spaces
/*
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
}*/

