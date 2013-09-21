#include "my402list.h"
#include "cs402.h"

/*
typedef tagMyTimeVal{
	
}timeVal;
*/
typedef tagMyPacket {
	
	long long inter_arrival_time;
	int tokens;
	long long service_time;
	long long q1_timestamp;
	long long q2_timestamp;
}My402Packet;

void *arrivalManager(void *);
void *tokenManager(void *);
void *serviceManager(void *);

