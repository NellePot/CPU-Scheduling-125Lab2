#include <stdio.h>
#include "../include/scheduler.h"

void calculate_metrics(Process *processes, int n) {
    for (int i = 0; i < n; i++) {
        Process *p = &processes[i];

        p->turnaround_time = p->finish_time - p->arrival_time;
        p->waiting_time = p->turnaround_time - p->burst_time;

        if (p->start_time != -1)
            p->response_time = p->start_time - p->arrival_time;
        else
            p->response_time = 0;
    }
}

double calculate_average_turnaround(Process *processes, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += processes[i].turnaround_time;
    }
    return sum / n;
}

double calculate_average_waiting(Process *processes, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += processes[i].waiting_time;
    }
    return sum / n;
}

double calculate_average_response(Process *processes, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += processes[i].response_time;
    }
    return sum / n;
}

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