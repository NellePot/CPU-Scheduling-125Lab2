#include <stdio.h>
#include "../include/scheduler.h"

void calculate_metrics(Process *processes, int n) {
    for (int i = 0; i < n; i++) {
        Process *p = &processes[i];
        p->turnaround_time = p->finish_time - p->arrival_time;
        p->waiting_time = p->turnaround_time - p->burst_time;
        p->response_time = p->start_time - p->arrival_time;
    }
}

double calculate_average_turnaround(Process *processes, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += processes[i].turnaround_time;
    }
    return sum / n;
}

void calculate_and_print_metrics(SchedulerState *state) {
    int n = state->num_processes;
    
    calculate_metrics(state->processes, n);
    
    printf("%-10s | %-3s | %-3s | %-3s | %-3s | %-3s | %-3s\n", 
           "Process", "AT", "BT", "FT", "TT", "WT", "RT");
    printf("----------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        Process *p = &state->processes[i];
        printf("%-10s | %-3d | %-3d | %-3d | %-3d | %-3d | %-3d\n",
               p->pid, p->arrival_time, p->burst_time, 
               p->finish_time, p->turnaround_time, p->waiting_time, p->response_time);
    }

    printf("\nAverage Turnaround Time: %.2f\n", calculate_average_turnaround(state->processes, n));
}