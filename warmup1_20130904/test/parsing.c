#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// max 4 characters else Error!!
int main(int argc, char *argv[]){

	char buf[7];
	FILE *fp = NULL;
	char *fileName=malloc(sizeof(50*sizeof(int)));	
	if(argc==2){
		fileName = argv[1];		
	}	
	fp = fopen(fileName, "r");
	if(fp == NULL){
		printf("\n Unble to open the file\n");	
	}
	printf("\n Successfully opened the file %s\n", fileName);
	
	while(fgets(buf ,7, fp) != NULL ){
			
		//printf("\n strlen for %s is %d\n", buf, strlen(buf));	
		if(strlen(buf)>5 && buf[4] != '\0'){
			printf("\n The input is bigger than 4 characters..\n");
			//buf[5] = '\0';	
			printf("\n String: %s\n",buf);	
			return 0;	
		}	
		printf("\n This strig is coming from the buffer: %s\n", buf);
		printf("\n its string length: %d\n", (int)strlen(buf));
	}
		//eof reached
		printf("EOF is reached\n");



	fclose(fp);		
	return 0;
}
