#include <stdio.h>
#include "../include/scheduler.h"
#include "../include/metrics.h"

void calculate_and_print_metrics(SchedulerState *state) {
    int n = state->num_processes;

    calculate_metrics(state->processes, n);

    printf("%-10s %-5s %-5s %-5s %-5s %-5s %-5s\n",
           "Process", "AT", "BT", "FT", "TT", "WT", "RT");

    for (int i = 0; i < n; i++) {
        Process *p = &state->processes[i];
        printf("%-10s %-5d %-5d %-5d %-5d %-5d %-5d\n",
               p->pid, p->arrival_time, p->burst_time,
               p->finish_time, p->turnaround_time,
               p->waiting_time, p->response_time);
    }

    printf("\nAverage Turnaround Time: %.2f\n", calculate_average_turnaround(state->processes, n));
    printf("Average Waiting Time: %.2f\n", calculate_average_waiting(state->processes, n));
    printf("Average Response Time: %.2f\n", calculate_average_response(state->processes, n));
}