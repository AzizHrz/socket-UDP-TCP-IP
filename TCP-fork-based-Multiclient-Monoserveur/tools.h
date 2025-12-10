#ifndef TOOLS_H
#define TOOLS_H

#include <time.h>

/* Function prototypes for shared utilities used by the server */
void send_msg(int fd, const char *msg);
void send_file_content(int fd, const char *filename);
void send_directory_list(int fd, const char *path);
void send_datetime(int fd);

/* Expose server start time so services can compute uptime */
extern time_t start_time;

#endif /* TOOLS_H */
