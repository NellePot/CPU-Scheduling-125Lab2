#include "../include/scheduler.h"
#include <stdio.h>

int schedule_stcf(SchedulerState *state) {
    Process *processes = state->processes;
    int n = state->num_processes;
    int current_time = 0;
    int completed = 0;

    while (completed < n) {
        int idx = -1;

        // Find ready process with shortest remaining time
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time &&
                processes[i].remaining_time > 0) {

                if (idx == -1 ||
                    processes[i].remaining_time < processes[idx].remaining_time) {
                    idx = i;
                }
            }
        }

        // CPU idle
        if (idx == -1) {
            state->gantt_chart[current_time] = -1;
            current_time++;
            continue;
        }

        // First time this process gets CPU
        if (processes[idx].start_time == -1) {
            processes[idx].start_time = current_time;
        }

        // Record Gantt chart
        state->gantt_chart[current_time] = idx;

        // Run for 1 time unit
        processes[idx].remaining_time--;
        current_time++;

        // If finished
        if (processes[idx].remaining_time == 0) {
            processes[idx].finish_time = current_time;
            completed++;
        }
    }

    state->total_time = current_time;
    return 0;
}