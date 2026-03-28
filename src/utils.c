#include "../include/scheduler.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 


void initialize_queue(Queue *q) {                       // himoang empty queue
    q->front = NULL;
    q->rear = NULL;
}

void enqueue(Queue *q, Process *p) {                    // dagdag ug process sa back sang queue
    Node *new_node = malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    new_node->process = p;
    new_node->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = new_node;                  // If queue is empty, both front and rear kay new node 
        return;
    }

    q->rear->next = new_node;                           // Add ang bag-ong node sa end and update rear
    q->rear = new_node;
}

Process* dequeue(Queue *q) {                            // Eh return mo ang process na nasa front sang queue kag remove siya sa queue 
    if (q->front == NULL) {
        return NULL;
    }

    Node *temp = q->front;
    Process *p = temp->process;

    q->front = q->front->next;

    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp);
    return p;
}

int is_empty(Queue *q) {
    return (q->front == NULL);
}

Process* peek(Queue *q) {
    if (q->front == NULL) return NULL;
    return q->front->process;
}

void print_queue(Queue *q) {
    Node *temp = q->front;

    printf("Queue: ");
    while (temp != NULL) {
        printf("%s ", temp->process->pid);
        temp = temp->next;
    }
    printf("\n");
}

void destroy_queue(Queue *q) {
    Node *current = q->front;

    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }

    q->front = NULL;
    q->rear = NULL;
}

int get_next_process(SchedulerState *state, int current_time, CompareFunc compare) {
    int idx = -1;
    for (int i = 0; i < state->num_processes; i++) {
        Process *p = &state->processes[i];
        if (p->arrival_time <= current_time && p->remaining_time > 0) {             //consider lang ang nagarrive pero wala pa ka finish 
            if (idx == -1 || compare(p, &state->processes[idx]) < 0) {              
                idx = i;
            }
        }
    }
    return idx;
}

int compare_fcfs(Process *a, Process *b) {                  //Pick the one that arrived first
    return a->arrival_time - b->arrival_time;
}

int compare_sjf(Process *a, Process *b) {                   //Pick the shortest total burst
    if (a->burst_time != b->burst_time)
        return a->burst_time - b->burst_time;
    return a->arrival_time - b->arrival_time; 
}

int compare_stcf(Process *a, Process *b) {                  // Pick the shortest remaining time
    if (a->remaining_time != b->remaining_time)
        return a->remaining_time - b->remaining_time;
    return a->arrival_time - b->arrival_time; 
}

void update_ready_queue(SchedulerState *state, int time, Process *running_process) {     //Check for new arrivals and add to ready queue
    for (int i = 0; i < state->num_processes; i++) {
        Process *p = &state->processes[i];

        if(p==running_process) continue;                                                 // skip the currently running process (fix)

        if (p->arrival_time <= time && p->remaining_time > 0 && !p->in_ready_queue) {
            enqueue(&state->ready_queue, p);
            p->in_ready_queue = 1;
        }
    }
} 

//helper function 
void reset_state(SchedulerState *state) {
    state->current_time = 0;
    state->total_time = 0;
    state->context_switches = 0;
    for (int i = 0; i < state->num_processes; i++) {
        state->processes[i].remaining_time  = state->processes[i].burst_time;
        state->processes[i].start_time      = -1;
        state->processes[i].finish_time     = -1;
        state->processes[i].waiting_time    = 0;
        state->processes[i].turnaround_time = 0;
        state->processes[i].response_time   = -1;
        state->processes[i].in_ready_queue  = 0;
        state->processes[i].priority        = 0;
        state->processes[i].time_in_queue   = 0;
        state->processes[i].quantum_used    = 0;
        state->processes[i].admitted        = 0;
        state->processes[i].started         = 0;
    }
    initialize_queue(&state->ready_queue);
}
 
void print_comparison_row(const char *name, SchedulerState *state) {
    double avg_tt = 0, avg_wt = 0, avg_rt = 0;
    for (int i = 0; i < state->num_processes; i++) {
        avg_tt += state->processes[i].turnaround_time;
        avg_wt += state->processes[i].waiting_time;
        avg_rt += state->processes[i].response_time;
    }
    avg_tt /= state->num_processes;
    avg_wt /= state->num_processes;
    avg_rt /= state->num_processes;
    printf("%-10s | %6.1f | %6.1f | %6.1f | %6d\n",
           name, avg_tt, avg_wt, avg_rt, state->context_switches);
}

