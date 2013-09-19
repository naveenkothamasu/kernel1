#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define FALSE 0
#define TRUE 1
int myDigitCheck(char a[]);
int main(){

	char a[10]="12321";
	int isSuccess = FALSE;
	isSuccess = myDigitCheck(a);
	//printf("\n The ASCII value of 0 is %d\n", '0');	
	//printf("\n The character with ASCII value 49 is %c\n", '30');	
	if(isSuccess == TRUE){
		printf("\nThe string has only the digit characters\n");
	}	
	return 0;
}

int myDigitCheck(char a[]){

	int i=0;
	for(i=0; i<strlen(a); i++){
		if(! (0+'0'<=a[i] && a[i]<=9+'0')){
			printf("\n Found a non-digit char:%c\n",a[i]);	
			return 0;	
		}	
	}
	return 1;
}
