#ifndef METRICS_H
#define METRICS_H

#include "scheduler.h"

void calculate_metrics(Process *processes, int n);
double calculate_average_turnaround(Process *processes, int n);
double calculate_average_waiting(Process *processes, int n);
double calculate_average_response(Process *processes, int n);
void calculate_and_print_metrics(SchedulerState *state);

#endif 