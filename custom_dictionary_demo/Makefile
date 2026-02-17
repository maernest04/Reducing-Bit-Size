CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: sender receiver demo

sender: send_main.o encoder.o
	$(CC) $(CFLAGS) -o sender send_main.o encoder.o

receiver: recv_main.o receiver.o encoder.o
	$(CC) $(CFLAGS) -o receiver recv_main.o receiver.o encoder.o

demo: demo.o
	$(CC) $(CFLAGS) -o demo demo.o

send_main.o: send_main.c encoder.h
encoder.o: encoder.c encoder.h
recv_main.o: recv_main.c encoder.h receiver.h
receiver.o: receiver.c receiver.h
demo.o: demo.c

clean:
	rm -f *.o sender receiver demo

.PHONY: all clean
