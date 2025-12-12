/* client.c - TCP Client for multiservice server
   Based on clientTcp_fork.c logic: connect, read initial message, then loop send commands and read responses
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT_MAIN 9000  /* Must match PORT_MAIN_SERVER in main_server.c */
#define BUF_SIZE 4096   /* Buffer size for receiving data */

/* Connect to the main server at given IP */
int connect_main_server(const char *ip) {
    int sock;
    struct sockaddr_in serv;

    /* Create TCP socket */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    /* Configure server address */
    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT_MAIN);
    serv.sin_addr.s_addr = inet_addr(ip);

    /* Connect to server */
    if (connect(sock, (struct sockaddr *)&serv, sizeof(serv)) < 0) {
        perror("connect");
        exit(1);
    }

    return sock;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <SERVER_IP>\n", argv[0]);
        return 1;
    }

    const char *server_ip = argv[1];
    char buf[BUF_SIZE];

    printf("Connecting to MAIN SERVER at %s:%d...\n", server_ip, PORT_MAIN);
    int sock = connect_main_server(server_ip);

    printf("[CLIENT] Connected.\n");

    /* Read initial server message (menu) */
    int n = read(sock, buf, BUF_SIZE - 1);
    if (n > 0) {
        buf[n] = '\0';
        printf("%s", buf);
    }

    /* Interactive loop: send commands and receive responses */
    while (1) {
        char command[256];
        printf("Enter command: ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;  /* Remove trailing newline */

        /* Send command to server */
        write(sock, command, strlen(command));

        /* Check for quit */
        if (strcmp(command, "QUIT") == 0) {
            printf("Closing connection.\n");
            break;
        }

        /* Read server response until --END-- */
        char response[BUF_SIZE * 10] = "";  /* Large buffer for accumulation */
        char temp[BUF_SIZE];
        while (1) {
            n = read(sock, temp, BUF_SIZE - 1);
            if (n <= 0) break;
            temp[n] = '\0';
            strcat(response, temp);
            if (strstr(response, "--END--")) break;
        }
        /* Remove --END-- from response */
        char *end_marker = strstr(response, "--END--");
        if (end_marker) *end_marker = '\0';
        printf("%s", response);
    }

    close(sock);
    return 0;
}
