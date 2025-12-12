// main_server.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include "ports.h"
#include "network.h"

#define BUFFER 512
#define MAX_CLIENTS 50

typedef struct {
    int fd;
    time_t start;
} client_context;

const char *menu =
"*********** SERVICES ***********\n"
"LISTS_DIR <path>\n"
"CAT_DIR <file>\n"
"ELAPSED_TIME\n"
"DATE_TIME\n"
"END\n"
"*******************************\n";

void *client_handler(void *arg) {
    client_context *ctx = arg;

    write(ctx->fd, menu, strlen(menu));

    char buf[BUFFER];
    while (1) {
        int n = read_all(ctx->fd, buf, sizeof(buf));
        if (n <= 0) break;

        if (!strncmp(buf, "LISTS_DIR ", 10)) {
            int svc = connect_to_service(PORT_LISTS_DIR_SERVICE);
            write(svc, buf+10, strlen(buf)-10);
            char res[BUFFER];
            while (read_all(svc, res, sizeof(res)) > 0)
                write(ctx->fd, res, strlen(res));
            close(svc);
        }
        else if (!strncmp(buf, "CAT_DIR ", 8)) {
            int svc = connect_to_service(PORT_CAT_DIR_SERVICE);
            write(svc, buf+8, strlen(buf)-8);
            char res[BUFFER];
            while (read_all(svc, res, sizeof(res)) > 0)
                write(ctx->fd, res, strlen(res));
            close(svc);
        }
        else if (!strncmp(buf, "ELAPSED_TIME", 12)) {
            int svc = connect_to_service(PORT_ELAPSED_SERVICE);
            char res[BUFFER];
            read_all(svc, res, sizeof(res));
            close(svc);

            time_t now_remote = atol(res);
            int elapsed = now_remote - ctx->start;
            
            char out[64];
            snprintf(out, sizeof(out), "%d seconds\n", elapsed);
            write(ctx->fd, out, strlen(out));
        }
        else if (!strncmp(buf, "DATE_TIME", 9)) {
            int svc = connect_to_service(PORT_DATE_TIME_SERVICE);
            char res[BUFFER];
            read_all(svc, res, sizeof(res));
            write(ctx->fd, res, strlen(res));
            close(svc);
        }
        else if (!strncmp(buf, "END", 3)) {
            write(ctx->fd, "Goodbye!\n", 9);
            break;
        }
        write(ctx->fd, menu, strlen(menu));
    }

    close(ctx->fd);
    free(ctx);
    return NULL;
}

int main() {
    int s = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT_MAIN_SERVER);

    bind(s, (struct sockaddr*)&addr, sizeof(addr));
    listen(s, MAX_CLIENTS);

    printf("[MAIN SERVER] Running on port %d\n", PORT_MAIN_SERVER);

    while (1) {
        int c = accept(s, NULL, NULL);

        client_context *ctx = malloc(sizeof(client_context));
        ctx->fd = c;
        ctx->start = time(NULL);

        pthread_t t;
        pthread_create(&t, NULL, client_handler, ctx);
        pthread_detach(t);
    }
}
