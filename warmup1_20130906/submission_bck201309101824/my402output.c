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
extern char *representNegatives(char *);

void printOutput(My402List *pList){

	int line_number = 0;
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
	printf("+-----------------+--------------------------+----------------+----------------+\n");
	printf("|%7cDate%6c|%1cDescription%14c|%9cAmount%1c|%8cBalance%1c|\n",32,32,32,32,32,32,32,32);
	printf("+-----------------+--------------------------+----------------+----------------+\n");
	for(current=My402ListFirst(pList);
		current!=NULL;
		current=My402ListNext(pList, current)){
		
		formatEachField((My402SortElem *)current->obj, pOutput, pBalance);
		line_number++;	
		printf("|%1c%s%1c|%1c%s%1c|%2c%s|%2c%s|\n",32,pOutput->printDate,32,32,pOutput->printDesc,32,32,pOutput->printAmount,32,pOutput->printBalance);	      
		//printf("01234567890123456789012345678901234567890123456789012345678901234567890123456789\n");
		//printf("0         10        20        30        40        50        60        70       79\n");	
		//printf("\n");
		
		//free the current node of type My402ListElem now
	}	
	printf("+-----------------+--------------------------+----------------+----------------+\n");
	
}

void formatEachField(My402SortElem *currentElem, My402Output *pOutput, long long *pBalance){

	//printf("\nPrint begins...");	
	int sign = 1; //Tracking the amount sign
	int i=0;
	int j=0;
	int whiteSpaces = 0;
	int count = 0;
	int balanceSign = 1;
	//store the balance that is printed
	char printDesc[25] = "\0"; //+1 for terminating char
	char printDate[16] = "\0"; //+1 for terminating char
	char *desc = malloc(sizeof(25));
	if(desc == NULL){
		perror("\nUnable to allocate memory");	
	}	
	memset(desc,'\0',25);
	char *amt = malloc(sizeof(15));
	if(amt == NULL){
		perror("\nUnable to allocate memory");	
	}	
	memset(amt,'\0',15);
	char *bal = malloc(sizeof(15));
	if(bal == NULL){
		perror("\nUnable to allocate memory");	
	}	
	memset(bal,'\0',15);
	char printAmount[15] = "\0"; //+1 for termiating char
	char printBalance[15] = "\0"; //+1 for terminating char
	char *cTime = (char *)malloc(26*sizeof(char));
	memset(cTime, '\0', 26*sizeof(char));
	//FIXME: what if sizeof(unsigned int) = 2 bytes?	
	long long currentAmount = currentElem->transAmount; 
	//Porcess Amount
	if('-' == currentElem->transType){
		sign = -1;	
	}
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
	pOutput->printDate = printDate;
	strncpy(printDesc, currentElem->transDesc,24);	
	i=24;	
	while(printDesc[i] == '\0' && i>0){ //although not expecting i=0
		printDesc[i] = ' ';
		i--;	
	}	
	printDesc[24] = '\0';
	//counting the leading white spaces 
	whiteSpaces = 0;
	i=0;	
	while(printDesc[i] == ' '&& printDesc[i]!= '\0'){
		whiteSpaces++;	
		i++;
	}
	strncpy(desc, printDesc+whiteSpaces, strlen(printDesc+whiteSpaces));
	//aligning the desc
	i=strlen(printDesc)-whiteSpaces;
	//printf("\ni=%d whitespaces=%d",i,whiteSpaces);
	while(i<=23){
		desc[i++] = ' ';
	}	
	//printf("\ndesc:%s..",desc);
	strncpy(printDesc, desc, 24);
	printDesc[24] = '\0';
	pOutput->printDesc = printDesc;	
	strncpy(printAmount,toCurrency(currentElem->transAmount),14);
	i=0;
	count = 14-strlen(printAmount);
	while(i<count){ //although not expecting i=0
		amt[i] = ' ';
		i++;	
	}
	for(j=0;printAmount[j] != '\0'; j++,i++){
		//printf("\ninside amoun condition\n");	
		amt[i] = printAmount[j];
	}
	amt[14] = '\0';
	strncpy(printAmount,amt,14);
	printAmount[14] = '\0';
	if(sign == -1){
		strncpy(printAmount, representNegatives(printAmount),14);
	}
	printAmount[14] = '\0';
	pOutput->printAmount = printAmount;
	
	//Process Balance
	*pBalance = (long long)sign*(long long)currentAmount + *pBalance;
	//printf("\nBal:%lld",*pBalance);
	if(*pBalance < 0){
		balanceSign = -1;	
	}
	strncpy(printBalance,toCurrency((*pBalance)*balanceSign),14);
	
        //right alignment starts
	i=0;
	j=0;
	count = 14-strlen(printBalance);
	while(i<count){ //although not expecting i=0
		bal[i] = ' ';
		i++;	
	}
	j=0;
	while(printBalance[j] != '\0'){
		bal[i++] = printBalance[j++];
	}
	bal[14] = '\0';	
	//printf("%s ", printBalance);
	strncpy(printBalance, bal, 14);
	printBalance[14] = '\0';	
	if(*pBalance < 0){
		strncpy(printBalance, representNegatives(printBalance),14);
	}
	printBalance[14] = '\0';	
	pOutput->printBalance = printBalance;

}
