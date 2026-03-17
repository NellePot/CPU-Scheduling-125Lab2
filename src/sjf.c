#include "../include/scheduler.h"
#include <stdio.h>

int schedule_sjf(SchedulerState *state) {
    Process *processes = state->processes;
    int n = state->num_processes;
    int current_time = 0;
    int completed = 0;

    while (completed < n) {
        int idx = -1;

        // find shorted job among ready processes
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time &&
                processes[i].remaining_time > 0) {

                if (idx == -1 || processes[i].burst_time < processes[idx].burst_time) {
                    idx = i;
                }
            }
        }

        // CPU idle if no ready processes
        if (idx == -1) {
            state->gantt_chart[current_time] = -1;
            current_time++;
            continue;
        }

        // record first start time
        if (processes[idx].start_time == -1) {
            processes[idx].start_time = current_time;
        }

        // run selected shortest job until completion
        while (processes[idx].remaining_time > 0) {
            state->gantt_chart[current_time] = idx;
            processes[idx].remaining_time--;
            current_time++;
        }

        processes[idx].finish_time = current_time;
        completed++;
    }

    state->total_time = current_time;
    return 0;
}