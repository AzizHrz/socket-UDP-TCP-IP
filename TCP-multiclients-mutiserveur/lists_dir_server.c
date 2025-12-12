#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <arpa/inet.h>
#include "ports.h"
#include "network.h"

int main() {
    int s = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT_LISTS_DIR_SERVICE);

    bind(s, (struct sockaddr*)&addr, sizeof(addr));
    listen(s, 5);

    printf("[LISTS_DIR] Running on port %d\n", PORT_LISTS_DIR_SERVICE);

    char buf[256];

    while (1) {
        int c = accept(s, NULL, NULL);
        int n = read_all(c, buf, sizeof(buf));

        DIR *d = opendir(buf);
        if (!d) {
            write(c, "ERROR: cannot open directory\n", 30);
        } else {
            struct dirent *e;
            while ((e = readdir(d)) != NULL) {
                write(c, e->d_name, strlen(e->d_name));
                write(c, "\n", 1);
            }
            closedir(d);
        }
        close(c);
    }
}
