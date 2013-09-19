#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char *toCurrency(unsigned int);
char *reverse(char *);
char *itoa(unsigned int);

int main(int argc, char *argv[]){

	unsigned int num = 34234;
	//printf("\nmain: argvc=%d\n", argc);
	if(argc>1){
		//printf("\nmain: argv[0]=%d\n", *argv[0]);
		//printf("\nmain: argv[1]=%s\n", argv[1]);
		num = atoi(argv[1]);	
	}
	char *str = NULL;
	//printf("\nsizeof(unsigned int)=  %u", sizeof(unsigned int));	
	str = toCurrency(num);
	printf("Given num:%u", num);
	printf("\nString:%s\n", str);

	return 0;
}

char *toCurrency(unsigned int num){
	
	char *str = (char *) malloc(80*sizeof(char));
	char *dest = (char *) malloc(80*sizeof(char));
	if(str == NULL){
		fprintf(stderr, "\nUnable to allocate memory");	
	}
	if(dest == NULL){
		fprintf(stderr, "\nUnable to allocate memory");	
	}
	memset(str,'\0',80*sizeof(char));
	memset(dest,'\0',80*sizeof(char));
	str = itoa(num);
	int i = strlen(str)-1;
	int j = 0;
	int count =1;
	for(; i>=0; i--){
		dest[j++] = str[i];
		if( count%3 == 0 && i-1>=0 ){
			dest[j++] = ',';	
		}
		count++;	
	}
	//FIXME: for one char case
	dest[j] = '\0';
	//printf("\ntoCurrency: string=%s",dest);	
	dest = reverse(dest);
	
	return dest;	
}

char *reverse(char *dest){
	int i=0;
	
	int len = strlen(dest);
	int j = len-1;	
	//printf("\nreverse: len=%d", len);
	char temp = '\0';
	while(i < j){
		temp =dest[i];
		dest[i] = dest[j];
		dest[j] = temp;	
		i++;
		j--;
	}	
	//printf("\nreverse: string=%s",dest);	
	return dest;
}

char *itoa(unsigned int num){

	int i=0;
	char *str = (char *)malloc(11*sizeof(char));
	if(str == NULL){
		fprintf(stderr,"\nUnable to allocate memory");	
	}
	memset(str,'\0',11*sizeof(char));
	while(num != 0){
		str[i++] = num%10+'0';
		num = num/10;		
	}
	//FIXME: when num is a single digit
	str[i] = '\0';
	//printf("\nitoa: string=%s",str);	
	return reverse(str);
}
