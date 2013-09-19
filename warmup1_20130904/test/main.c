#include<stdio.h>
#include<stdlib.h>
int main(int argc, char *argv[]){


	FILE *fp = stdin;
	char *fileName=malloc(sizeof("test.txt"));
	//printf("\nsizeof :%d \n", sizeof("test.txt"));	
	fileName = "test.txt";	
	if(argc == 2){
		//File name is specified
		fileName = argv[1];	
	}	
	fp = fopen(fileName,"w");
	if(fp == NULL){
		printf("\n not able to open the file,test.txt to write\n");	
		
	}
	printf("\n The file, %s has been opened successfully for writing\n", fileName);
	
	fgets("",buf,fp);	






	/*	
	while(!feof(fp)){
			
	}	
	*/
	fprintf(fp, "%s","Hello file, I am your first line");	
	fclose(fp);
	printf("\n The file has been closed successfully\n");	
	/*	
	fp = fopen("test.txt","r");
	if(fp == NULL){
		printf("\n not able to open the file,%s  to read\n", fileName);	
	}
	//Read the file
		
	printf("\n The contents of the file are: \n");
	while(!feof(fp)){
		printf("%c",fputs(fp));	
	}
	fclose(fp);	
	*/
	return 0;

}
