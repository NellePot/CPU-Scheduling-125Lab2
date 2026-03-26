#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/scheduler.h"

/* =========================
   MLFQ QUEUE FUNCTIONS
   ========================= */

// Bug 1 fixed: restored original signature for initialize_mlfq_queue
void initialize_mlfq_queue(MLFQQueue *q, int level, int quantum, int allotment, int capacity) {
    q->level = level;
    q->time_quantum = quantum;
    q->allotment = allotment;
    q->capacity = capacity;
    q->size = 0;
    q->front = 0;
    q->rear = -1;
    q->queue = malloc(sizeof(Process *) * capacity);
}

void enqueue_mlfq(MLFQQueue *q, Process *p) {
    if (q->size == q->capacity) return;

    q->rear = (q->rear + 1) % q->capacity;
    q->queue[q->rear] = p;
    q->size++;
}

Process *dequeue_mlfq(MLFQQueue *q) {
    if (q->size == 0) return NULL;

    Process *p = q->queue[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return p;
}

int is_empty_mlfq(MLFQQueue *q) {
    return q->size == 0;
}

void destroy_mlfq_queue(MLFQQueue *q) {
    free(q->queue);
    q->queue = NULL;
}

/* =========================
   MLFQ SCHEDULER SETUP
   ========================= */

// Bug 2 fixed: now accepts quantums[] and allotments[] arrays instead of hardcoding
void initialize_mlfq_scheduler(MLFQScheduler *mlfq, int num_queues, int boost_period, int capacity, int *quantums, int *allotments) {
    mlfq->num_queues = num_queues;
    mlfq->boost_period = boost_period;
    mlfq->last_boost = 0;
    mlfq->queues = malloc(sizeof(MLFQQueue) * num_queues);

    for (int i = 0; i < num_queues; i++) {
        initialize_mlfq_queue(&mlfq->queues[i], i, quantums[i], allotments[i], capacity);
    }
}

void destroy_mlfq_scheduler(MLFQScheduler *mlfq) {
    for (int i = 0; i < mlfq->num_queues; i++) {
        destroy_mlfq_queue(&mlfq->queues[i]);
    }
    free(mlfq->queues);
    mlfq->queues = NULL;
}

/* =========================
   HELPER FUNCTIONS
   ========================= */

void add_new_arrivals_mlfq(SchedulerState *state, MLFQScheduler *mlfq) {
    for (int i = 0; i < state->num_processes; i++) {
        Process *p = &state->processes[i];

        if (p->arrival_time <= state->current_time &&
            p->remaining_time > 0 &&
            p->admitted == 0) {

            p->priority = 0;
            p->time_in_queue = 0;
            p->quantum_used = 0;
            p->admitted = 1;

            enqueue_mlfq(&mlfq->queues[0], p);
        }
    }
}

Process *get_next_mlfq_process(MLFQScheduler *mlfq) {
    for (int i = 0; i < mlfq->num_queues; i++) {
        if (!is_empty_mlfq(&mlfq->queues[i])) {
            return dequeue_mlfq(&mlfq->queues[i]);
        }
    }
    return NULL;
}

int higher_priority_queue_has_job(MLFQScheduler *mlfq, int current_priority) {
    for (int i = 0; i < current_priority; i++) {
        if (!is_empty_mlfq(&mlfq->queues[i])) {
            return 1;
        }
    }
    return 0;
}

void priority_boost_mlfq(MLFQScheduler *mlfq, int current_time) {
    // matches the manual's condition exactly
    if (current_time - mlfq->last_boost >= mlfq->boost_period) {
        for (int i = 1; i < mlfq->num_queues; i++) {
            while (!is_empty_mlfq(&mlfq->queues[i])) {
                Process *p = dequeue_mlfq(&mlfq->queues[i]);
                p->priority = 0;
                p->time_in_queue = 0;
                p->quantum_used = 0;
                enqueue_mlfq(&mlfq->queues[0], p);
            }
        }
        mlfq->last_boost = current_time;
    }
}

void mlfq_adjust_priority(MLFQScheduler *mlfq, Process *p) {
    MLFQQueue *current_queue = &mlfq->queues[p->priority];

    if (current_queue->allotment != -1 &&
        p->time_in_queue >= current_queue->allotment) {

        if (p->priority < mlfq->num_queues - 1) {
            p->priority++;
            p->time_in_queue = 0;
            p->quantum_used = 0;
            enqueue_mlfq(&mlfq->queues[p->priority], p);
        }
    }
}

/* =========================
   MAIN MLFQ SCHEDULER
   ========================= */

int schedule_mlfq(SchedulerState *state, const char *config_file) {
    Process *running_process = NULL;
    MLFQScheduler mlfq;
    int completed = 0;

    // --- Read config file ---
    int num_queues = 3;
    int boost_period = 100;
    int quantums[10];
    int allotments[10];

    // Bug 3 fixed: consistent variable name config_fp throughout
    FILE *config_fp = fopen(config_file, "r");
    if (config_fp == NULL) {
        printf("Error opening MLFQ config file: %s\n", config_file);
        return -1;
    }

    char line[100];
    int q_index = 0;  // Bug 3 fixed: was wrongly named 'q' before

    while (fgets(line, sizeof(line), config_fp)) {
        if (line[0] == '#' || line[0] == '\n') continue;

        if (strncmp(line, "BOOST_PERIOD", 12) == 0) {
            sscanf(line, "BOOST_PERIOD %d", &boost_period);
        } else {
            int level, quantum, allotment;
            if (sscanf(line, "Q%d %d %d", &level, &quantum, &allotment) == 3) {
                quantums[level] = quantum;
                allotments[level] = allotment;
                q_index++;  // Bug 3 fixed: was 'q++' before
            }
        }
    }
    num_queues = q_index;   // Bug 3 fixed: was 'q' before
    fclose(config_fp);      // Bug 3 fixed: was 'fp' before

    // Bug 2 fixed: now passes quantums[] and allotments[] from config file
    initialize_mlfq_scheduler(&mlfq, num_queues, boost_period,
                               state->num_processes, quantums, allotments);

    // Reset all process state
    for (int i = 0; i < state->num_processes; i++) {
        state->processes[i].remaining_time = state->processes[i].burst_time;
        state->processes[i].start_time = -1;
        state->processes[i].finish_time = -1;
        state->processes[i].waiting_time = 0;
        state->processes[i].turnaround_time = 0;
        state->processes[i].response_time = -1;
        state->processes[i].priority = 0;
        state->processes[i].time_in_queue = 0;
        state->processes[i].quantum_used = 0;
        state->processes[i].admitted = 0;
        state->processes[i].started = 0;
    }

    state->current_time = 0;
    state->total_time = 0;
    mlfq.last_boost = 0;

    while (completed < state->num_processes) {
        add_new_arrivals_mlfq(state, &mlfq);
        priority_boost_mlfq(&mlfq, state->current_time);

        // also boost the currently running process if needed
        if (running_process != NULL && running_process->priority > 0) {
            running_process->priority = 0;
            running_process->time_in_queue = 0;
            running_process->quantum_used = 0;
        }

        if (running_process == NULL) {
            running_process = get_next_mlfq_process(&mlfq);
        }

        // CPU idle
        if (running_process == NULL) {
            state->gantt_chart[state->current_time] = -1;
            state->current_time++;
            state->total_time++;
            continue;
        }

        Process *p = running_process;

        // Bug 4 fixed: current_queue declared only once, after priority is finalized
        MLFQQueue *current_queue = &mlfq.queues[p->priority];

        int index = (int)(p - state->processes);

        // Record response time on first execution
        if (p->started == 0) {
            p->start_time = state->current_time;
            p->response_time = p->start_time - p->arrival_time;
            p->started = 1;
        }

        state->gantt_chart[state->current_time] = index;

        p->remaining_time--;
        p->time_in_queue++;
        p->quantum_used++;

        state->current_time++;
        state->total_time++;

        add_new_arrivals_mlfq(state, &mlfq);

        // Process finished
        if (p->remaining_time == 0) {
            p->finish_time = state->current_time;
            p->turnaround_time = p->finish_time - p->arrival_time;
            p->waiting_time = p->turnaround_time - p->burst_time;
            completed++;
            running_process = NULL;
            continue;
        }

        // Allotment exhausted → demote
        if (current_queue->allotment != -1 &&
            p->time_in_queue >= current_queue->allotment) {

            mlfq_adjust_priority(&mlfq, p);
            running_process = NULL;
        }
        // Higher priority job arrived → preempt
        else if (higher_priority_queue_has_job(&mlfq, p->priority)) {
            enqueue_mlfq(&mlfq.queues[p->priority], p);
            running_process = NULL;
        }
        // Quantum exhausted → requeue at same level
        else if (current_queue->time_quantum != -1 &&
                 p->quantum_used >= current_queue->time_quantum) {

            p->quantum_used = 0;
            enqueue_mlfq(&mlfq.queues[p->priority], p);
            running_process = NULL;
        }
        // Keep running
        else {
            running_process = p;
        }
    }

    destroy_mlfq_scheduler(&mlfq);
    return 0;
}