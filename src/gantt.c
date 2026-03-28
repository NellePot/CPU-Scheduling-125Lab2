#include <stdio.h>
#include <string.h>
#include "../include/gantt.h"
#include "../include/scheduler.h"

#define TIME_SCALE 20
#define MIN_DASHES 1
#define BOX_PADDING 4

void print_gantt_chart(SchedulerState *state) {
    (void)state;
    printf("\n=== Gantt Chart ===\n");

    int i = 0;
    while (i < state->total_time) {
        int pid_index = state->gantt_chart[i];
        int duration = 0;

        // Count how long this specific process ran consecutively
        while (i + duration < state->total_time && state->gantt_chart[i + duration] == pid_index) {
            duration++;
        }

        if (pid_index == -1) {
            printf("[IDLE]");
        } else {
            // Get the name from the original processes array using the index
            char *name = state->processes[pid_index].pid;
            printf("[%s", name);
            
            // Scale the dashes based on duration (1 dash per 20 units)
            int num_dashes = duration / 20;
            if (num_dashes < 1) num_dashes = 1;
            for (int j = 0; j < num_dashes; j++) printf("-");
            printf("]");
        }
        i += duration;
    }
    printf("\n");

    i = 0;
    printf("0"); // Start time
    while (i < state->total_time) {
        int pid_index = state->gantt_chart[i];
        int duration = 0;
        while (i + duration < state->total_time && state->gantt_chart[i + duration] == pid_index) {
            duration++;
        }

        // Calculate the same width used in the box above to align the numbers
        int num_dashes = duration / TIME_SCALE;
        if (num_dashes < MIN_DASHES) num_dashes = MIN_DASHES;
        
        int box_width = BOX_PADDING + num_dashes; 
        
        for (int j = 0; j < box_width - 1; j++) printf(" ");
        printf("%d", i + duration);

        i += duration;
    }
    printf("\n\n");
}