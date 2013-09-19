#include<stdio.h>
#include<stdlib.h>
#include "cs402.h"
#include<string.h>
#define CHAR_LIMIT 16 

char *substring(char *,char *,char *);
//FIXME: re-check the sizes 
typedef struct{
	char date[15];
	char description[24];
	char amount[14];
	char balance[14];
		
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
	

	//FIXME: user should be ketp asking or program should exit?
	//FIXME: modify according to the makefile	
	/*	
	if(!(argc == 2 || argc == 1)){
		printf("\nError::Too many or too few arguments.");
		printf("\nPlease enter the command in either of the following formats:\n");
		printf("\n\tmake sort tfile");
		printf("\n\tor");
		printf("\n\tmake sort\n\n");	
		return 0;
	}*/
	//FIXME: when working with make file	
	//if(argc == 3){
		fileName = argv[1];		
		fp = fopen(fileName,"r");
	//}
	if(fp == NULL){
		//FIXME: Is there a case for unable to open stdin? 
		printf("\nUnable to open the file: %s. Please check the given file name and its path and try again!\n", argv[2]);					return 0;	
	}
	
	while(fgets(buf, sizeof(buf), fp) != NULL){
		//<Date>\t<Description>\t<Amount>\t<Balance>\n
			
		if(strlen(buf) > CHAR_LIMIT+1 && buf[strlen(buf)-1] != '\0'){
			printf("\n The line has more than %d characters, so the program aborts now! \n", CHAR_LIMIT);	
		}else{
			printf("\nStruct starts\n");	
			start_ptr = buf;
			for(tabNumber=0; tabNumber<3; tabNumber++){
				tab_ptr = strchr(start_ptr,'\t');
				if(tab_ptr != NULL){
					*tab_ptr = '\0';	
				}		
				printf("%s\n",substring(buf,start_ptr,tab_ptr));
				start_ptr = tab_ptr+1;	
			}	
				printf("%s\n",substring(buf,start_ptr,start_ptr+strlen(buf)));
			printf("Strcut ends\n");
		}	
	}	
	fclose(fp);	
	return 0;
}

char *substring(char *buf,char *start_ptr, char *tab_ptr){

	int i=0;
	char *substring = malloc(tab_ptr-start_ptr);
	while(start_ptr != tab_ptr){
		substring[i] = *start_ptr++;
		i++;	
	}	
	return substring;
}
