# fork-based-server

CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGETS = serveurTCP_fork clientTCP_fork

all: $(TARGETS)

serveurTCP_fork: serveurTCP_fork.c
	$(CC) $(CFLAGS) -o serveurTCP_fork serveurTCP_fork.c tools.c

clientTCP_fork: clientTCP_fork.c
	$(CC) $(CFLAGS) -o clientTCP_fork clientTCP_fork.c

clean:
	rm -f $(TARGETS)

re: clean all

.PHONY: all clean re	