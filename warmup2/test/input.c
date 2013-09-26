#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int intVal(char *str);
float strToFloat(char *str);
int
main(int argc, char **argv){

	
	strToFloat("123230.35");
	
	/**
	 warmup2 [-lambda lambda] [-mu mu] \
        [-r r] [-B B] [-P P] [-n num] \
        [-t tsfile]
	**/
	/*
	float lambda = 0.5, mu = 0.35, r = 1.5;
	int B = 10, P = 3, n = 20;
	char *t = (char *)malloc(20*sizeof(char));
	if(t == NULL){
		fprintf(stderr, "malloc() failed - unable to allocate memory\n");	
	}
	int i=0;
	int k=0;
	for(; i<argc; i++){
	
		k = intVal(argv[i]);
		switch(k){
		
		case 1:
			lambda = strToFloat(argv[i+1]);
			i++;
			break;
		case 2:
			
			mu = strToFloat(argv[i+1]);
			i++;
			break;
		case 3:
			r = strToFloat(argv[i+1]);
			i++;
			break;
		case 4:
			B = atoi(argv[i+1]);
			i++;
			break;
		case 5:
			P = atoi(argv[i+1]);
			i++;
			break;
		case 6:
			n = atoi(argv[i+1]);
			i++;
			break;
		case 7:
			t = argv[i+1];
			i++;
			break;
		default:
			printf("some unidentified arg=%s\n", argv[i]);
			break;
		
		}	
	}
			printf("lambda = %f\n", lambda);
			printf("mu = %f\n", mu);
			printf("r = %f\n", r);
			printf("B = %d\n", B);
			printf("P = %d\n", P);
			printf("n = %d\n", n);
			printf("t = %s\n", t);
	*/
}

int intVal(char *str){

	if(strcmp("-lambda",str) == 0){
		return 1;	
	}else if(strcmp("-mu",str) == 0){
		return 2;
	}else if(strcmp("-r",str)== 0){
		return 3;
	}else if(strcmp("-B",str)== 0){
		return 4;
	}else if(strcmp("-P",str)== 0){
		return 5;
	}else if(strcmp("-n",str)== 0){
		return 6;
	}else if(strcmp("-t",str)== 0){
		return 7;
	}
	
	return -1;
}

float strToFloat(char *str){

	int i=0;
	int digit = 0;
	int n = 0;
	int intPart = 0;
	int dotPos = 0;
	double num =0;
	while(str[i] != '.'){
		i++;	
	}
	//TODO:exactly one dot present
	dotPos = i;
	i =i-1;
	while(i >= 0){
		digit = str[i]-'0'; //TODO: str[i] is only a digit char and nothing else
		intPart = intPart + digit* pow(10,n);	
		i--;	
		n++;
	}
	//printf("intPart=%d\n",intPart);
	i=dotPos+1;
	num = (float) intPart;
	n=1;	
	while(str[i] != '\0'){

		digit = str[i]-'0';
		//printf("current float %f\n", digit/pow(10,n));
		num = num + ((double)digit / pow(10,n));
		i++;	
		n++;
	}
	printf("num=%g\n",num);	
}
