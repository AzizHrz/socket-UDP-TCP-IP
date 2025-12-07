#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define NMAX 10
#define BUFSIZE 1024

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server-host> <server-port>\n", argv[0]);
        return 1;
    }

    const char *host = argv[1];
    const char *port = argv[2];

    /* resolve server address */
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        /* IPv4 */
    hints.ai_socktype = SOCK_DGRAM;   /* UDP */

    if (getaddrinfo(host, port, &hints, &res) != 0) {
        perror("getaddrinfo");
        return 1;
    }

    int sock = socket(res->ai_family, res->ai_socktype, 0);
    if (sock < 0) { perror("socket"); freeaddrinfo(res); return 1; }

    /* generate random number */
    srand(time(NULL) ^ getpid());
    int n = (rand() % NMAX) + 1;

    char buf[BUFSIZE];
    snprintf(buf, sizeof(buf), "%d", n);

    /* send number */
    if (sendto(sock, buf, strlen(buf)+1, 0, res->ai_addr, res->ai_addrlen) < 0) {
        perror("sendto");
        close(sock); freeaddrinfo(res); return 1;
    }

    /* receive reply */
    struct sockaddr_storage from;
    socklen_t fromlen = sizeof(from);
    ssize_t r = recvfrom(sock, buf, sizeof(buf)-1, 0, (struct sockaddr*)&from, &fromlen);
    if (r < 0) { perror("recvfrom"); close(sock); freeaddrinfo(res); return 1; }
    buf[r] = '\0';

    printf("Client sent n=%d\nServer replied: %s\n", n, buf);

    close(sock);
    freeaddrinfo(res);
    return 0;
}
