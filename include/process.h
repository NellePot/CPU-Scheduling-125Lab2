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
    int in_ready_queue;     // Flag to avoid duplicates
    int turnaround_time;     // Total time from arrival to completion
    int response_time;       // Time from arrival to first execution
    int quantum_used;       //Time used in current queue time slice (for MLFQ)
    int admitted;            // Flag to track if process has been admitted to ready queue (for MLFQ)
    int started;             // Flag to track if process has started execution (for MLFQ)
} Process;
 
#endif