#include "../include/scheduler.h"
#include <stdio.h>

int schedule_fcfs(SchedulerState *state) {
    int t = 0;
    int completed = 0;
    Process *running_process = NULL;

    Queue *ready_queue = &state->ready_queue;
    initialize_queue(ready_queue);

    while (completed < state->num_processes) {

        // enqueue processes that arrive at time t
        for (int i = 0; i < state->num_processes; i++) {
            if (state->processes[i].arrival_time == t) {
                enqueue(ready_queue, &state->processes[i]);
            }
        }

        // if CPU is idle, get next process
        if (running_process == NULL && !is_empty(ready_queue)) {
            running_process = dequeue(ready_queue);

            if (running_process->start_time == -1) {
                running_process->start_time = t;
            }
        }

        // run current process for 1 time unit
        if (running_process != NULL) {
            state->gantt_chart[t] = running_process - state->processes;
            running_process->remaining_time--;

            if (running_process->remaining_time == 0) {
                running_process->finish_time = t + 1;
                completed++;
                running_process = NULL;
            }
        } else {
            // CPU idle
            state->gantt_chart[t] = -1;
        }

        t++;
    }

    state->total_time = t;
    return 0;
}