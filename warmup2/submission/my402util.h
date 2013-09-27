#include<string.h>
#include "cs402.h"
#include "my402list.h"

int isTokenBucketFull(int, int);
void printList(My402List *);
struct timeval sub_timeval(struct timeval , struct timeval);
struct timeval add_timeval(struct timeval , struct timeval);
struct printtime sub_printtime(struct timeval op1, struct timeval op2);
