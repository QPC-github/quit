#==========================================
#    Makefile: makefile for quit 1.0
#	Copyright 1993, 1998, 2013
#                 Toyoda Masashi 
#		  (mtoyoda@acm.org)
#	Copyright 2013
#                 Kentaro Fukuchi
#==========================================

CC=gcc
CFLAGS=-O

quit: quit.c quit.h
	$(CC) $(CFLAGS) -o quit quit.c -lncurses

clean:
	rm -f quit
