#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(){
	

	char ch = '\0';
	char s[100] = {'\0'};
	int i=0;	
	scanf("%c",&ch);
	while(ch != 'k'){
		s[i++] = ch;	
		scanf("%c",&ch);
	}
	printf("%s", s);
	printf("right after");
	printf("\n is there a new line:%d\n", strchr(s, '\n')!=NULL?1:0);
	return 0;
}
