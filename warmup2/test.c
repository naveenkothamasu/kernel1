#include<stdio.h>
#include<stdlib.h>

int main(){

	int result =1;
	printf("\nresult has val:%d", result);
	printf("\nresult is staying at:%lu", (unsigned long)&result);
	//printf("\nthe address of result is staying at:%u", &&result); /* WRONG */	
	printf("\n");	
	return 0;
}
