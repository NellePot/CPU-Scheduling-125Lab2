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
    if (n<=0) return 0.0;                                                   // avoid division by zero (fix)

    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += processes[i].turnaround_time;
    }
    return sum / n;
}

double calculate_average_waiting(Process *processes, int n) {
    if (n<=0) return 0.0;               
                                        
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += processes[i].waiting_time;
    }
    return sum / n;
}

double calculate_average_response(Process *processes, int n) {
    if (n<=0) return 0.0;               
                                        
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += processes[i].response_time;
    }
    return sum / n;
}
