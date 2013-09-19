#include<stdio.h>
#include<stdlib.h>
#define NL printf("\n");

int main(){

	FILE *fp = NULL;
	int delOperation = -1;	
	NL
	printf("Initiated test files deletion");	
	NL
	delOperation = remove("test.txt");
	if(delOperation == 0){
		NL
		printf("The test files have been deleted successfully");	
		NL
	}else{
		NL
		printf("Unable to delete test files");	
		NL
	}
	return 0;
}
