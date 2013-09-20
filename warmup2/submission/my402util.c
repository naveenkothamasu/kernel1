#include<stdio.h>
#include<stdlib.h>

#include "my402util.h"

int isTokenBucketFull(int tokensAsOfNow, int B){
	
	return B==tokensAsOfNow ? TRUE : FALSE;
}
