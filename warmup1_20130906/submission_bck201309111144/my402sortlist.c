#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>
#include<sys/time.h>

#include "my402list.h"
#include "cs402.h"
#include "my402sortlist.h"
#define CHAR_LIMIT 1024 

extern int printOutput(My402List *pList);
extern int myDigitCheck(char *);

void printList(My402List *pList){

	My402ListElem *current = NULL;
	for(current=My402ListFirst(pList); current != NULL; current = My402ListNext(pList, current)){
	
		printObj((My402SortElem *)current->obj);
	}
}

void printObj(My402SortElem *obj){
	printf("\n%c\t",obj->transType);
	printf("%llu\t",(unsigned long long)obj->transTime);
	printf("%lld\t",obj->transAmount);
	printf("%s\t",obj->transDesc);
}
int main(int argc, char *argv[]){

	int isInsert = FALSE;
	My402SortElem *currentSortElem = NULL;	
	My402List list;
	My402ListInit(&list);
	My402List *pList = &list;	
	FILE *fp = stdin;
	char *fileName = {"\0"};
	char *pSortElem[4];
	pSortElem[0] = (char *)malloc(2*sizeof(char)); // 1 for +/- and the other for '\0'	
	pSortElem[1] = (char *)malloc(11*sizeof(char));//MAX 10, +1 for terminating char
	pSortElem[2] = (char *)malloc(sizeof(int));
	pSortElem[3] = (char *)malloc(sizeof(1024));
	
	//TODO: output to stdout and error to stderr
		
	//FIXME: re-check the initialization part	
	
	char buf[CHAR_LIMIT+26] = {'\0'}; 	
	char *start_ptr = buf;
	char *tab_ptr = NULL;
	int tabNumber = 0;
	char *str = (char *)malloc(sizeof(1050));
	memset(str,sizeof(str),'\0');
	char *transDesc = (char *)malloc(sizeof(1024));	
	memset(transDesc, sizeof(transDesc), '\0');

	//FIXME: user should be ketp asking or program should exit?
	//FIXME: modify according to the makefile	
	
	if(!(argc == 2 || argc == 3) 
		||(argc>1 && strcmp("sort",argv[1]) != 0)
		||(argc==3 && 
			!( isalpha(argv[2][0]) != 0 || argv[2][0] == '_'|| argv[2][0] == '/')
		  )
	
	){
		fprintf(stderr,"\nmalformed command");
		fprintf(stderr, "\nPlease enter the command in either of the following formats:");
		fprintf(stderr, "\n\twarmpup1 sort tfile");
		fprintf(stderr, "\n\tor");
		fprintf(stderr, "\n\twarmup1 sort\n\n");	
		return 0;
	}
	
	//FIXME: when working with make file	
	if(argc == 3){
		fileName = argv[2];		
		fp = fopen(fileName,"r");
	}
	if(fp == NULL){
		//FIXME: Is there a case for unable to open stdin?
		//FIXME: input file is not in the correct format 
		fprintf(stderr, "\nUnable to open the file: %s. Please check the given file name and its path and try again!\n", argv[2]);
		perror(NULL);	
		return 0;	
	}

	while((str = fgets(buf, sizeof(buf), fp)) != NULL){
			//printf("\n complete string length:%d\n", strlen(str));	
			if(strlen(buf)==0 || buf[0] == '\n'){
				fprintf(stderr,"\nThere is an empty line present in the file");
				fprintf(stderr,"\nSo the program aborts now!\n");	
				return 0;
			}
			if(isTabsOK(buf) == FALSE){
				fprintf(stderr,"\nThere is a line present in the file with more than 3 TABS.\nEach line in the file must exactly have three TABS");
				fprintf(stderr,"\nSo the program aborts now!\n");	
				return 0;
			}	
			if(strlen(str)>CHAR_LIMIT+1){ //+1 for new line char
				fprintf(stderr, "\nThe line has more than %d characters, so the program aborts now! \n", CHAR_LIMIT);	
				return 0;
			}else{
			//Parse and see if it is malformed	
				//printf("\nStruct starts\n");	
				
				start_ptr = str;	
				for(tabNumber=0; tabNumber<3; tabNumber++){
					tab_ptr = strchr(start_ptr,'\t');
					if(tab_ptr == NULL){
						fprintf(stderr,"\nEach line in the file must exactly have three TABs");	
						fprintf(stderr, "\nThe string is malformed, so the program aborts now! \n");	
						return 0;
					}
					if(tab_ptr != NULL){
						*tab_ptr = '\0';	
					}		
					//FIXME: substrings must not be NULL
					//printf("%s ",substring(buf,start_ptr));
					pSortElem[tabNumber] = substring(buf, start_ptr);
					start_ptr = tab_ptr+1;	
				}
				transDesc = substring(buf, start_ptr);
				if(strlen(transDesc) == 0){
				
					fprintf(stderr,"\nTransaction Description can not be empty.");
					fprintf(stderr, "\nThe string is malformed, so the program aborts now! \n");	
					return 0;
				}	
				//printf("%s", substring(buf,start_ptr));
				pSortElem[3] = substring(buf, start_ptr);	
				//FIXME: verify each field has the right data
				if(verifySortElem(pSortElem) == FALSE){
					fprintf(stderr, "\nThe string is malformed, so the program aborts now! \n");	
					return 0;
				}	
				currentSortElem = createSortElem(pSortElem);	
				isInsert = insertionSort(pList, currentSortElem);
				if(isInsert == FALSE){
					fprintf(stderr, "\nso the program aborts now! \n");	
					return 0;	
				}
				/*
				if(strchr(str,'\n') == NULL){
					break; //EOF reached	
				}*/
			}	
		
		//}else{
			//printf("\nThe last character: %c", str[strlen(str)-1]=='\n'?'t':'f');	
			//printf("\n strlen(buf)=%d", (int)strlen(buf));	
			//printf("\nThe line has more than %d characters, so the program aborts now! \n", CHAR_LIMIT);	
			//return 0;
		//}
	}
	//printf("\n hit the EOF..came out\n");	
	fclose(fp);	
	//printList(pList);
	//printf("\n The output starts here..");
	if(printOutput(pList) == FALSE){
		return FALSE;	
	}
	unlinkSortElements(pList);
	//printf("\n The output ends here..");
	
	return 0;
}

