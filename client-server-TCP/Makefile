####################################################
#   Makefile – Projet 2 : Clients/Serveur en TCP
#   Files: clientTCP.c, serveurTCP.c
####################################################

CC      = gcc
CFLAGS  = -Wall -Wextra -O2

TARGETS = clientTCP serveurTCP

all: $(TARGETS)

clientTCP: clientTCP.c
	$(CC) $(CFLAGS) -o clientTCP clientTCP.c

serveurTCP: serveurTCP.c
	$(CC) $(CFLAGS) -o serveurTCP serveurTCP.c

clean:
	rm -f $(TARGETS) *.o

re: clean all

.PHONY: all clean re
