#include "../include/scheduler.h"
#include <stdio.h>

int schedule_rr(SchedulerState *state, int quantum) {
    int current_time = 0;
    int completed = 0;
    Queue *ready_queue = &state->ready_queue;
    initialize_queue(ready_queue);

    while (completed < state->num_processes) {
        update_ready_queue(state, current_time);                //check first sino yung dumating before next run 

        if (!is_empty(ready_queue)) {
            Process *p = dequeue(ready_queue);

            if (p->start_time == -1) p->start_time = current_time;

            int run_time = (p->remaining_time < quantum) ? p->remaining_time : quantum;

            int p_idx = p - state->processes;                       //maget sa index(current-base)

            for (int t = 0; t < run_time; t++) {
                state->gantt_chart[current_time] = p_idx;
                p->remaining_time--;
                current_time++;

                update_ready_queue(state, current_time);            //ginacheck niya ang arrivals during quantum
            }
                                                    
            if (p->remaining_time == 0) {
                p->finish_time = current_time;
                completed++;
            } else {
                enqueue(ready_queue, p);
            }
        } else {
            state->gantt_chart[current_time++] = -1;
        }
    }
    state->total_time = current_time;
    return 0;
}