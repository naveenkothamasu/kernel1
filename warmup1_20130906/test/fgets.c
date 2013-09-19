#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(int argc, char *argv[]){

	FILE *fp = NULL;
	char buf[6];	
	int i=0;
	fp = fopen(argv[1], "r");	
	if(fp == NULL){
		printf("\nFile, %s can not be opened!! \n", argv[1]);	
		return 0;
	}
	printf("\n Succefully opened the file.");	
	char *str =fgets(buf, sizeof(buf), fp);
	printf("\n strlen(buf) = %d", (int)strlen(buf));
	for(i=0; buf[i]!='\0'; i++){
		printf("\n buf[%d] = %c",i,buf[i]);	
	}
	printf("\n");
	char *newLineAt = NULL;
	if((newLineAt = strchr(buf,'\n')) != NULL ){
		printf("\n buf[%d] has new line\n",newLineAt-buf);	
	}	
	fclose(fp);	
	return 0;
}
