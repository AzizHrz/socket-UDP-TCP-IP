#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "tools.h"
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include "tools.h"

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