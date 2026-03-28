#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../include/scheduler.h" 
#include "../include/gantt.h" 
#include "../include/metrics.h" 


int compare_mode = 0;  //global flag for comparison mode 

int main(int argc, char *argv[]) {
    char algorithm[20] = "";
    char input_file[100] = "";
    char mlfq_config_file[100] = "";
    int quantum = 30; // default
    char *endptr;
    long q;

    // Parse algorithm
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--algorithm=", 12) == 0) {
           snprintf(algorithm, sizeof(algorithm), "%s", argv[i] + 12); //fixed
        } else if (strncmp(argv[i], "--input=", 8) == 0) {
           snprintf(input_file, sizeof(input_file), "%s", argv[i] + 8); //fixed
        } else if (strncmp(argv[i], "--quantum=", 10) == 0) {           //fixed
            errno = 0;
            q = strtol(argv[i] + 10, &endptr, 10);
            if (errno != 0 || *endptr != '\0' || q <= 0) {
                printf("Invalid quantum value\n");
                return 1;
            }
            quantum = (int)q;
        } else if (strncmp(argv[i], "--mlfq-config=", 14) == 0) {
            snprintf(mlfq_config_file, sizeof(mlfq_config_file), "%s", argv[i] + 14); 
        } else if (strcmp (argv[i], "--compare") == 0) {
            compare_mode=1; 
        }
    }

    if (strlen(algorithm) == 0 && compare_mode == 0) {
        printf("Please specify --algorithm=FCFS | SJF | STCF | RR | MLFQ\n");
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

    if (n <= 0) {
        printf("No processes to schedule\n");
        fclose(fp);
        return 1;
    }

    // Initialize state
    SchedulerState state;
    state.num_processes = n;
    state.processes = malloc(sizeof(Process) * n);                          
    state.current_time = 0;
    state.total_time = 0;
    initialize_queue(&state.ready_queue);
    
    if (state.processes == NULL) {
        printf("Memory allocation failed\n");
        fclose(fp);
        return 1;
    }

    memset(state.processes, 0, sizeof(Process) * n); // Initialize process array to zero

    char line_buffer[100];
    int i = 0;

    while(i < n && fgets(line_buffer, sizeof(line_buffer), fp) != NULL) {
        if (line_buffer[0] == '\n' || line_buffer[0] == '\0') {
            continue;
        }

        if(sscanf(line_buffer, "%15s %d %d",                                    //fixed
                state.processes[i].pid,
                &state.processes[i].arrival_time,
                &state.processes[i].burst_time) != 3) {
            printf("Error reading process data from file\n");
            free(state.processes);
            fclose(fp);
            return 1;
        }

            state.processes[i].remaining_time = state.processes[i].burst_time;
            state.processes[i].start_time = -1;
            state.processes[i].finish_time = -1;
            state.processes[i].waiting_time = 0;
            state.processes[i].turnaround_time = 0;
            state.processes[i].response_time = -1;
            state.processes[i].in_ready_queue = 0;
             
            //MLFQ-related initializations
            state.processes[i].priority = 0;
            state.processes[i].time_in_queue = 0;
            state.processes[i].quantum_used = 0;
            state.processes[i].admitted = 0;
            state.processes[i].started = 0;

            i++;
        }

        if (i != n) {
            printf("Unexpected number of valid process lines\n");
            destroy_queue(&state.ready_queue);
            free(state.processes);
            fclose(fp);
            return 1;
    }

    Process *original_processes = malloc(sizeof(Process) * n);
    memcpy(original_processes, state.processes, sizeof(Process) * n);

    int total_burst = 0;
    int max_arrival = 0;

    for (int i = 0; i < n; i++) {
        total_burst += state.processes[i].burst_time;
        if (state.processes[i].arrival_time > max_arrival) {
            max_arrival = state.processes[i].arrival_time;
        }
    }

    state.gantt_chart = malloc(sizeof(int) * (total_burst + max_arrival + 1));

    if (state.gantt_chart == NULL) {
        printf("Memory allocation failed\n");
        destroy_queue(&state.ready_queue);
        free(state.processes);
        fclose(fp);
        return 1;
    }

    fclose(fp);

    if(compare_mode){
       printf("\n=== Algorithm Comparison for %s ===\n", input_file);
        printf("%-10s | %6s | %6s | %6s | %6s\n",
            "Algorithm", "Avg TT", "Avg WT", "Avg RT", "Ctx Sw");
         printf("-----------|--------|--------|--------|--------\n");
    
    // FCFS
    memcpy(state.processes, original_processes, sizeof(Process) * n);
    reset_state(&state);
    schedule_fcfs(&state);
    calculate_metrics(state.processes, state.num_processes);
    print_comparison_row("FCFS", &state);


    // SJF
    memcpy(state.processes, original_processes, sizeof(Process) * n);
    reset_state(&state);
    schedule_sjf(&state);
    calculate_metrics(state.processes, state.num_processes);
    print_comparison_row("SJF", &state);


    // STCF
    memcpy(state.processes, original_processes, sizeof(Process) * n);
    reset_state(&state);
    schedule_stcf(&state);
    calculate_metrics(state.processes, state.num_processes);
    print_comparison_row("STCF", &state);


    // RR
    memcpy(state.processes, original_processes, sizeof(Process) * n);
    reset_state(&state);;
    schedule_rr(&state, quantum);
    calculate_metrics(state.processes, state.num_processes);
    print_comparison_row("RR", &state); 
    /*printf("%-10s | ", "RR");
    double avg_tt = 0, avg_wt = 0, avg_rt = 0;
    for (int i = 0; i < state.num_processes; i++) {
        avg_tt += state.processes[i].turnaround_time;
        avg_wt += state.processes[i].waiting_time;
        avg_rt += state.processes[i].response_time;
    }
    avg_tt /= state.num_processes;
    avg_wt /= state.num_processes;
    avg_rt /= state.num_processes;
    printf("%6.1f | %6.1f | %6.1f\n", avg_tt, avg_wt, avg_rt);*/ 


    // MLFQ
    memcpy(state.processes, original_processes, sizeof(Process) * n); 
    reset_state(&state);
    if (schedule_mlfq(&state, mlfq_config_file) != 0) {
        printf("%-10s | ERROR\n", "MLFQ");
    } else {
        calculate_metrics(state.processes, state.num_processes);
        print_comparison_row("MLFQ", &state);
    } 
} else {
    //Select algo 
    if (strcmp(algorithm, "FCFS") == 0) {
        schedule_fcfs(&state);
    } else if (strcmp(algorithm, "SJF") == 0) {
        schedule_sjf(&state);
    } else if (strcmp(algorithm, "STCF") == 0) {
        schedule_stcf(&state);
    } else if (strcmp(algorithm, "RR") == 0) {
        if (quantum <= 0) {
            printf("Please specify a valid quantum for RR\n");
            destroy_queue(&state.ready_queue);
            free(state.processes);
            free(state.gantt_chart);
            return 1;
        }
        schedule_rr(&state, quantum);
    } else if (strcmp(algorithm, "MLFQ") == 0) {
        if (strlen(mlfq_config_file) == 0) {
            printf("Please specify --mlfq-config=filename\n");
            destroy_queue(&state.ready_queue);
            free(state.processes);
            free(state.gantt_chart);
            return 1;
        }

        if (schedule_mlfq(&state, mlfq_config_file) != 0) {
            destroy_queue(&state.ready_queue);
            free(state.processes);
            free(state.gantt_chart);
            return 1;
        }
    } else {
        printf("Unknown algorithm!\n");
        destroy_queue(&state.ready_queue);
        free(state.processes);
        free(state.gantt_chart);
        return 1;
    }
}
// Output results
if(!compare_mode){
    print_gantt_chart(&state);
    calculate_and_print_metrics(&state); 
} 

// Free memory
destroy_queue(&state.ready_queue);
free(state.processes);
free(state.gantt_chart);

return 0;
}