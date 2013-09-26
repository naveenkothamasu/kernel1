#include<sys/time.h>
#include "my402list.h"
#include "cs402.h"
#include "my402util.h"
#include "my402constants.h"

typedef struct tagFilterData {

	My402List *pListQ1;
	My402List *pListQ2;
	int tokenCount;

	int isStopNow;
	int isMorePackets;
	
} My402FilterData;

typedef struct tagMyPacket {
	
	long long int packet_num; //needed?
	long long inter_arrival_time;
	int tokens;
	long long service_time;
	struct timeval q1_begin_time;
	struct timeval q1_end_time;
	struct timeval q2_begin_time;
	struct timeval q2_end_time;
} My402Packet;

void *arrivalManager(void *);
void *tokenManager(void *);
void *serviceManager(void *);
void tvcpy(struct timeval, struct timeval);
