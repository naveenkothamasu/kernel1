#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include "my402threads.h"

//TODO: need a mutex on stats?

void runStats(My402FilterData filter){

	
	printf("runStats: inside..\n");
	stats->inter_arrival_time = currentAvg(stats->inter_arrival_time, stats->packet_num);
	stats->service_time = currentAvg(stats->service_time, stats->packet_num);

	stats->time_in_system = currentAvg(stats->time_in_system, stats->served_num);

 	printf("\tStatistics:"\n)

        printf("\t\taverage packet inter-arrival time = %g\n", stats.avg_inter_arrival_time);
        printf("\t\taverage packet service time = %g\n", stats.avg_service_time);
    
 	printf("\t\taverage number of packets in Q1 = %g\n",stats.avg_packets_in_q1);
 	printf("\t\taverage number of packets in Q2 = %g\n",stats.avg_packets_in_q2);
 	printf("\t\taverage number of packets at S = %g\n",stats.avg_packets_in_s);
    
 	printf("\t\taverage time a packet spent in system = %g\n", stats.packet_spent_time);
	printf("\t\tstandard deviation for time spent in system = %g\n", stats.sd);

	printf("\t\ttoken drop probability = %g", (double)stats.tokens_dropped/(double)stats.current_tokens);
	printf("\t\tpacket drop probability = %g",(double)stats.packets_dropped/(double)stats.current_packets);	

}


double getNewAvg(double old_avg, double val, int old_num){

	double current_avg = (old_avg* (double) old_num + val) / (old_num+1);
	return current_avg;
}
//increase packets_served after this call by 1;
double getSD(My402RunStats *stats, double elem){

	double sd = 0;
	double ratio = pow(stats->sd,2)+ pow(stats->avg_spent_system, 2);
	double old_avg_sqred = ratio * (double) stats->packets_served;
	
	firstTerm = (old_avg_sqred + pow(elem,2)) / (double)(packets_served+1);
	double new_avg = getNewAvg(stats->avg_spent_system, elem,  stats->packets_served);
	stats->avg_spent_system = new_avg;
	double variance = firstTerm +  pow(new_avg,2);
	sd = sqrt(variance);	

	return sd;
}
