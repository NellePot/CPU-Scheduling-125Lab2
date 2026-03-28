#include "../include/scheduler.h"
#include <stdio.h>

extern int compare_mode; 

int schedule_sjf(SchedulerState *state) {
    int current_time = 0;
    int completed = 0;

    while (completed < state->num_processes) {
        int idx = get_next_process(state, current_time, compare_sjf);     //get new process

        if (idx != -1) {                                                  //runs until matapos
            Process *p = &state->processes[idx];
            if (p->start_time == -1) p->start_time = current_time;   
            
            int wait_before_start = current_time - p->arrival_time;
            if (wait_before_start > 0) {
                if (!compare_mode){
                    printf("Process %s waited %d time units before starting\n", p->pid, wait_before_start);
                }
            }

            while (p->remaining_time > 0) {                              
                state->gantt_chart[current_time] = idx;
                p->remaining_time--;
                current_time++;
            }
            p->finish_time = current_time;

            int waiting = p->finish_time - p->arrival_time - p->burst_time;
             if (waiting > 300) {
                if (!compare_mode){ // arbitrary threshold for starvation
                    printf("Starvation detected: Process %s waited %d time units\n",
                        p->pid, waiting);
                }
            }

            completed++;
        } else {                                                         //idle
            state->gantt_chart[current_time++] = -1;
        }
    }
    state->total_time = current_time;
    return 0;
}