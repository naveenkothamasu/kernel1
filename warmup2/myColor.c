#include<stdio.h>
#include<stdlib.h>
int main(){

	enum color_t {RED =0, BLUE=1, YELLOW=2 };
	int myColor = BLUE;

	if(myColor = 1){
		printf("\n My color is BLUE");	
	}
	myColor = RED;
	printf("\n My color is RED");	
	
	myColor = 3;
	printf("\n My color is %d", myColor);
	
	printf("\n");	
	
	return 0;
}
