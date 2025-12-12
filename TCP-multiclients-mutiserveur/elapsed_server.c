#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include "ports.h"
#include "network.h"

int main() {
    int s = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT_ELAPSED_SERVICE);

    bind(s, (struct sockaddr*)&addr, sizeof(addr));
    listen(s, 5);

    printf("[ELAPSED_TIME] Running on port %d\n", PORT_ELAPSED_SERVICE);

    while (1) {
        int c = accept(s, NULL, NULL);

        // Just return current system time (client knows its start)
        time_t now = time(NULL);
        char out[128];
        snprintf(out, sizeof(out), "%ld\n", now);

        write(c, out, strlen(out));
        close(c);
    }
}
