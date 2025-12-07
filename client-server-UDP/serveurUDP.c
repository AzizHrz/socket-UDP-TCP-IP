/* serveurUDP.c
   Usage: ./serveurUDP <port>
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSZ 2048

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1;
    }
    int port = atoi(argv[1]);
    if (port <= 0) { fprintf(stderr, "Bad port\n"); return 1; }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) { perror("socket"); return 1; }

    struct sockaddr_in srv;
    memset(&srv, 0, sizeof(srv));
    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = INADDR_ANY;
    srv.sin_port = htons(port);

    if (bind(sock, (struct sockaddr*)&srv, sizeof(srv)) < 0) {
        perror("bind"); close(sock); return 1;
    }

    printf("UDP server listening on port %d\n", port);
    srand(time(NULL));

    char buf[BUFSZ];
    for (;;) {
        struct sockaddr_in cli;
        socklen_t clilen = sizeof(cli);
        ssize_t r = recvfrom(sock, buf, sizeof(buf)-1, 0, (struct sockaddr*)&cli, &clilen);
        if (r <= 0) continue;
        buf[r] = '\0';
        int n = atoi(buf);
        if (n <= 0) n = 1;
        /* build response: n random numbers separated by space */
        char out[BUFSZ];
        out[0] = '\0';
        for (int i = 0; i < n; ++i) {
            char t[32];
            snprintf(t, sizeof(t), "%d", rand() % 1000);
            if (i) strncat(out, " ", sizeof(out)-strlen(out)-1);
            strncat(out, t, sizeof(out)-strlen(out)-1);
        }
        strncat(out, "\n", sizeof(out)-strlen(out)-1);

        if (sendto(sock, out, strlen(out), 0, (struct sockaddr*)&cli, clilen) < 0) {
            perror("sendto");
        } else {
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &cli.sin_addr, ip, sizeof(ip));
            printf("Request from %s:%d n=%d -> sent %d numbers\n", ip, ntohs(cli.sin_port), n, n);
        }
    }

    close(sock);
    return 0;
}
