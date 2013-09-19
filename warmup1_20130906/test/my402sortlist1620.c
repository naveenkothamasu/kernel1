#include<stdio.h>
#include<stdlib.h>
#include "cs402.h"
#include<string.h>
#define CHAR_LIMIT 15 

char *substring(char *,char *);
//FIXME: re-check the sizes 
typedef struct{
	char transType[1];
	char transTime[4];
	char trasAmount[10];
	char transDesc[1024];
		
}My402SortElem; 
int main(int argc, char *argv[]){

	FILE *fp = stdin;
	char *fileName = NULL;
	//TODO: output to stdout and error to stderr
		
	//FIXME: re-check the initialization part	
	
	char buf[CHAR_LIMIT+3] = {'\0'}; 	
	char *start_ptr = buf;
	char *tab_ptr = NULL;
	int isMalFormed = FALSE;
	int tabNumber = 0;
	char *str = (char *)malloc(sizeof(1050));
	

	//FIXME: user should be ketp asking or program should exit?
	//FIXME: modify according to the makefile	
		
	if(!(argc == 2 || argc == 1)){
		printf("\nError::Too many or too few arguments.");
		printf("\nPlease enter the command in either of the following formats:\n");
		printf("\n\tmake sort tfile");
		printf("\n\tor");
		printf("\n\tmake sort\n\n");	
		return 0;
	}
	//FIXME: when working with make file	
	if(argc == 2){
		fileName = argv[1];		
		fp = fopen(fileName,"r");
	}
	if(fp == NULL){
		//FIXME: Is there a case for unable to open stdin? 
		printf("\nUnable to open the file: %s. Please check the given file name and its path and try again!\n", argv[2]);					return 0;	
	}

	while((str = fgets(buf, sizeof(buf), fp)) != NULL){
			if(strlen(str)>CHAR_LIMIT+1){
				printf("\nThe line has more than %d characters, so the program aborts now! \n", CHAR_LIMIT);	
				return 0;
			}else{
			//Parse and see if it is malformed	
				printf("\nStruct starts\n");	
				start_ptr = str;	
				for(tabNumber=0; tabNumber<3; tabNumber++){
					tab_ptr = strchr(start_ptr,'\t');
					if(tab_ptr == NULL){
						printf("\nThe string is malformed, so the program aborts now! \n");	
						return 0;
					}
					if(tab_ptr != NULL){
						*tab_ptr = '\0';	
					}		
					//FIXME: substrings must not be NULL
					printf("%s ",substring(buf,start_ptr));
					start_ptr = tab_ptr+1;	
				}	
				printf("%s", substring(buf,start_ptr));
				printf("Strcut ends\n");
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
	printf("\n hit the EOF..came out\n");	
	fclose(fp);	
	return 0;
}

char *substring(char *buf,char *start_ptr){

	char *substring = malloc(80*(int)sizeof(int));
	int i=0;
	while(*start_ptr != '\0'){
		substring[i++]= *start_ptr++;
	}	
	return substring;
}

void insertionSort(My402List pList, char *pKey){

	My402ListElem *current = My402ListLast(pList);
	My402ListElem *prevCurrent = current;
	while(current != NULL &&
		current->obj != NULL && 
		atoi(current->obj) > atoi(pKey)){
	
		prevCurrent = current;
		current = My402ListPrev(pList, current);	
	}
	if(current == NULL){
		insertBefore(pList, pKeyElem, prevCurrent);
	}else{
		insertAfter(pList, pKeyElem, current);
	}
	
}

