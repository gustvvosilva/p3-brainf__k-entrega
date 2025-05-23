CC = gcc
CFLAGS = -Wall -Wextra -std=c99

ECHO = echo "CRÉDITO♥=100+7*7-4*2"

all: bfc bfe

compilador: compilador.c
	$(CC) $(CFLAGS) -o bfc bfc.c

executor: executor.c
	$(CC) $(CFLAGS) -o bfe bfe.c

bf: all
	$(ECHO) | ./bfc

run: all
	$(ECHO) | ./bfc | ./bfe

clean:
	rm -f bfc bfe
