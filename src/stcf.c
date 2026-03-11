#include "../include/scheduler.h"
#include <stdio.h>
#include <string.h>

int schedule_stcf(SchedulerState *state) {
    Process *processes = state->processes;
    int n = state->num_processes;
    int current_time = state->current_time;
    int completed = 0;

    // Initialize process fields
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].start_time = -1;
        processes[i].finish_time = -1;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].response_time = 0;
    }

    while (completed < n) {
        int idx = -1;

        // Find the ready process with the shortest remaining time
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time &&
                processes[i].remaining_time > 0) {

                if (idx == -1 ||
                    processes[i].remaining_time < processes[idx].remaining_time) {
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            current_time++; //no process ready to run, increment time
            continue;
        }

        if (processes[idx].start_time == -1) {
            processes[idx].start_time = current_time;
        }

        // Run for only 1 time unit
        processes[idx].remaining_time--;
        current_time++;

        // If process finishes
        if (processes[idx].remaining_time == 0) {
            processes[idx].finish_time = current_time;
            completed++;
        }
    }

    /*metric calculation
    for (int i = 0; i < n; i++) {
        processes[i].turnaround_time = processes[i].finish_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
        processes[i].response_time = processes[i].start_time - processes[i].arrival_time;
    }*/

    state->current_time = current_time;
    return 0;
}