#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/scheduler.h"

int main(int argc, char *argv[]) {
    char algorithm[10] = "";
    char input_file[100] = "";

    // Parse algorithm
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--algorithm=", 12) == 0) {
            strcpy(algorithm, argv[i] + 12);
        } else if (strncmp(argv[i], "--input=", 8) == 0) {
            strcpy(input_file, argv[i] + 8);
        }
    }

    if (strlen(algorithm) == 0) {
        printf("Please specify --algorithm=FCFS | SJF | STCF\n");
        return 1;
    }

    if (strlen(input_file) == 0) {
        printf("Please specify --input=filename\n");
        return 1;
    }

    FILE *fp = fopen(input_file, "r");
    if (fp == NULL) {
        printf("Error opening input file: %s\n", input_file);
        return 1;
    }

    // Count number of processes
    int n = 0;
    char line[100];
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strlen(line) > 1) {  
            n++;
        }
    }
    rewind(fp);

    // Initialize state
    SchedulerState state;
    state.num_processes = n;
    state.processes = malloc(sizeof(Process) * n);
    state.gantt_chart = malloc(sizeof(int) * 1000);
    state.current_time = 0;
    state.total_time = 0;
    initialize_queue(&state.ready_queue);

    if (state.processes == NULL || state.gantt_chart == NULL) {
        printf("Memory allocation failed\n");
        fclose(fp);
        return 1;
    }

    // Read processes from file
    for (int i = 0; i < n; i++) {
        fscanf(fp, "%s %d %d",
               state.processes[i].pid,
               &state.processes[i].arrival_time,
               &state.processes[i].burst_time);

        state.processes[i].remaining_time = state.processes[i].burst_time;
        state.processes[i].start_time = -1;
        state.processes[i].finish_time = -1;
    }

    fclose(fp);

    // Select algorithm
    if (strcmp(algorithm, "FCFS") == 0) {
        schedule_fcfs(&state);
    } else if (strcmp(algorithm, "SJF") == 0) {
        schedule_sjf(&state);
    } else if (strcmp(algorithm, "STCF") == 0) {
        schedule_stcf(&state);
    } else {
        printf("Unknown algorithm!\n");
        free(state.processes);
        free(state.gantt_chart);
        return 1;
    }

    // Output results
    print_gantt_chart(&state);
    calculate_and_print_metrics(&state);

    // Free memory
    free(state.processes);
    free(state.gantt_chart);

    return 0;
}