We used 4 priority levels because the processes have very different lengths (A:240, B:180, C:150, D:80, E:130). Short jobs like D (80) finish quickly in higher queues, while medium jobs like E and C (130–150) take a bit longer. Long jobs like B and A (180–240) move down to lower queues. If we only used 3 queues, B and A would be grouped together too early, making scheduling less fair.

The time quanta (10, 30, 60) increase as we go down the queues. The smallest quantum (10) helps short jobs finish quickly. Larger quanta in lower queues reduce context switching for longer jobs. The allotments (50, 150, 300) give each process 5 chances per queue (since 50/10 = 5, 150/30 = 5, 300/60 = 5). This helps separate medium jobs from very long ones before moving them down.

A boost period of 150 was chosen after testing:
- boost=200 -> Avg TT=645 (A and B starved in lower queues)
- boost=150 -> Avg TT=631 (best balance, RT=13 maintained)

Overall, MLFQ gave the best response time (13), better than STCF (15) and RR (43), while keeping turnaround time similar to RR, and it did this without needing to know burst times in advance