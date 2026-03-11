CC = gcc
CFLAGS = -Wall -Iinclude
SRC = src/main.c src/fcfs.c src/utils.c src/gantt.c src/metrics.c
TARGET = schedsim

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)