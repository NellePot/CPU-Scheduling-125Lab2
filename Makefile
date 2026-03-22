CC = gcc
CFLAGS = -Wall -Iinclude
SRC = src/main.c src/fcfs.c src/sjf.c src/stcf.c src/rr.c src/utils.c src/process.c src/gantt.c src/metrics.c
TARGET = schedism

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)