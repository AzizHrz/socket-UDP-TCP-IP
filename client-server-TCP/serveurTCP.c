/* serveurTCP.c
   Usage: ./serveurTCP <port>
   Parallel server: each client handled in child process (fork).
   Simple hardcoded authentication: username "alice", password "wonderland".
*/
#define _POSIX_C_SOURCE 200809L
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/wait.h>
#include <signal.h>

#define BUFSZ 4096

static void handle_sigchld(int sig) {
    (void)sig;
    while (waitpid(-1, NULL, WNOHANG) > 0) {}
}

/* helper: read line ending with '\n' */
static ssize_t read_line(int fd, char *buf, size_t max) {
    size_t pos = 0;
    while (pos + 1 < max) {
        ssize_t r = recv(fd, buf + pos, 1, 0);
        if (r <= 0) return r;
        if (buf[pos] == '\r') continue; /* ignore CR */
        pos++;
        if (buf[pos-1] == '\n') break;
    }
    buf[pos] = '\0';
    return (ssize_t)pos;
}

static int send_response(int fd, const char *fmt, ...) {
    char tmp[BUFSZ];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(tmp, sizeof(tmp), fmt, ap);
    va_end(ap);
    /* send body */
    if (send(fd, tmp, strlen(tmp), 0) < 0) return -1;
    /* send sentinel */
    if (send(fd, "<END>\n", 6, 0) < 0) return -1;
    return 0;
}

static void do_client(int cfd) {
    char line[BUFSZ];
    time_t start = time(NULL);

    /* Authentication */
    ssize_t n = read_line(cfd, line, sizeof(line));
    if (n <= 0) { close(cfd); return; }
    /* expect: AUTH user pass\n */
    char cmd[32], user[128], pass[128];
    if (sscanf(line, "%31s %127s %127s", cmd, user, pass) != 3) {
        send_response(cfd, "AUTH_FAIL\n");
        close(cfd); return;
    }
    if (strcmp(cmd, "AUTH") != 0) {
        send_response(cfd, "AUTH_FAIL\n");
        close(cfd); return;
    }
    /* hardcoded check */
    if (strcmp(user, "alice") != 0 || strcmp(pass, "wonderland") != 0) {
        send_response(cfd, "AUTH_FAIL\n");
        close(cfd); return;
    }
    send_response(cfd, "AUTH_OK\nWelcome %s\n", user);

    /* service loop */
    for (;;) {
        ssize_t r = read_line(cfd, line, sizeof(line));
        if (r <= 0) break;
        /* trim newline */
        if (line[r-1] == '\n') line[r-1] = '\0';
        /* parse commands */
        if (strcmp(line, "SERVICES") == 0) {
            send_response(cfd,
                "SERVICES:\n- DATE\n- LS <dir>\n- CAT <file>\n- UPTIME\n- QUIT\n");
        } else if (strcmp(line, "DATE") == 0) {
            time_t now = time(NULL);
            struct tm tm = *localtime(&now);
            char datestr[128];
            strftime(datestr, sizeof(datestr), "%A %d/%m/%Y %H:%M:%S\n", &tm);
            send_response(cfd, "%s", datestr);
        } else if (strncmp(line, "LS ", 3) == 0) {
            char *dir = line + 3;
            DIR *d = opendir(dir);
            if (!d) {
                send_response(cfd, "ERROR: cannot open dir %s\n", dir);
            } else {
                struct dirent *ent;
                char out[BUFSZ];
                out[0] = '\0';
                while ((ent = readdir(d)) != NULL) {
                    strncat(out, ent->d_name, sizeof(out)-strlen(out)-1);
                    strncat(out, "\n", sizeof(out)-strlen(out)-1);
                }
                closedir(d);
                send_response(cfd, "%s", out);
            }
        } else if (strncmp(line, "CAT ", 4) == 0) {
            char *file = line + 4;
            FILE *f = fopen(file, "r");
            if (!f) {
                send_response(cfd, "ERROR: cannot open file %s\n", file);
            } else {
                char buffer[1024];
                size_t rr;
                /* read and send in parts by building one response string (careful on big files) */
                /* We'll stream line by line to client then send sentinel */
                while ((rr = fread(buffer, 1, sizeof(buffer), f)) > 0) {
                    if (send(cfd, buffer, rr, 0) < 0) break;
                }
                fclose(f);
                /* after file contents, send sentinel */
                send(cfd, "\n<END>\n", 7, 0);
            }
        } else if (strcmp(line, "UPTIME") == 0) {
            time_t now = time(NULL);
            double elapsed = difftime(now, start);
            send_response(cfd, "Elapsed seconds: %.0f\n", elapsed);
        } else if (strcmp(line, "QUIT") == 0) {
            send_response(cfd, "BYE\n");
            break;
        } else {
            send_response(cfd, "Unknown command: %s\n", line);
        }
    }

    close(cfd);
}

int main(int argc, char **argv) {
    if (argc != 2) { fprintf(stderr, "Usage: %s <port>\n", argv[0]); return 1; }
    int port = atoi(argv[1]); if (port <= 0) { fprintf(stderr, "Bad port\n"); return 1; }

    signal(SIGCHLD, handle_sigchld);

    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { perror("socket"); return 1; }

    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) { perror("bind"); close(s); return 1; }
    if (listen(s, 10) < 0) { perror("listen"); close(s); return 1; }
    printf("TCP server listening on port %d\n", port);

    while (1) {
        struct sockaddr_in cli;
        socklen_t clilen = sizeof(cli);
        int c = accept(s, (struct sockaddr*)&cli, &clilen);
        if (c < 0) { perror("accept"); continue; }
        pid_t pid = fork();
        if (pid < 0) { perror("fork"); close(c); continue; }
        if (pid == 0) {
            /* child */
            close(s);
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &cli.sin_addr, ip, sizeof(ip));
            printf("Client connected from %s:%d\n", ip, ntohs(cli.sin_port));
            do_client(c);
            printf("Client %s disconnected\n", ip);
            exit(0);
        } else {
            /* parent */
            close(c);
        }
    }

    close(s);
    return 0;
}
