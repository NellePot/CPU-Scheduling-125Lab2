#include "../include/scheduler.h"
#include <stdio.h>

extern int compare_mode;

int schedule_stcf(SchedulerState *state) {
    int current_time = 0;
    int completed = 0;
    int last_idx = -1; //for preemption logging

    while (completed < state->num_processes) {                                        //check for new process or preemption
        int idx = get_next_process(state, current_time, compare_stcf);

        if (idx != -1) {                                                             //runs once then check again if may process na may shorter time
            Process *p = &state->processes[idx];

            if (last_idx != -1 && last_idx != idx) {
                state->context_switches++; 
                if (!compare_mode){ // Preemption occurred
                    printf("Process %s was preempted at t=%d (remaining: %d)\n",
                        state->processes[last_idx].pid, current_time, 
                        state->processes[last_idx].remaining_time);
                    printf("Process %s resumed at t=%d (remaining: %d)\n",
                        p->pid, current_time, p->remaining_time);
                }
            }
            if (p->start_time == -1) {
                p->start_time = current_time;

                if(!compare_mode){
                    printf("Process %s started at t=%d (burst: %d)\n", p->pid, current_time, p->burst_time);
                } 
            }

            state->gantt_chart[current_time] = idx;
            p->remaining_time--;
            current_time++;
            last_idx = idx;

            if (p->remaining_time == 0) {
                p->finish_time = current_time;
                completed++;
            }


        } else {                                                                      //idle
            state->gantt_chart[current_time++] = -1;
            last_idx = -1; // Reset last_idx during idle to avoid incorrect preemption logs when a process starts after idle
        }
    }
    state->total_time = current_time;
    return 0;
}