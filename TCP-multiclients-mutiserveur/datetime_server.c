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
    addr.sin_port = htons(PORT_DATE_TIME_SERVICE);

    bind(s, (struct sockaddr*)&addr, sizeof(addr));
    listen(s, 5);

    printf("[DATE_TIME] Running on port %d\n", PORT_DATE_TIME_SERVICE);

    while (1) {
        int c = accept(s, NULL, NULL);
        time_t now = time(NULL);
        write(c, ctime(&now), strlen(ctime(&now)));
        close(c);
    }
}
