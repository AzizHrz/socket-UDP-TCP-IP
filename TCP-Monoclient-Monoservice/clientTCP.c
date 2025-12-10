/* clientTCP.c
 * Monoclient / Monoserveur 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE 256

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *server_ip = argv[1];
    int port = atoi(argv[2]);

    /* ---- 1. Create socket ---- */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* ---- 2. Server address ---- */
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    /* ---- 3. Connect ---- */
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("[CLIENT] Connected to %s:%d\n", server_ip, port);

    /* ---- 4. Read server answer ---- */
    char buffer[BUF_SIZE];
    int n = read(sockfd, buffer, BUF_SIZE - 1);

    if (n > 0) {
        buffer[n] = '\0';
        printf("Received: %s\n", buffer);
    }

    /* ---- 5. Close socket ---- */
    close(sockfd);

    return 0;
}
