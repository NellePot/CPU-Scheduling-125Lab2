#include "../include/scheduler.h"
#include <stdio.h>

int schedule_fcfs(SchedulerState *state) {
    int current_time = 0;
    int completed = 0;

    while (completed < state->num_processes) {
        int idx = get_next_process(state, current_time, compare_fcfs);  // Get lang new process if wala pay ga run 

        if (idx != -1) {
            Process *p = &state->processes[idx];
            if (p->start_time == -1) p->start_time = current_time;

            while (p->remaining_time > 0) {                         // Run until completion 
                state->gantt_chart[current_time] = idx;
                p->remaining_time--;
                current_time++;
            }
            p->finish_time = current_time;

            int waiting = p->finish_time - p->arrival_time - p->burst_time;
             if (waiting > 0) {
                printf("Convoy effect detected: Process %s waited %d time units\n",
                       p->pid, waiting);
            }

            completed++;
        } else {
            state->gantt_chart[current_time++] = -1;                // Idle
        }
    }
    state->total_time = current_time;
    return 0;
}