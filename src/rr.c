#include "../include/scheduler.h"
#include <stdio.h>

extern int compare_mode;

int schedule_rr(SchedulerState *state, int quantum) {
    state->current_time = 0;
    int completed = 0;
    int context_switch = 0;
    Queue *ready_queue = &state->ready_queue;
    initialize_queue(ready_queue);

    while (completed < state->num_processes) {
        update_ready_queue(state, state->current_time, NULL);                //check first sino yung dumating before next run 

        if (!is_empty(&state->ready_queue)) {
            Process *p = dequeue(&state->ready_queue);

            p->in_ready_queue = 0;                              //clears queue since na-dequeue na siya (fix)

            if (p->start_time == -1) p->start_time = state->current_time;

            int run_time = (p->remaining_time < quantum) ? p->remaining_time : quantum;
            int p_idx = p - state->processes;                       //maget sa index(current-base)

            for (int t = 0; t < run_time; t++) {
                state->gantt_chart[state->current_time] = p_idx;
                p->remaining_time--;
                state->current_time++;

                update_ready_queue(state, state->current_time, p);            //ginacheck niya ang arrivals during quantum
            }
                                                    
            if (p->remaining_time == 0) {
                p->finish_time = state->current_time;
                completed++;
            } else {
                context_switch++;
                enqueue(&state->ready_queue, p);
                p->in_ready_queue = 1;                          //process is enqueued (fix)
            }
        } else {
            state->gantt_chart[state->current_time] = -1;
            state->current_time++;
        }
    }
    if(!compare_mode){
        printf("Total context switches: %d\n", context_switch);
    }
    state->total_time = state->current_time;
    return 0;
}