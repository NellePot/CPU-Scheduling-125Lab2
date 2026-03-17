#include "../include/scheduler.h"
#include <stdlib.h>
#include <stdio.h>


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