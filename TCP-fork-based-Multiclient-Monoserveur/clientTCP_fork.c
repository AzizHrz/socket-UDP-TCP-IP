/* clientTCP.c
 * Works with the updated serveurTCP.c multiservice implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE 512

// Read server response (blocking)
void read_response(int sockfd) {
    char buffer[BUF_SIZE + 1];
    int n = read(sockfd, buffer, BUF_SIZE);

    if (n > 0) {
        buffer[n] = '\0';
        printf("SERVER: %s", buffer);
        fflush(stdout);

    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *server_ip = argv[1];
    int port = atoi(argv[2]);

    /* 1. Create socket */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("socket"); exit(EXIT_FAILURE); }

    /* 2. Server address */
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    /* 3. Connect */
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("[CLIENT] Connected to %s:%d\n", server_ip, port);

    /* 4. Command loop */
    char cmd[BUF_SIZE];

    read_response(sockfd);

    while (1) {
        printf("\nCommands:\n");
        printf("  DATE\n");
        printf("  LS <dir>\n");
        printf("  CAT <file>\n");
        printf("  UPTIME\n");
        printf("  QUIT\n");
        printf("Enter command: ");

        fflush(stdout);

        // Read user input
        if (!fgets(cmd, sizeof(cmd), stdin))
            break;

        // Send command to server
        if (write(sockfd, cmd, strlen(cmd)) < 0) {
            perror("write");
            break;
        }

        // If QUIT: break after sending
        if (strncmp(cmd, "QUIT", 4) == 0) {
            read_response(sockfd);
            break;
        }

        // Read server response
        read_response(sockfd);
    }

    /* 5. Close */
    close(sockfd);
    printf("[CLIENT] Closed.\n");
    return 0;
}
