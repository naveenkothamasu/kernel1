#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "cs402.h"

#include "my402threads.h"

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
	for(; i<argc; i++){
	
		k = intVal(argv[i]);
		switch(k){
		
		case 1:
			lambda = strtod(argv[i+1], NULL);
			if(lambda == 0){
				fprintf(stderr, "invalid input, malformed command\n");	
				exit(EXIT_FAILURE);
			}	
			if(lambda < 0.1){
				lambda = 0.1;	
			}
			i++;
			break;
		case 2:
			
			mu = strtod(argv[i+1], NULL);
			if(mu < 0.1){
				mu = 0.1;	
			}
			if(mu == 0){
				fprintf(stderr, "invalid input, malformed command\n");	
				exit(EXIT_FAILURE);
			}	
			i++;
			break;
		case 3:
			r = strtod(argv[i+1], NULL);
			if(r < 0.1){
				r = 0.1;	
			}
			if(r == 0){
				fprintf(stderr, "invalid input, malformed command\n");	
				exit(EXIT_FAILURE);
			}	
			i++;
			break;
		case 4:
			B = strtol(argv[i+1], NULL, 10);
			if(B == 0){
				fprintf(stderr, "invalid input, malformed command\n");	
				exit(EXIT_FAILURE);
			}	
			i++;
			break;
		case 5:
			P = strtol(argv[i+1], NULL, 10);
			if(P == 0){
				fprintf(stderr, "invalid input, malformed command\n");	
				exit(EXIT_FAILURE);
			}
			i++;
			break;
		case 6:
			n = strtol(argv[i+1], NULL, 10);
			if(n == 0){
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
		if(t != NULL){
			return TRUE;
		}else{
			return FALSE;
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
	}
	
	return -1;
}

