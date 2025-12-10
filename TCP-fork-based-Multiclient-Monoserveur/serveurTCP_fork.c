/* serveurTCP_fork.c
 * Stage 2 — Multiclient / Monoserveur (Monoservice)
 * Architecture: Parallel server using fork()
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>
#include "tools.h"

#define BUF_SIZE 256
 
/* Global server start time (used by services to report uptime) */
time_t start_time;
void services(char *buffer, int clientfd) {

        if (strncmp(buffer, "DATE", 4) == 0) {
            send_datetime(clientfd);
        }
        else if (strncmp(buffer, "LS ", 3) == 0) {
            char dir[256];
            sscanf(buffer + 3, "%s", dir);
            send_directory_list(clientfd, dir);
        }
        else if (strncmp(buffer, "CAT ", 4) == 0) {
            char filename[256];
            sscanf(buffer + 4, "%s", filename);
            send_file_content(clientfd, filename);
        }
        else if (strncmp(buffer, "UPTIME", 6) == 0) {
            time_t now = time(NULL);
            int diff = (int) difftime(now, start_time);

            char upbuf[128];
            snprintf(upbuf, sizeof(upbuf),
                     "Connection uptime: %d seconds\n", diff);

            send_msg(clientfd, upbuf);
        }
        else {
            send_msg(clientfd, "Unknown command\n");
        }

} 

// Child process handles one client
void handle_client(int clientfd) {
    char buffer[BUF_SIZE];

    // Example simple service: send date
    time_t now = time(NULL);
    snprintf(buffer, BUF_SIZE,
             "Welcome! Server date: %s\n", ctime(&now));
    ssize_t sizeW = write(clientfd, buffer, strlen(buffer));
    if (sizeW < 0) {
        perror("write error");
        close(clientfd);
        exit(EXIT_FAILURE);
    }

    // Optionally read something from client (blocking)
    while (1) {
        int n = read(clientfd, buffer, BUF_SIZE - 1);
        if (n <= 0) break; // client disconnected
        buffer[n] = '\0';
        printf("[Child %d] Received from client: %s", getpid(), buffer);
        if (strncmp(buffer, "QUIT", 4) == 0) {
            send_msg(clientfd, "Goodbye!\n");
            break;
        }
        else {
            services(buffer, clientfd);
        }    
    }

    printf("[Child %d] Client disconnected.\n", getpid());
    close(clientfd);
    exit(0); // Child exits
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);

    // Avoid zombie processes
    signal(SIGCHLD, SIG_IGN);

    // 1. Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("socket"); exit(EXIT_FAILURE); }

    // 2. Prepare address
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    // 3. Bind
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind"); close(sockfd); exit(EXIT_FAILURE);
    }

    // 4. Listen
    if (listen(sockfd, 10) < 0) {
        perror("listen"); close(sockfd); exit(EXIT_FAILURE);
    }

    printf("[SERVER] Listening on port %d...\n", port);
    
    /* Record server start time for uptime reporting */
    start_time = time(NULL);
    // 5. Accept clients in a loop
    while (1) {
        /* Accept a client and capture its address so we can log IP:port */
        struct sockaddr_in cli_addr;
        socklen_t cli_len = sizeof(cli_addr);
        int clientfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);
        if (clientfd < 0) {
            perror("accept"); 
            continue;
        }

        /* Log client's IP and port */
        char cli_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &cli_addr.sin_addr, cli_ip, sizeof(cli_ip));
        printf("[SERVER] New client connected from %s:%d\n", cli_ip, ntohs(cli_addr.sin_port));

        // 6. Fork a child to handle the client
        int pid = fork();

        if (pid < 0) {
            perror("fork");
            close(clientfd);
            continue;
        }

        if (pid == 0) {
            // CHILD process
            close(sockfd);          // child does not accept new clients
            handle_client(clientfd);
        } else {
            // PARENT process
            close(clientfd);        // parent closes client socket
        }
    }

    close(sockfd);
    return 0;
}
