# Monoclient/Monoserveur TCP

CC      = gcc
CFLAGS  = -Wall -Wextra -O2

TARGETS = clientTCP serveurTCP

all: $(TARGETS)

serveurTCP: serveurTCP.c
	$(CC) $(CFLAGS) -o serveurTCP serveurTCP.c

clientTCP: clientTCP.c
	$(CC) $(CFLAGS) -o clientTCP clientTCP.c

clean:
	rm -f $(TARGETS)

re: clean all

.PHONY: all clean re
