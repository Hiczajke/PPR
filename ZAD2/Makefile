CFLAGS = -O2 -Wall
LDFLAGS = -s
CC = gcc
LD = ld
MAKE = make
MAKEARGS = 'SHELL=/bin/sh'
SHELL = /bin/sh

all: server client

server: server.c
	$(CC) $(CFLAGS) -c $<
	$(CC) $(LDFLAGS) $< -o $@

client: client.c
	$(CC) $(CFLAGS) -c $<
	$(CC) $(LDFLAGS) $< -o $@


clean:
	rm -f client server *.o 

