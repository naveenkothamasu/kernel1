#include<stdio.h>
#include<stdlib.h>
int main(){

	double d = 2.7322449684143;
	int a = (int) (d*1000000);
	int b = (int)(d*(double)1000)*1000;
	int dec = a-b;
	printf("d=%g\n",d);
	printf("a=%d\n",a);
	printf("b=%d\n",b);
	printf("dec=%d\n",dec);
	printf("%08d.%d\n", (int) (d*1000), dec);
}
