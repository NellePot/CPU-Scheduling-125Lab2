#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h" 

typedef struct Node {
    Process *process;
    struct Node *next;
} Node;

typedef struct {
    Node *front; 
    Node *rear;
} Queue;

typedef struct {
    Process *processes;     
    int num_processes;      
    int current_time;       
    int *gantt_chart;
    int total_time;  
    Queue ready_queue;     
} SchedulerState; 

typedef struct {
    int level;              // Queue priority level (0 = highest)
    int time_quantum;       // Time slice for this queue (-1 for FCFS)
    int allotment;          // Max time before demotion (-1 for infinite)
    Process *queue;         // Array or linked list of processes
    int size;               // Current queue size
} MLFQQueue;

typedef struct {
    MLFQQueue *queues;      // Array of queues
    int num_queues;         // Number of priority levels
    int boost_period;       // Period for priority boost (S)
    int last_boost;         // Last boost time
} MLFQScheduler;

typedef int (*CompareFunc)(Process*,Process*); 

void initialize_queue(Queue *q);
void enqueue(Queue *q, Process *p);
void destroy_queue(Queue *q);
Process* dequeue(Queue *q);
int is_empty(Queue *q);
Process* peek(Queue *q);   //fixed


int get_next_process(SchedulerState *state, int current_time, CompareFunc compare);
int compare_fcfs(Process *a, Process *b); 
int compare_sjf(Process *a, Process *b); 
int compare_stcf(Process *a, Process *b); 
void update_ready_queue(SchedulerState *state, int time, Process *running_process); 

int schedule_fcfs(SchedulerState *state);
int schedule_sjf(SchedulerState *state);
int schedule_stcf(SchedulerState *state);
int schedule_rr(SchedulerState *state, int quantum);
int schedule_mlfq(SchedulerState *state, MLFQScheduler *config); 
void print_gantt_chart(SchedulerState *state);
void calculate_metrics(Process *processes, int n);
double calculate_average_turnaround(Process *processes, int n);
double calculate_average_waiting(Process *processes, int n);
double calculate_average_response(Process *processes, int n);
void calculate_and_print_metrics(SchedulerState *state);

#endif