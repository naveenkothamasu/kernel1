#include<stdio.h>
#include<stdlib.h>
int main(int argc, char *argv[]){
	
	char buf[50] = {'\0'};
	int i= 5;
	int j=0;
	char str[80] = "file " ;	
	char temp[12] = " > temp.txt";
	FILE *fp = fopen("fileType.sh","w");
	//printf("\nfile name:%s\n", argv[1]);	
	while(*(argv[1]+j) != '\0'){
		str[i] = *(argv[1]+j);	
		i++;
		j++;
	}	
	j=0;
	while(j<=11){
		str[i++] = temp[j];	
		j++;	
	}
	str[i] = '\0';
	//printf("\nistr =%s\n",str);
	fwrite(str, strlen(str),1, fp);
	system("sh fileType.sh");
	fp = fopen("temp.txt","r");	
	fgets(buf,50,fp);
	printf("file type: %s",buf);
	fclose(fp);
	

}
