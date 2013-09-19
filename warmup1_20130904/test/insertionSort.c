#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 4 
void printArray(int a[]);
void insertionSort();
void exchange(int *,int *);
int main(){


	int a[MAX] = {4,6,1,2};
	printArray(a);	
	insertionSort(a);
	printArray(a);	


}

void printArray(int a[]){
	int i=0;	
	printf("\n");
	for(i=0; i<MAX; i++){
		printf("%d ",a[i]);	
	}
	printf("\n");
}

void insertionSort(int a[]){
        int i=0;
	int j=0;    
        int key=a[1];   	int temp=0;
        for(i=1; i<MAX ; i++){
    
                //place a[i] in a[0...i-1]
                key=a[i];
                for(j=i-1; j>=0; j--){
                        if(a[j] >  key){
                                //exchange(&a[j],&key);
				temp = a[j];	
				a[j] =key;
				key = temp;
                        }    
                }   

        }    
	//printf("\n inside insertionSort");
	//printArray(a);
}

void exchange(int *a, int *b){ 
        int *temp=NULL;
        temp = a;
        a=b;
        b=temp;
}
