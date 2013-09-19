#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<string.h>

int main(){

	char a[3] = {'\0'};
	FILE *fp = NULL;
	struct stat *buf = malloc(sizeof(struct stat));
	memset(buf, '\0',sizeof(struct stat));
	fp = fopen("a.out","r");
	int f = fread(a,1,3,fp);
	if(f == 0){
		perror("\nfread failed");	
	}
	stat("test", buf);
	printf("\nst_mode: %d",S_ISDIR(buf->st_mode));	
	fclose(fp);
		
	
	return 0;
}
