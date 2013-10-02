#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "cs402.h"

#include "my402util.h"

int intVal(char *str);
extern double lambda, mu, r;
extern int B, P, n;
extern char *t;
int
isDeterministicMode(int argc, char *argv[]){

	/*	
	 warmup2 [-lambda lambda] [-mu mu] \
        [-r r] [-B B] [-P P] [-n num] \
        [-t tsfile]
	**/
	int i=0;
	int k=0;
	double aLambda = lambda;
	double aMu = mu;
	double aR = r;
	for(; i<argc; i++){
	
		k = intVal(argv[i]);
		switch(k){
		
		case 1:
			lambda = strtod(argv[i+1], NULL);
			if(lambda <= 0){
				fprintf(stderr, "lambda must be a positive real number\n");
				fprintf(stderr, "invalid input, malformed command\n");	
				exit(EXIT_FAILURE);
			}	
			aLambda = lambda;
			if(lambda < 0.1){
				lambda = 0.1;	
			}
			i++;
			break;
		case 2:
			
			mu = strtod(argv[i+1], NULL);
			if(mu <= 0){
				fprintf(stderr, "mu must be a positive real number\n");	
				fprintf(stderr, "invalid input, malformed command\n");	
				exit(EXIT_FAILURE);
			}
			if(mu < 0.1){
				mu = 0.1;	
			}
			aMu = mu;
				
			i++;
			break;
		case 3:
			r = strtod(argv[i+1], NULL);
			if(r <= 0){
				fprintf(stderr, "r must be a positive real number\n");	
				fprintf(stderr, "invalid input, malformed command\n");	
				exit(EXIT_FAILURE);
			}
			if(r < 0.1){
				r = 0.1;	
			}
			aR = r;
				
			i++;
			break;
		case 4:
			/*
			 *	1. non-digits
				2. decimal point
				3. negatives
				4. 0
				5. Max val
				
			*/
			if(isPositiveInt(argv[i+1]) == FALSE){
				
				fprintf(stderr, "B must be a positive integer\n");	
				fprintf(stderr, "invalid input, malformed command\n");	
				exit(EXIT_FAILURE);
			}
			B = strtol(argv[i+1], NULL, 10);
			if(B <= 0){
				fprintf(stderr, "B must be a positive integer\n");	
				fprintf(stderr, "invalid input, malformed command\n");	
				exit(EXIT_FAILURE);
			}
			//this check might not be needed, as the vlaue in B itself would be -ve, once the OVERFLOW		
			if(B > 2147483647 ){
				fprintf(stderr, "The max valu of B is 2147483647 \n");	
				fprintf(stderr, "invalid input, malformed command\n");	
				exit(EXIT_FAILURE);
			
			}
			i++;
			break;
		case 5:
			if(isPositiveInt(argv[i+1]) == FALSE){
				
				fprintf(stderr, "P must be a positive integer\n");	
				fprintf(stderr, "invalid input, malformed command\n");	
				exit(EXIT_FAILURE);
			}
			P = strtol(argv[i+1], NULL, 10);
			if( P <= 0){
				fprintf(stderr, "P must be a positive integer\n");	
				fprintf(stderr, "invalid input, malformed command\n");	
				exit(EXIT_FAILURE);
			}
			i++;
			break;
		case 6:
			if(isPositiveInt(argv[i+1]) == FALSE){
				
				fprintf(stderr, "n must be a positive integer\n");	
				fprintf(stderr, "invalid input, malformed command\n");	
				exit(EXIT_FAILURE);
			}
			n = strtol(argv[i+1], NULL, 10);
			if(n <= 0){
				fprintf(stderr, "n must be a positive integer\n");	
				fprintf(stderr, "invalid input, malformed command\n");	
				exit(EXIT_FAILURE);
			}	
			i++;
			break;
		case 7:
			t = argv[i+1];
			if(t == NULL){
				fprintf(stderr, "invalid input, malformed command\n");	
				exit(EXIT_FAILURE);
			}	
			i++;
			break;
		case 8:
			break; //OK
		default:
			printf("some unidentified arg=%s\n", argv[i]);
			fprintf(stderr, "invalid input, malformed command\n");	
			//exit(EXIT_FAILURE);
			break;
		
		}	
	}
	printf("Emulation Parameters:\n");
	if(*t == '\0'){
		printf("\tlambda = %g\n", aLambda);
		printf("\tmu = %g\n", aMu);
		printf("\tnumber to arrive = %d\n", n);
	}
	printf("\tr = %g\n", aR);
	printf("\tB = %d\n", B);
	printf("\tP = %d\n", P);

	if(*t != '\0'){
		printf("\ttsfile = %s\n", t);
		return FALSE;
	}else{
		return TRUE;
	}
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
	}else if(strcmp("warmup2",str)== 0 || strcmp("./warmup2",str)== 0){
		return 8;
	}
	
	return -1;
}

