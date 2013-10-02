#include<sys/time.h>
#include "my402list.h"
#include "cs402.h"
#include "my402constants.h"

#define handle_errors(err, exception) \
	do{errno = err; perror(exception); exit(EXIT_FAILURE); }while(0)


typedef struct tagMyStats {
	
	double avg_time_in_q1;
	double sd_time_spent_system;
	double avg_time_spent_system;
 
} My402Stats;
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
	long long  actual_num;

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
	printtime arrivalStamp;
} My402Packet;


typedef struct tagArrivalStats {

	double avg_inter_arrival_time;
	double avg_packets_in_q1;
	long packets_dropped;
	long current_packets;
	long long time_spent_q1;

} My402ArrivalStats;

typedef struct tagTokenStats {

	long tokens_dropped;
	long current_tokens;
	long long time_spent_q1;

} My402TokenStats;

typedef struct tagServiceStats {

	long long time_spent_q2;
	long long time_spent_s;
	long long system_time;
	double avg_service_time;
	double avg_packets_in_q2;
	double avg_packets_in_s;

	long long packets_served;
	struct timeval  emulation_time;
	double sd;
	double old_avg;
} My402ServiceStats;


void *arrivalManager(void *);
void *tokenManager(void *);
void *serviceManager(void *);
void runStats(My402ArrivalStats *aStats, My402TokenStats *tStats, My402ServiceStats *sStats);
double getNewAvgByNewNum(double old_avg, double val, int new_num);
double getAvg(double old_avg, long long val, int old_num);
void getStandardDeviation(My402ServiceStats *, long val);


