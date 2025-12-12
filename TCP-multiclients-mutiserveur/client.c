/* client.c - TCP Client for multiservice server
   Connects to main server and sends commands like LISTS_DIR, CAT_DIR, etc.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT_MAIN 9000  /* Must match PORT_MAIN_SERVER in main_server.c */
#define MAXBUF 4096     /* Buffer size for receiving data */

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

/* Send a string message to the server */
void send_line(int sock, const char *msg) {
    send(sock, msg, strlen(msg), 0);
}

/* Receive a line from the server (null-terminated) */
int recv_line(int sock, char *buf) {
    int n = recv(sock, buf, MAXBUF - 1, 0);
    if (n <= 0) return 0;
    buf[n] = 0;  /* Null-terminate */
    return n;
}

/* Authentication function (currently disabled) */
void authenticate(int sock) {
    char buf[MAXBUF];

    recv_line(sock, buf);
    printf("%s\n", buf);   // "Authenticate yourself"

    char username[100], password[100], packet[MAXBUF];

    while (1) {
        printf("Username: ");
        scanf("%s", username);

        printf("Password: ");
        scanf("%s", password);

        snprintf(packet, sizeof(packet), "AUTH %s %s\n", username, password);
        send_line(sock, packet);

        recv_line(sock, buf);

        if (strcmp(buf, "OK") == 0) {
            printf("[OK] Authentication success\n");
            break;
        } else {
            printf("[ERR] Wrong credentials. Try again.\n");
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <SERVER_IP>\n", argv[0]);
        return 1;
    }

    const char *server_ip = argv[1];
    char buf[MAXBUF];
    char command[MAXBUF];

    printf("Connecting to MAIN SERVER at %s:%d...\n", server_ip, PORT_MAIN);
    int sock = connect_main_server(server_ip);

    /* Optional authentication (commented out as server doesn't require it) */
    // authenticate(sock);

    /* Main interaction loop */
    while (1) {
        /* Receive and display the menu from server */
        recv_line(sock, buf);
        printf("\n=== SERVICES ===\n%s\n", buf);

        /* Prompt user for command */
        printf("Enter command (e.g., LISTS_DIR /path, CAT_DIR file.txt, ELAPSED_TIME, DATE_TIME, END): ");
        fgets(command, sizeof(command), stdin);
        /* Remove trailing newline */
        command[strcspn(command, "\n")] = 0;

        /* Send the command to server */
        send_line(sock, command);
        send_line(sock, "\n");  /* Ensure newline for server parsing */

        /* Check if user wants to quit */
        if (strcmp(command, "END") == 0) {
            printf("Closing connection.\n");
            break;
        }

        /* Receive and display server response */
        recv_line(sock, buf);
        printf("\n[SERVER RESPONSE]:\n%s\n", buf);
    }

    close(sock);
    return 0;
}
