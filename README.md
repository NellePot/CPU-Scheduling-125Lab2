# CPU Scheduling - CMSC 125 [Lab 2]

## Group Members
Borces, Michaela F.
Tambolero, Marinelle Joan U.

**Compilation and Usage Instructions**
- Open terminal in CPU scheduling folder:
    cd /home/michaela/cpu-Scheduling-125Lab2
- Build
    make
- Run from shell (mysh)
  mysh already handles execvp for > commands, so:
    - Build
        make 
    - Start shell
       > ./mysh
    - run (FCFS example) 
       > /home/michaela/CPU-Scheduling-125Lab2/schedism --algorithm=FCFS --input=/home/michaela/CPU-Scheduling-125Lab2/src/input.txt 

## Supported algorithms / features
> --algorithm=FCFS

> --algorithm=SJF

> --algorithm=STCF

>  --algorithm=RR + --quantum=N

> --algorithm=MLFQ + --mlfq-config=cfg

**Supports input file process list:**
format per line: PID ARRIVAL BURST 
example in src/input.txt:
  - P1 0 240
  - P2 10 180
  - P3 20 150
    
**Outputs:**
- Text Gantt chart
- Metrics:
  
    -Turnaround time
  
    -Waiting time
  
    -Response time
  
    -Averages

## Example invocations 
FCFS:

    >./schedism --algorithm=FCFS --input=src/input.txt
SJF:

    >./schedism --algorithm=SJF --input=src/input.txt
STCF:

    >./schedism --algorithm=STCF --input=src/input.txt
RR, quantum 30:

    >./schedism --algorithm=RR --input=src/input.txt --quantum=30
MLFQ, config file:

    create /home/michaela/CPU-Scheduling-125Lab2/mlfq.conf:
        -BOOST_PERIOD 100
        -Q0 8 16
        -Q1 16 32
        -Q2 -1 -1(FCFS at low prio)
    run:
      ./schedism --algorithm=MLFQ --input=src/input.txt --mlfq-config=mlfq.conf 
COMPARATIVE ANALYSIS: 

    >./schedism --compare --input=tests/workload1.txt --quantum=30 --mlfq-config=src/mlfq_config.txt

## Proof of Functionality
FCFS: 
    <img width="1212" height="381" alt="fcfs" src="https://github.com/user-attachments/assets/9ce415b4-f739-4f04-89b2-d369d32d2601" />
SJF: 
    <img width="1387" height="716" alt="sjf" src="https://github.com/user-attachments/assets/e8713fd9-1a6b-46c5-a008-8072f28c05a7" />
STCF:
    <img width="1221" height="575" alt="stcf" src="https://github.com/user-attachments/assets/90ae0064-ea74-49b4-b64d-d05613621058" />
RR:
    <img width="1317" height="352" alt="rr" src="https://github.com/user-attachments/assets/407bfdc9-8a6b-484d-b3ab-6fe43119902e" />
MLFQ: 
    <img width="1644" height="846" alt="mlfq1" src="https://github.com/user-attachments/assets/1bea1c39-0b51-47fd-be92-82d67a05dfee" />
    <img width="1849" height="997" alt="mlfq2" src="https://github.com/user-attachments/assets/8fc43feb-a914-4f66-a299-d29bb7e5a841" />
COMPARATIVE ANALYSIS: 
    <img width="1837" height="265" alt="compare" src="https://github.com/user-attachments/assets/19df8609-3f05-4af0-af9b-de10598eb780" />






