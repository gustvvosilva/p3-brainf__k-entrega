CC = gcc
CFLAGS = -Wall -Wextra -std=c99

ECHO = echo "CRÉDITO♥=100+7*7-4*2"

all:
	$(CC) $(CFLAGS) bfc.c -o bfc
	$(CC) $(CFLAGS) bfe.c -o bfe

bf: all
	$(ECHO) | ./bfc

run: all
	$(ECHO) | ./bfc | ./bfe

clean:
	rm -f bfc bfe
