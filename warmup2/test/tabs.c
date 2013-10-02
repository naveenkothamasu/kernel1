#include<stdio.h>
#include<stdlib.h>

int
main(int arg, char **argv){

	FILE *fp = fopen("temp.txt","r");
	char *buf = malloc(sizeof(50));	
	char *out = NULL;
	char *in = NULL;
	char *hi[3]; 
	char arrival[50]= {'\0'};
	hi = arrival;
	char tokens[50]= {'\0'};
	hi = tokens;
	char service[50] = {'\0'};
	hi = service;

	while(fgets(buf,sizeof(buf), fp)){
	
		out = buf;
		while(*out != '\n'){
			in=out;
			while(*in != '\t' || *in != ' '){
				in++;
			}
			strncpy(hi[i], out-in);		
			
		}

	}
	
}
