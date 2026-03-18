#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    char pid[16];           // Process identifier
    int arrival_time;       // When process arrives
    int burst_time;         // Total CPU time needed
    int remaining_time;     // For preemptive algorithms
    int start_time;         // When first executed (for RT)
    int finish_time;        // When completed (for TT)
    int waiting_time;       // Time spent waiting
    int priority;           // For MLFQ
    int time_in_queue;      // For MLFQ allotment tracking
    int turnaround_time;     // Total time from arrival to completion
    int response_time;       // Time from arrival to first execution
    int in_ready_queue;     // Flag for RR to avoid duplicates
} Process;
 
#endif