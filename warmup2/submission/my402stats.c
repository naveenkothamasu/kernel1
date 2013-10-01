#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include "my402threads.h"

//TODO: need a mutex on stats?

void runStats(My402ArrivalStats *aStats, My402TokenStats *tStats, My402ServiceStats *sStats){

	
	double emulation_time = sStats->emulation_time.tv_sec * 1000000 + sStats->emulation_time.tv_usec;
 	printf("Statistics:\n");
        printf("\taverage packet inter-arrival time = %g\n", (double)((double)aStats->avg_inter_arrival_time/(double)1000000) );
        printf("\taverage packet service time = %g\n", (double) ((double)sStats->avg_service_time/(double)1000000));
	printf("\n");
 	printf("\taverage number of packets in Q1 = %g\n",(double) ( ((double) aStats->time_spent_q1+(double) tStats->time_spent_q1) / emulation_time));
 	printf("\taverage number of packets in Q2 = %g\n",(double) ( (double) sStats->time_spent_q2/ emulation_time));
 	printf("\taverage number of packets in S = %g\n",(double) ( (double) sStats->time_spent_s/ emulation_time));
	printf("\n");
 	printf("\taverage time a packet spent in system = %g\n", (double)((double)sStats->system_time/((double)1000000*(double)sStats->packets_served)));
	printf("\tstandard deviation for time spent in system = %g\n", (double) sStats->sd);
	printf("\n");
	if( tStats->current_tokens == 0){
		printf("\ttoken drop probability = N/A (no token arrived at this facility)\n" );
	}else{
		printf("\ttoken drop probability = %g\n", (double)tStats->tokens_dropped/(double)tStats->current_tokens);
	}
	if( aStats->current_packets == 0){
		printf("\tpacket drop probability = N/A (no packet arrived at this facility)\n" );
	}else{
		printf("\tpacket drop probability = %g\n",(double)aStats->packets_dropped/(double)aStats->current_packets);	
	}
}

double getNewAvgByNewNum(double old_avg, double val, int new_num){

	int old_num = new_num -1;
	return getAvg(old_avg, val, old_num);
}
double getAvg(double old_avg, long long val, int old_num){

	double current_avg = (old_avg* (double) old_num + val) / (old_num+1);
	return current_avg;
}
//increase packets_served after this call by 1;
double getSD(My402ServiceStats *stats, long long elem){

	if(stats->packets_served == 0){
		return 0;	
	}
	double sd = 0;
	double old_avg = (double) stats->system_time / stats->packets_served;	
	double ratio = pow(stats->sd,2)+ pow(old_avg, 2);
	double old_avg_sqred = ratio * (double) stats->packets_served;
	double new_avg = getAvg(old_avg, elem, stats->packets_served);

	double firstTerm = (old_avg_sqred + pow(elem,2)) / (double)(stats->packets_served+1);
	double variance = firstTerm +  pow(new_avg,2);
	sd = sqrt(variance);	

	return sd;
}
