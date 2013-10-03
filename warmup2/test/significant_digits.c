#include<stdio.h>
#include<stdlib.h>
int
main(int argc, char *argv[]){

	double val = strtod(argv[1],NULL);
	printf("val=%.6g\n", val);

}
