#include "../include/scheduler.h"
#include <stdio.h>

int schedule_stcf(SchedulerState *state) {
    int current_time = 0;
    int completed = 0;

    while (completed < state->num_processes) {                                        //check for new process or preemption
        int idx = get_next_process(state, current_time, compare_stcf);

        if (idx != -1) {                                                             //runs once then check again if may process na may shorter time
            Process *p = &state->processes[idx];
            if (p->start_time == -1) p->start_time = current_time;

            state->gantt_chart[current_time] = idx;
            p->remaining_time--;
            current_time++;

            if (p->remaining_time == 0) {
                p->finish_time = current_time;
                completed++;
            }
        } else {                                                                      //idle
            state->gantt_chart[current_time++] = -1;
        }
    }
    state->total_time = current_time;
    return 0;
}