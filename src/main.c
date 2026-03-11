#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "../include/scheduler.h"

int main() {
    int n = 3; 
    SchedulerState state;
    state.num_processes = n;
    state.processes = malloc(sizeof(Process) * n);
    state.gantt_chart = malloc(sizeof(int) * 1000); 
    state.current_time = 0;

    sprintf(state.processes[0].pid, "P1");
    state.processes[0].arrival_time = 0;
    state.processes[0].burst_time = 240;
    state.processes[0].remaining_time = 240;

    sprintf(state.processes[1].pid, "P2");
    state.processes[1].arrival_time = 10;
    state.processes[1].burst_time = 180;
    state.processes[1].remaining_time = 180;
    
    sprintf(state.processes[2].pid, "P3");
    state.processes[2].arrival_time = 20;
    state.processes[2].burst_time = 150;
    state.processes[2].remaining_time = 150;

    schedule_fcfs(&state);

    //print_gantt_chart(&state);
    calculate_and_print_metrics(&state);

    free(state.processes);
    free(state.gantt_chart);

    return 0;
} 