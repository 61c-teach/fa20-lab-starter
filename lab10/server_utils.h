#ifndef THREAD_LAB_SERVER_UTILS_H
#define THREAD_LAB_SERVER_UTILS_H

#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/prctl.h>

#include "omp_apps.h"
#include "libhttp/libhttp.h"

/** Global configuration variables. */
int server_fd;
int server_port;
char *server_files_directory;

extern char *USAGE;
extern char *report;
extern char *header_tag_left;
extern char *header_tag_right;
extern char *content_type;
extern char *content_len;
extern int dotp_size;

int start_with(char*, char*);

void http_make_header(int,  char *, int, __off_t);

void http_make_error(int , int );

void signal_callback_handler(int );

void exit_with_usage(char*);

void handle_report_request(int, int);

/** Serves the file at path to the the socket fd. */
void http_serve_file(int,  char*, int);

/** Serves the directory at path to the the socket fd. */
void http_serve_directory(int socket_fd,  char *path);

/** Request handler**/
void handle_files_request(int socket_fd, struct http_request*);

void serve_forever(int *socket_number);

void dispatch(int);

#endif //THREAD_LAB_SERVER_UTILS_H
