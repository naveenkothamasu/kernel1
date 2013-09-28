#include<string.h>
#include "cs402.h"
#include "my402list.h"
#include "my402threads.h"

int isTokenBucketFull(int, int);
void printList(My402List *);
void sub_timeval(struct timeval *,struct timeval , struct timeval);
void add_timeval(struct timeval *,struct timeval , struct timeval);
void sub_printtime(printtime *,struct timeval ,struct timeval);
int isPositive_timeval(struct timeval);
void getcurrenttime(printtime *, struct timeval);
void double_to_timeval(struct timeval *, double);
void timeval_to_printtime(printtime *, struct timeval);
void func(struct timeval *, long long );

