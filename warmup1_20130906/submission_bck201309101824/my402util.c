#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char *toCurrency(long long);
char *reverse(char *);
char *itoa(long long);
char *representNegatives(char *str);
int myDigitCheck(char *a);

/*
int main(int argc, char *argv[]){

	long long num = 1000000001;
	//printf("\nmain: argvc=%d\n", argc);
	if(argc>1){
		//printf("\nmain: argv[0]=%d\n", *argv[0]);
		//printf("\nmain: argv[1]=%s\n", argv[1]);
		num = atoi(argv[1]);	
	}
	char *str = NULL;
	//printf("\nsizeof(unsigned int)=  %u", sizeof(unsigned int));	
	str = toCurrency(num);
	//printf("Given num:%lld", num);
	printf("String:%s\n", str);

	return 0;
}
*/
char *toCurrency(long long num){

	int i=0;		
	int j = 0;
	int count =1;
	int len = 0;
	long long intPart = num/100;
	int decimalPart = num%100;
	//printf("\ntoCurrency: decimalPart = %d\n",decimalPart);
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
	//printf("\n Inside util: %llu\n",num);	
	if(num >= 1000000000){ //because the number is in cents. The limit is 1B
		for(i=0; i<9; i++){
			str[i] = '?';	
		}
		str[i] = '\0';	
	}else{
		//printf("\n integer part=%lld\n",intPart);	
		str = itoa(intPart);
	}	
	i = strlen(str)-1;
	for(; i>=0; i--){
		dest[j++] = str[i];
		if( count%3 == 0 && i-1>=0 ){
			dest[j++] = ',';	
		}
		count++;	
	}
	dest[j] = '\0';
	dest = reverse(dest);
	if(num < 1000000000){
		len = strlen(dest);
		dest[len+1] = decimalPart%10+'0';
		decimalPart = decimalPart/10;
		dest[len] = decimalPart+'0';
		dest[len+2] = '\0';
	}
	len = strlen(dest);
	//There is enough space for 1 char in dest
	dest[len+1] = '\0';
	dest[len] = dest[len-1];
	dest[len-1] = dest[len-2];
	dest[len-2] = '.';
	//printf("\ntoCurrency: string=%s",dest);	
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

char *itoa(long long num){

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

char *representNegatives(char *str){
	
	if(str == NULL){
		return NULL;	
	}
	char *localStr = (char *)malloc(15*sizeof(char));
	if(localStr == NULL){
		fprintf(stderr,"\nUnable to allocate memory");	
	}	
	memset(localStr,'\0',15*sizeof(char));	
	localStr[0] = '(';
	strncpy(localStr+1, str+2,14);
	localStr[13] = ')';
	localStr[14] = '\0';
	return localStr;	
}

int myDigitCheck(char *a){

        int i=0;
        for(i=0; i<strlen(a); i++){
                if(! (0+'0'<=a[i] && a[i]<=9+'0')){
                        printf("\n Found a non-digit char:%c\n",a[i]);
                        return 0;
                }
        }
        return 1;
}

