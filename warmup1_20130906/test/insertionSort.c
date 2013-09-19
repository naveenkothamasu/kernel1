#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 13 
void printArray(int a[]);
void insertionSort();
void exchange(int *,int *);
int main(){


	int a[MAX] = {7,6,5,4,3,2,1,8,54,66,97,12,43};
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
		j=i;
		while(a[j-1]>key && j>=0){
			//keep moving a[j]
			a[j]=a[j-1];
			j--;	
		}
		a[j] = key;
          }   
}    
	//printf("\n inside insertionSort");
	//printArray(a);


