#include<stdio.h>
#include<stdlib.h>
int main(){


	FILE *fp = fopen("temp.txt","r");
	if(fp == NULL){
		perror("Unable to open the give file");
		return 1;	
	}	
	char ptr[10] = {'\0'};	
	fread(ptr,1,7,fp);
	fclose(fp);
	printf("\n String:%s", ptr);
	return 0;
}
