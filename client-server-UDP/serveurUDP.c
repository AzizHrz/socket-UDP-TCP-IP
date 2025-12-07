/* serveurUDP.c
   UDP Server: listens on a port and responds to client requests
   Usage: ./serveurUDP <port>
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSZ 2048   /* Buffer size for messages */

int main(int argc, char **argv) {
    /* Validate command line arguments */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1;
    }
    
    /* Parse and validate port number */
    int port = atoi(argv[1]);
    if (port <= 0) { fprintf(stderr, "Bad port\n"); return 1; }

    /* Create UDP socket (SOCK_DGRAM for datagram protocol) */
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) { perror("socket"); return 1; }

    /* Initialize server address structure */
    struct sockaddr_in srv;
    memset(&srv, 0, sizeof(srv));
    srv.sin_family = AF_INET;               /* IPv4 */
    srv.sin_addr.s_addr = INADDR_ANY;      /* Listen on all available interfaces */
    srv.sin_port = htons(port);            /* Convert port to network byte order */

    /* Bind socket to address and port */
    if (bind(sock, (struct sockaddr*)&srv, sizeof(srv)) < 0) {
        perror("bind"); close(sock); return 1;
    }

    printf("UDP server listening on port %d\n", port);
    
    /* Initialize random number generator */
    srand(time(NULL));

    char buf[BUFSZ];
    
    /* Main server loop: accept and process client requests */
    for (;;) {
        /* Receive data from client */
        struct sockaddr_in cli;
        socklen_t clilen = sizeof(cli);
        ssize_t r = recvfrom(sock, buf, sizeof(buf)-1, 0, (struct sockaddr*)&cli, &clilen);
        if (r <= 0) continue;
        
        /* Null-terminate received data and parse as integer */
        buf[r] = '\0';
        int n = atoi(buf);
        if (n <= 0) n = 1;
        
        /* Build response: n random numbers separated by space */
        char out[BUFSZ];
        out[0] = '\0';
        for (int i = 0; i < n; ++i) {
            char t[32];
            snprintf(t, sizeof(t), "%d", rand() % 1000);
            if (i) strncat(out, " ", sizeof(out)-strlen(out)-1);
            strncat(out, t, sizeof(out)-strlen(out)-1);
        }
        strncat(out, "\n", sizeof(out)-strlen(out)-1);

        /* Send response back to client */
        if (sendto(sock, out, strlen(out), 0, (struct sockaddr*)&cli, clilen) < 0) {
            perror("sendto");
        } else {
            /* Convert client IP address to string and log the request */
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &cli.sin_addr, ip, sizeof(ip));
            printf("Request from %s:%d n=%d -> sent %d numbers\n", ip, ntohs(cli.sin_port), n, n);
        }
    }

    /* Close socket and exit */
    close(sock);
    return 0;
}
