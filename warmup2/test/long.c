#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(){

	long i=0;
	int k=sizeof(long);
	printf("size of int=%lu bytes\n",  sizeof(int));
	printf("size of long=%lu bytes\n",  sizeof(long));
	int num_bits = k*8;
	printf("so the number of bits=%d\n", num_bits);
	printf("Max val of long is %g\n", pow(2,num_bits)-1);
	
}
