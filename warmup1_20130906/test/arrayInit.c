#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){

	char a[3] = {'\0'};
	//memset(a,'\0', 3);
	int i=0;	
	for(;i<=3 && a[i] == '\0';){
		printf("\ncount me!\n");	
		i++;
	}
	return 0;
}
