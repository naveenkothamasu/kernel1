#include "my402list.h"
#include "cs402.h"

typedef tagFilterData{

	My402List *pListQ1;
	My402List *pListQ2;
	int tokenCount;

	int isStopNow;
	
}My402FilterData;

typedef tagMyPacket {
	
	long long inter_arrival_time;
	int tokens;
	long long service_time;
	struct timeVal q1_begin_time;
	struct timeVal q1_end_time;
	struct timeVal q2_begin_time;
	struct timeVal q2_end_time;
}My402Packet;

void *arrivalManager(void *);
void *tokenManager(void *);
void *serviceManager(void *);

