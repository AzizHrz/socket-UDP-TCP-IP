####################################################
#   Makefile – Projet 1 : Client/Serveur en UDP
#   Files: clientUDP.c, serveurUDP.c
####################################################

CC      = gcc
CFLAGS  = -Wall -Wextra -O2

TARGETS = clientUDP serveurUDP

all: $(TARGETS)

clientUDP: clientUDP.c
	$(CC) $(CFLAGS) -o clientUDP clientUDP.c

serveurUDP: serveurUDP.c
	$(CC) $(CFLAGS) -o serveurUDP serveurUDP.c

clean:
	rm -f $(TARGETS) *.o

re: clean all

.PHONY: all clean re
