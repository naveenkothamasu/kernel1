#include<stdio.h>
#include<stdlib.h>

int intVal(char *str);
int
main(int argc, char **argv){

	/*	
	 warmup2 [-lambda lambda] [-mu mu] \
        [-r r] [-B B] [-P P] [-n num] \
        [-t tsfile]
	**/
	
	double lambda = 0.5, mu = 0.35, r = 1.5;
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
			lambda = strtod(argv[i+1], NULL);
			i++;
			break;
		case 2:
			
			mu = strtod(argv[i+1], NULL);
			i++;
			break;
		case 3:
			r = strtod(argv[i+1], NULL);
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
			printf("lambda = %g\n", lambda);
			printf("mu = %g\n", mu);
			printf("r = %g\n", r);
			printf("B = %d\n", B);
			printf("P = %d\n", P);
			printf("n = %d\n", n);
			printf("t = %s\n", t);
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

