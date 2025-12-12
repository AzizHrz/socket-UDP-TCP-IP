/* serveurTCP.c
 * Stage 1 — Monoclient / Monoserveur (Multiservice)
 * Architecture: Blocking I/O, single process, single thread
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>

#define BUF_SIZE 512

// Send string safely
void send_msg(int fd, const char *msg) {
    ssize_t n = write(fd, msg, strlen(msg));
    if (n < 0) perror("write");
}

// Send file content
void send_file_content(int fd, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        send_msg(fd, "ERROR: Cannot open file\n");
        return;
    }

    char buf[BUF_SIZE];
    while (fgets(buf, sizeof(buf), f) != NULL) {
        send_msg(fd, buf);
    }
    fclose(f);
}

// Send directory listing
void send_directory_list(int fd, const char *path) {
    DIR *d = opendir(path);
    if (!d) {
        send_msg(fd, "ERROR: Cannot open directory\n");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(d)) != NULL) {
        send_msg(fd, entry->d_name);
        send_msg(fd, "\n");
    }
    closedir(d);
}

// Send current server date/time
void send_datetime(int fd) {
    time_t now = time(NULL);
    char buf[BUF_SIZE];
    struct tm *t = localtime(&now);

    snprintf(buf, sizeof(buf),
             "Date: %02d/%02d/%04d - Time: %02d:%02d:%02d\n",
             t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
             t->tm_hour, t->tm_min, t->tm_sec);

    send_msg(fd, buf);
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("socket"); exit(EXIT_FAILURE); }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind"); close(sockfd); exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 1) < 0) {
        perror("listen"); close(sockfd); exit(EXIT_FAILURE);
    }

    printf("[SERVER] Waiting on port %d...\n", port);

    int clientfd = accept(sockfd, NULL, NULL);
    if (clientfd < 0) {
        perror("accept"); close(sockfd); exit(EXIT_FAILURE);
    }

    printf("[SERVER] Client connected.\n");

    // Start measuring uptime
    time_t start_time = time(NULL);

    char buffer[BUF_SIZE];

    // Blocking loop: wait for commands
    while (1) {
        memset(buffer, 0, BUF_SIZE);
        int n = read(clientfd, buffer, BUF_SIZE - 1);

        if (n <= 0) {
            printf("[SERVER] Client disconnected.\n");
            break;
        }

        buffer[n] = '\0';

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
        else if (strncmp(buffer, "QUIT", 4) == 0) {
            send_msg(clientfd, "Goodbye!\n");
            break;
        }
        else {
            send_msg(clientfd, "Unknown command\n");
        }
    }

    close(clientfd);
    close(sockfd);
    return 0;
}
