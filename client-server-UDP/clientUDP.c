#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define NMAX 10        /* Maximum number of random numbers to request */
#define BUFSIZE 1024   /* Buffer size for messages */

int main(int argc, char **argv) {
    /* Validate command line arguments */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server-host> <server-port>\n", argv[0]);
        return 1;
    }

    const char *host = argv[1];
    const char *port = argv[2];

    /* Create UDP socket (SOCK_DGRAM for datagram protocol) */
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) { perror("socket"); return 1; }

    /* Initialize server address structure */
    struct sockaddr_in srv;
    memset(&srv, 0, sizeof(srv));
    srv.sin_family = AF_INET;                    /* IPv4 */
    srv.sin_port = htons(atoi(port));           /* Convert port to network byte order */
    
    /* Convert IP address string to binary form */
    if (inet_pton(AF_INET, host, &srv.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock);
        return 1;
    }

    /* Generate random number between 1 and NMAX */
    srand(time(NULL) ^ getpid());
    int n = (rand() % NMAX) + 1;

    /* Format number as string for transmission */
    char buf[BUFSIZE];
    snprintf(buf, sizeof(buf), "%d", n);

    /* Send random number to server */
    if (sendto(sock, buf, strlen(buf)+1, 0, (struct sockaddr*)&srv, sizeof(srv)) < 0) {
        perror("sendto");
        close(sock);
        return 1;
    }

    /* Receive server response with n random numbers */
    struct sockaddr_in from;
    socklen_t fromlen = sizeof(from);
    ssize_t r = recvfrom(sock, buf, sizeof(buf)-1, 0, (struct sockaddr*)&from, &fromlen);
    if (r < 0) { perror("recvfrom"); close(sock); return 1; }
    buf[r] = '\0';

    /* Display results */
    printf("Client sent n=%d\nServer replied: %s\n", n, buf);

    /* Close socket and exit */
    close(sock);
    return 0;
}
