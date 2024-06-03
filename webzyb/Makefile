# Makefile for KISSZYB project

CC = gcc
CFLAGS = -lkisszyb

all: webzyb-client webzyb-host

webzyb-client: webzyb-client.c
	$(CC) webzyb-client.c -o webzyb-client.exe $(CFLAGS)

webzyb-host: webzyb-host.c
	$(CC) webzyb-host.c -o webzyb-host.exe $(CFLAGS)

clean:
	rm -f webzyb-client.exe webzyb-host.exe

