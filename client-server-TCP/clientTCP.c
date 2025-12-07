/* clientTCP.c
   Usage: ./clientTCP <server-host> <server-port>
   Simple interactive client.
*/
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFSZ 4096

/* read until sentinel "<END>\n" is seen, print response to stdout */
static int read_response(int fd) {
    char buf[BUFSZ];
    size_t total = 0;
    for (;;) {
        ssize_t r = recv(fd, buf, sizeof(buf)-1, 0);
        if (r <= 0) return -1;
        buf[r] = '\0';
        /* search sentinel */
        char *p = strstr(buf, "<END>\n");
        if (p) {
            /* print up to sentinel */
            *p = '\0';
            printf("%s", buf);
            return 0;
        } else {
            printf("%s", buf);
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 3) { fprintf(stderr, "Usage: %s <server-host> <server-port>\n", argv[0]); return 1; }
    const char *host = argv[1], *port = argv[2];

    struct addrinfo hints, *res;
    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host, port, &hints, &res) != 0) { perror("getaddrinfo"); return 1; }

    int sock = socket(res->ai_family, res->ai_socktype, 0);
    if (sock < 0) { perror("socket"); freeaddrinfo(res); return 1; }
    if (connect(sock, res->ai_addr, res->ai_addrlen) < 0) { perror("connect"); close(sock); freeaddrinfo(res); return 1; }
    freeaddrinfo(res);

    /* authenticate: prompt user */
    char user[128], pass[128];
    printf("Username: "); fflush(stdout); if (!fgets(user, sizeof(user), stdin)) return 1;
    user[strcspn(user, "\n")] = '\0';
    printf("Password: "); fflush(stdout); if (!fgets(pass, sizeof(pass), stdin)) return 1;
    pass[strcspn(pass, "\n")] = '\0';

    char out[512];
    snprintf(out, sizeof(out), "AUTH %s %s\n", user, pass);
    send(sock, out, strlen(out), 0);

    if (read_response(sock) < 0) { fprintf(stderr,"Server closed\n"); close(sock); return 1; }

    /* main loop: read commands from stdin and send to server */
    char cmd[512];
    while (1) {
        printf("Command (SERVICES|DATE|LS <dir>|CAT <file>|UPTIME|QUIT): ");
        if (!fgets(cmd, sizeof(cmd), stdin)) break;
        if (cmd[strlen(cmd)-1] != '\n') { /* ensure newline */ strcat(cmd, "\n"); }
        send(sock, cmd, strlen(cmd), 0);
        if (strncmp(cmd, "QUIT", 4) == 0) break;
        if (read_response(sock) < 0) { fprintf(stderr, "Server closed\n"); break; }
    }

    close(sock);
    return 0;
}
