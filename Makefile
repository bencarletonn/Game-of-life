# Macros
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
HEADER = gol.h
RM = rm -rf

all: gameoflife

gameoflife: gameoflife.c libgol.so
	$(CC) -g -c $(CFLAGS) gameoflife.c -I lib -o gameoflife.o
	$(CC) -g gameoflife.o -L . -l:libgol.so -o gameoflife

libgol.so: gol.c $(HEADER)
	$(CC) -fPIC -c $(CFLAGS) gol.c -o libgol.o -g
	$(CC) -shared libgol.o -o libgol.so -g
clean:
	$(RM) libgol.o libgol.so gameoflife.o gameoflife
