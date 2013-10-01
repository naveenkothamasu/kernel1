#include<stdio.h>
#include<stdlib.h>

double getSD(My402RunStats stats, double elem);

int
main(){

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

