#include<stdio.h>
#include<stdlib.h>

double getSD(My402RunStats stats, double elem);

int
main(){

        double sd = 0;
        double ratio = pow(stats.sd,2)+ pow(stats.avg_spent_system, 2); 
        double sum = ratio * (double) stats.packets_served;
        double new_avg = getNewAvg(stats.avg_spent_system, stats.cPacket_time,  stats.packets_served);
        double variance = ((sum+ pow(elem,2))/ stats.packets_served + 1) - pow(new_avg,2);
        //sd = sqrt(variance);    
	printf("var=%g\n", variance);

}

