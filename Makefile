CC = gcc
CFLAGS = -Wall -Wextra -std=c99

ECHO = echo "fabricio=10*3+9"

all: compilador executor

compilador: compilador.c
	$(CC) $(CFLAGS) -o compilador compilador.c

executor: executor.c
	$(CC) $(CFLAGS) -o executor executor.c

bf: all
	$(ECHO) | ./compilador

run: all
	$(ECHO) | ./compilador | ./executor

clean:
	rm -f compilador executor