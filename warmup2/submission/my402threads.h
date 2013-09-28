#include<sys/time.h>
#include "my402list.h"
#include "cs402.h"
#include "my402constants.h"

typedef struct tagFilterData {

	My402List *pListQ1;
	My402List *pListQ2;
	int tokenCount;

	int isStopNow;
	int isMorePackets;
	
} My402FilterData;

typedef struct tagPrintTime{
	
	int intPart;
	int decPart;
	double actual_num;

} printtime;

typedef struct tagMyPacket {
	
	long long int packet_num; 
	long long inter_arrival_time;
	int tokens;
	long long service_time;
	printtime q1_begin_time;
	printtime q1_end_time;
	printtime q2_begin_time;
	printtime q2_end_time;
} My402Packet;

void *arrivalManager(void *);
void *tokenManager(void *);
void *serviceManager(void *);
