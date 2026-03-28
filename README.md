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


