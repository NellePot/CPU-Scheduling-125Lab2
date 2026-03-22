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