void unlinkSortElements(My402List *pList){

        My402ListElem *current = My402ListLast(pList);
        My402ListElem *currentPrev = current;
        while(currentPrev != NULL){

                //obtain the prev node first and then unlink    
                currentPrev = My402ListPrev(pList,current);
		free(current->obj);
                My402ListUnlink(pList, current);
                current = currentPrev;
        }
}

int verifySortElem(char **pSortElem){

	char *dotPos = 0;
	char *startPos = NULL;
	char *lastPos = NULL;
	int i=0;
	int len= 0;	
	//transType field validations
	if (! (*pSortElem[0] == '+' || *pSortElem[0] == '-')){
		fprintf(stderr,"\nTransaction type is given incorrectly as %c.Transaction type must be + or -.",*pSortElem[0]);	
		return FALSE;	
	}
	//Timestamp field validations
	//printf("\ntime%s",pSortElem[1]);
	if(myDigitCheck(pSortElem[1]) == FALSE){
		fprintf(stderr,"\nTransaction time is given incorrectly as %s.Transaction time must contain only numerics.",pSortElem[1]);
		return FALSE;
	}
	//At this point every char is a digit, now check if it is too big
	//0 <= timeStamp <= current time
	//printf("\ntime: %lld",atoll(pSortElem[1]));
	//printf("\ncurr: %lld",(long long)time(NULL));
	if(
		!(0<= atoll(pSortElem[1])
 		&& (long long)atoll(pSortElem[1])< (long long) time(NULL)/* current time*/
	  	 )
	  ) {
		
		fprintf(stderr,"\nTransaction time is given incorrectly as %s.\nTransaction time must be a positive number less than the current time.",pSortElem[1]);	
		return FALSE;	
	}
	//Amount field validations: <7 digits>.<2 digits>
	startPos = &pSortElem[2][0];
	dotPos = strchr(pSortElem[2], '.');
	lastPos = startPos+strlen(pSortElem[2])-1;
	if(pSortElem[2][0] == '-'){
		fprintf(stderr, "\nTransaction Amount is given incorrectly as %s.\nTransaction amount must be a positive numeral.", pSortElem[2]);
		return FALSE;	
		
	}	
	if(dotPos == NULL){
		//printf("\n processing %s", pSortElem[2]);	
		fprintf(stderr, "\nTransaction Amount is given incorrectly as %s.\nTransaction amount must be a decimal number with two decimals.", pSortElem[2]);	
		return FALSE;	
	}
	if(!( dotPos-startPos <=7 &&  lastPos-dotPos == 2) ){
		fprintf(stderr, "\nTransaction Amount is given incorrectly as %s.\nTransaction amount must be a decimal number with two decimals and at most 7 digits before the decimal.", pSortElem[2]);	
	
		return FALSE;
	}
	len = strlen(pSortElem[2]);
	for(i=0; i<strlen(pSortElem[2]); i++){
		if(i == len-3){
			continue; //dot check is passed by this point	
		}	
		if(!isdigit(pSortElem[2][i])){
			fprintf(stderr,"\nTransaction Amount is give incorrectly as %s. Transaction amount must not contain any non-numerals except the single decimal point.", pSortElem[2]);
			return FALSE;
		}
	}
		
	if(atoll(pSortElem[2]) == 0){
		fprintf(stderr,"\nTransaction Amount is give incorrectly as %s. Transaction amount must be a positive number.", pSortElem[2]);
		return FALSE;	
	}
	//Description field validations
	if(strlen(pSortElem[3]) == 0){
		fprintf(stderr,"\nTransaction Description can not be empty.");
		return FALSE;	
	}
	if(strchr(pSortElem[3],'\t') != NULL ){
		fprintf(stderr,"\nTransaction Description contains a tab.");
		return FALSE;	
	}	
	return TRUE;	
}
My402SortElem *createSortElem(char **pSortElem){

	My402SortElem *pLocalSortElem = (My402SortElem *)malloc(sizeof(My402SortElem));
	memset(pLocalSortElem, '\0', sizeof(My402SortElem));
	if(pLocalSortElem == NULL){
		perror("\nUnable to allocate memory");
		return 0;	
	}	
	pLocalSortElem->transType = *pSortElem[0];
	pLocalSortElem->transTime = (unsigned int)atoi(pSortElem[1]);
	//FIXME: use the correct data type below
	//FIXME: can strtod handle values from 0 to 2^31 -1...?
	pLocalSortElem->transAmount = (long long)(strtod(pSortElem[2],NULL)*(double)100); //Keep it in cents
	pLocalSortElem->transDesc = trim(pSortElem[3]);
	return pLocalSortElem;	
}

