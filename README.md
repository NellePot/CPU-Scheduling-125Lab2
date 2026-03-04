# CPU Scheduling - CMSC 125 [Lab 2]

## Group Members
Borces, Michaela F.
Tambolero, Marinelle Joan U.

## Problem Analysis 
This project involves designing and implementing a discrete-event CPU scheduling simulator in C to analyze the behavior of fundamental scheduling algorithms: FCFS, SJF, STCF, Round Robin, and MLFQ. The simulator models process arrival and execution over simulated time, calculates key performance metrics (turnaround, waiting, and response time), and generates Gantt charts for visualization. The goal is to compare algorithm efficiency, fairness, and responsiveness while examining tradeoffs of different scheduling policies. 

**Key Challenges include:**
- **Algorithm Implementation:** Implementing multiple CPU scheduling algorithms such as FCFS, SJF, STCF, and Round Robin while ensuring each follows its specific scheduling rules.
- **MLFQ Design:** Designing and implementing a Multi-Level Feedback Queue scheduler with multiple priority levels, allotments, and priority boosts without using burst time directly.
- **Time Tracking:** Correctly tracking process arrival, execution, and completion times in the simulation.

## Solution Architecture
The simulator will simply follow a modular design.
  
**>>Input Handling**
The program reads the workload from an input file or command-line arguments and stores the processes in appropriate data structures.

**>>Scheduler**
Executes the selected algorithm (FCFS, SJF, STCF, or RR) to determine which process runs next.

Each scheduling follows the pipeline:

**Load:** Get the process information (PID, arrival time, burst time).

**Schedule:** Select the next process to run based on the selected algorithm.

**Execute:** Simulate CPU execution by updating remaining time and handling process completion or preemption.

**Record:** Track execution order to generate the Gantt chart.

**>>Output**
The program generates the Gantt chart and calculates scheduling metrics.

