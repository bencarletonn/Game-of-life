all: gameoflife
gameoflife: gameoflife.c
	gcc -Wall -Wextra -pedantic -std=c11 gameoflife.c gol.c -o gameoflife
clean:
	rm gameoflife

