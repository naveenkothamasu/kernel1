#include<stdio.h>
#include<stdlib.h>

int intVal(char *str);
int
main(int argc, char **argv){

	/**
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
			printf("lambda = %s\n", argv[i+1], i);
			i++;
			break;
		case 2:
			printf("mu = %s\n", argv[i+1]);
			i++;
			break;
		case 3:
			printf("r = %s\n", argv[i+1]);
			i++;
			break;
		case 4:
			printf("B = %s\n", argv[i+1]);
			i++;
			break;
		case 5:
			printf("P = %s\n", argv[i+1]);
			i++;
			break;
		case 6:
			printf("n = %s\n", argv[i+1]);
			i++;
			break;
		case 7:
			printf("t = %s\n", argv[i+1]);
			i++;
			break;
		default:
			printf("some unidentified arg=%s\n", argv[i]);
			break;
		
		}	
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
