#include<stdio.h>
#include<stdlib.h>

struct printtime{
	int intPart;
	int decPart;
};
void fun(struct printtime *);
int main(){

	struct printtime s;
	fun(&s);	
	printf("printtime.intPart = %d\n", s.intPart);
	printf("printtime.decPart = %d\n", s.decPart);
}

void fun(struct printtime *p){
	p->intPart = 7;
	p->decPart = 8;
}
