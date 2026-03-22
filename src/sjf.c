#include "../include/scheduler.h"
#include <stdio.h>

int schedule_sjf(SchedulerState *state) {
    int current_time = 0;
    int completed = 0;

    while (completed < state->num_processes) {
        int idx = get_next_process(state, current_time, compare_sjf);     //get new process

        if (idx != -1) {                                                  //runs until matapos
            Process *p = &state->processes[idx];
            if (p->start_time == -1) p->start_time = current_time;         

            while (p->remaining_time > 0) {                              
                state->gantt_chart[current_time] = idx;
                p->remaining_time--;
                current_time++;
            }
            p->finish_time = current_time;
            completed++;
        } else {                                                         //idle
            state->gantt_chart[current_time++] = -1;
        }
    }
    state->total_time = current_time;
    return 0;
}