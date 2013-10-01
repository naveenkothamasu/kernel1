#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]){


	printf("argc=%d\n", argc);
	int i=0;
	for(;i<argc; i++){
		printf("argv[%d]=%s ", i, argv[i]);
	}
	printf("\n");
}
