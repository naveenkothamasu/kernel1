#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include "my402threads.h"

//TODO: need a mutex on stats?

void runStats(My402ArrivalStats *aStats, My402TokenStats *tStats, My402ServiceStats *sStats){

	
	//stats->avg_inter_arrival_time = currentAvg(stats->avg_inter_arrival_time, stats->packet_num);
	//stats->avg_service_time = currentAvg(stats->avg_service_time, stats->packet_num);

	//stats->time_in_system = currentAvg(stats->time_in_system, stats->served_num);

 	printf("\tStatistics:\n");

	double emulation_time = sStats->emulation_time.tv_sec * 1000000 + sStats->emulation_time.tv_usec;

        printf("\t\taverage packet inter-arrival time = %g\n", (double)((double)aStats->avg_inter_arrival_time/(double)1000000) );
        printf("\t\taverage packet service time = %g\n", (double) ((double)sStats->avg_service_time/(double)1000000));
 	printf("\t\taverage number of packets in Q1 = %g\n",(double) ( ((double) aStats->time_spent_q1+(double) tStats->time_spent_q1) / emulation_time));
 	printf("\t\taverage number of packets in Q2 = %g\n",(double) ( (double) sStats->time_spent_q2/ emulation_time));
 	printf("\t\taverage number of packets in S = %g\n",(double) ( (double) sStats->time_spent_s/ emulation_time));
    
 	//printf("\t\taverage time a packet spent in system = %g\n", stats->packet_spent_time);
	printf("\t\tstandard deviation for time spent in system = %g\n", (double) sStats->sd);
	if( tStats->current_tokens == 0){
		printf("\t\ttoken drop probability = N/A (no token arrived at this facility)\n" );
	}else{
		printf("tokens dropped %ld\n", tStats->tokens_dropped);
		printf("current tokens %ld\n", tStats->current_tokens);
		printf("\t\ttoken drop probability = %g\n", (double)tStats->tokens_dropped/(double)tStats->current_tokens);
	}
	if( aStats->current_packets == 0){
		printf("\t\tpacket drop probability = N/A (no packet arrived at this facility)\n" );
	}else{
		printf("\t\tpacket drop probability = %g\n",(double)aStats->packets_dropped/(double)aStats->current_packets);	
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

	double sd = 0;
	double old_avg = (double) stats->system_time / (double) stats->emulation_time.tv_usec;	
	double ratio = pow(stats->sd,2)+ pow(old_avg, 2);
	double old_avg_sqred = ratio * (double) stats->packets_served;
	double new_avg = getAvg(old_avg, elem, stats->packets_served);

	double firstTerm = (old_avg_sqred + pow(elem,2)) / (double)(stats->packets_served+1);
	double variance = firstTerm +  pow(new_avg,2);
	sd = sqrt(variance);	

	return sd;
}
