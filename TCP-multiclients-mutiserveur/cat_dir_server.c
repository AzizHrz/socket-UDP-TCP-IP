#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "ports.h"
#include "network.h"

int main() {
    int s = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT_CAT_DIR_SERVICE);

    bind(s, (struct sockaddr*)&addr, sizeof(addr));
    listen(s, 5);

    printf("[CAT_DIR] Running on port %d\n", PORT_CAT_DIR_SERVICE);

    char file[256];

    while (1) {
        int c = accept(s, NULL, NULL);
        int n = read_all(c, file, sizeof(file));

        FILE *f = fopen(file, "r");
        if (!f) {
            write(c, "ERROR: cannot open file\n", 24);
        } else {
            char line[256];
            while (fgets(line, sizeof(line), f))
                write(c, line, strlen(line));
            fclose(f);
        }
        close(c);
    }
}
