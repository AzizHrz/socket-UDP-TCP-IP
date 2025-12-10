/* serveurTCP.c
 * Monoclient / Monoserveur (Monoservice)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define BUF_SIZE 256

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);

    /* ---- 1. Create socket ---- */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* ---- 2. Prepare server address ---- */
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;   // Accept connections from any host
    serv_addr.sin_port = htons(port);

    /* ---- 3. Bind socket ---- */
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    /* ---- 4. Listen ---- */
    if (listen(sockfd, 1) < 0) {
        perror("listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("[SERVER] Listening on port %d...\n", port);

    /* ---- 5. Accept one client ---- */
    int newsockfd = accept(sockfd, NULL, NULL);
    if (newsockfd < 0) {
        perror("accept");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("[SERVER] Client connected.\n");

    /* ---- 6. Service logic: send date ---- */
    char buffer[BUF_SIZE];
    time_t now = time(NULL);
    snprintf(buffer, BUF_SIZE, "Server date: %s", ctime(&now));

    ssize_t sent = write(newsockfd, buffer, strlen(buffer));
    if (sent < 0) {
    perror("write");
    }

    /* ---- 7. Close connections ---- */
    close(newsockfd);
    close(sockfd);

    printf("[SERVER] Connection closed.\n");

    return 0;
}
