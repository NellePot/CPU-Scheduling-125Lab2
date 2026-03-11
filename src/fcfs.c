#include "scheduler.h"
#include <stdio.h>
#include <string.h>

int schedule_fcfs(SchedulerState *state) {
    int t = 0;
    int completed = 0;
    Process *running_process = NULL;
    
    Queue ready_queue;
    initialize_queue(&ready_queue);

    while (completed < state->num_processes) {
        
        for (int i = 0; i < state->num_processes; i++) {                   //Check arrival_time sang tanan processes, i-enqueue siya sa ready_queue
            if (state->processes[i].arrival_time == t) {
                enqueue(&ready_queue, &state->processes[i]);
            }
        }

        if (running_process == NULL && !is_empty(&ready_queue)) {         //next process if free ang CPU
            running_process = dequeue(&ready_queue);
            
            running_process->start_time = t;                              //record start time para sa response time calculation
        }

        if (running_process != NULL) {
            state->gantt_chart[t] = running_process->arrival_time;       // Temporary placeholder
            
            running_process->remaining_time--;

            if (running_process->remaining_time == 0) {
                running_process->finish_time = t + 1;
                completed++;
                running_process = NULL; 
            }
        } else {
            state->gantt_chart[t] = -1; 
        }

        t++;
        if (t > 10000) break; 
    }

    state->total_time = t;
    return 0;
}