char *trim(char *input){
	
	if(input == NULL){
		return NULL; //Although NULL is never passed to this function	
	}
	int descLen = (int)strlen(input);
	//printf("\nString length:%d\n", descLen);	
	char *returnVal = (char *)malloc((descLen+1)*sizeof(char));
	memset(returnVal, '\0', (descLen+1)*sizeof(char));	
	int i = strlen(input)-1;	
	int j=0;
	
	if(input == NULL){
		return NULL;	
	}
	
	if(input[strlen(input)-1] == '\n'){
		input[strlen(input)-1] = '\0';	
	}
	if(input[0] != ' '){
		return input;	
	}else{

		i=0;
		while(input[i] == ' ' && i < strlen(input)){
			i++;
		}
		i=0;	
		while(input[i] != '\0'){
			returnVal[j] = input[i];
			j++;
			i++;	
		}
		
		return returnVal;
	}
}

char *substring(char *buf,char *start_ptr){
	
	//FIXME: any way to reduce the below size?
	char *substring = malloc(1024*(int)sizeof(int));
	memset(substring, '\0', 1024*(int)sizeof(int));	
	int i=0;
	while(*start_ptr != '\0'){
		substring[i++]= *start_ptr++;
	}
	//Terminating char is present alredy through memset
	return substring;
}

int insertionSort(My402List *pList, My402SortElem *pKey){

	My402ListElem *current = My402ListLast(pList);
	My402ListElem *prevCurrent = current;
	while(current != NULL &&
		current->obj != NULL && 
		 ((My402SortElem *)current->obj)->transTime > pKey->transTime){
	
		prevCurrent = current;
		current = My402ListPrev(pList, current);	
	}
	if(current != NULL && current->obj != NULL){
		if(((My402SortElem *)current->obj)->transTime ==  pKey->transTime){
			fprintf(stderr,"\nTime stamp %lld is repeated for some entries",(long long)pKey->transTime);	
			//clean up an return
			unlinkSortElements(pList);	
			return FALSE;
		
		}
	}
	if(current == NULL){
		My402ListInsertBefore(pList, pKey, prevCurrent);
	}else{
		My402ListInsertAfter(pList, pKey, current);
	}
	return TRUE;
}

int isTabsOK(char *str){

	int i=0;
	int tabCount = 0;
	int isSuccess = TRUE;
	while(str[i] != '\0'){
		if(str[i] == '\t'){
			tabCount++;	
		}
		i++;	
	}
	
	if(tabCount !=3){
		isSuccess = FALSE;	
	}
	
	return isSuccess;
}


