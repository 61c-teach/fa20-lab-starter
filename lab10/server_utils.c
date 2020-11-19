#include "server_utils.h"
#include <unistd.h>

char *header_tag_left = "<center><h1>";
char *header_tag_right = "</h1><hr></center>";
char *content_type = "Content-Type";
char *content_len = "Content-Length";
int dotp_size = 100000; // default to 5 0s, can be changed from command line arg
const char *template_str = "<!DOCTYPE html> \
<html lang=\"en\"> \
<head> \
    <meta charset=\"UTF-8\"> \
    <title>Sobel Edge Detector Result</title> \
</head> \
<body> \
<h1>Image Filter</h1> \
<h3>Original</h3> \
<img src=\".%s\"> \
<h3>Sobel-Edge-Detectored</h3> \
<img src=\".%s\"> \
\
</body> \
</html>";

/** Serves the file at path to the the socket fd. */
void handle_report_request(int socket_fd, int arr_size) {
   char *result = compute_dotp(arr_size);
   int size = strlen(result);
   http_make_header(socket_fd, "text/html", 200, size);
   http_send_data(socket_fd, result, size);
   free(result);
}

void http_make_header(int sfd,  char *ftype, int status_code, __off_t size) {
   http_start_response(sfd, 200);
   http_send_header(sfd, content_type, ftype);

   if (size != -1) {
      char size_char[64];
      sprintf(size_char, "%ld", size);
      http_send_header(sfd, content_len, size_char);
   }
   http_end_headers(sfd);
}

void handle_bmp_request(int socket_fd, char *path) {
   char *res = image_proc(path);
   char buf[2048];

   // error occurred in calling image_proc
   if (!res) {
      printf("Error occurred reading bmp image. \n");
      return;
   }

   // Serve the origin and modified image in a html file
   http_make_header(socket_fd, "text/html", 200, -1);
   sprintf(buf, template_str, path, res);
   http_send_string(socket_fd, buf);
}

/** Serves the file at path to the the socket fd. */
void http_serve_file(int socket_fd, char *path, int size) {
   char *type = http_get_mime_type(basename(path));
   int  filed, len_count = 0, cnt = 0;
   char buf[BUFSIZ];

   http_make_header(socket_fd, type, 200, size);
   filed = open(path, O_RDONLY);
   len_count = 0;
   while ((cnt = read(filed, buf, BUFSIZ - 1)) > 0) {
      len_count += cnt;
      http_send_data(socket_fd, buf, (size_t)cnt);
      lseek(filed, len_count, SEEK_SET);
   }
   close(filed);
}

/** Serves the directory at path to the the socket fd. */
void http_serve_directory(int socket_fd,  char *path) {
   DIR *dir = opendir(path);
   struct dirent *ent;
   char* fname;
   char buf[256];

   http_make_header(socket_fd, "text/html", 200, -1);
   char* start = "<ul>";
   http_send_string(socket_fd, start);

   while ((ent = readdir(dir)) != NULL) {
      fname = ent->d_name;
      sprintf(buf, "<li><a href=\".%s%s\">%s</a></li>\n", path, fname, fname);
      http_send_string(socket_fd, buf);
   }
   char *end = "</ul>";
   http_send_string(socket_fd, end);
}

void http_make_error(int socket_fd, int status) {
   char msg[64] = {0};
   strcat(msg, header_tag_left);
   strcat(msg, http_get_response_message(status));
   strcat(msg, header_tag_right);

   http_make_header(socket_fd, "text/html", status, strlen(msg));
   http_send_string(socket_fd, msg);
}

void signal_callback_handler(int signum) {
   printf("Caught signal %d: %s\n", signum, strsignal(signum));
   printf("Closing socket %d\n", server_fd);
   if (shutdown(server_fd, SHUT_RDWR) < 0)
      perror("Failed to shutdown socket at server_fd (ignoring)\n");
   if (close(server_fd) < 0) perror("Failed to close server_fd (ignoring)\n");
   exit(EXIT_SUCCESS);
}

void exit_with_usage(char *executable_name) {
   fprintf(stderr, "Usage:");
   fprintf(stderr, " %s ", executable_name);
   fprintf(stderr, "%s", USAGE);
   exit(EXIT_SUCCESS);
}

inline int start_with(char *src, char *target) {
   return strncmp(src, target, strlen(target)) == 0;
}

/** Reads an HTTP request from socket_fd, and deliver an HTTP response
 *   1) If requested an existing file, respond with the file
 *   2) If requested a directory:
 *      if there exists an index.html present in the directory,
 *      send index.html. otherwise list files in the directory with links.
 *   4) Send a 404 Not Found if no result found. */
void handle_files_request(int socket_fd, struct http_request *request) {
   char path[128] = {0};
   path[0] = '.';

   // If requested file is a bmp image, apply sobel edge detector
   // and deliver the result images
   if (start_with(request->path, "/filter") && strcmp(http_get_mime_type(request->path), "image/bmp") == 0 ) {
      strcpy(path + 1, request->path + strlen("/filter"));
      handle_bmp_request(socket_fd, path);
      return;
   }
   strcpy(path + 1, request->path);

   struct stat file_stat;
   stat(path, &file_stat);

   if ((file_stat.st_mode & __S_IFMT) == __S_IFREG) {
      http_serve_file(socket_fd, path, file_stat.st_size);
      return;
   } else if ((file_stat.st_mode & __S_IFMT) == __S_IFDIR) {
      char path_origin[128] = {0};
      strncpy(path_origin, path, strlen(path));
      strcat(path, "//index.html");
      if (stat(path, &file_stat) == -1) {
         http_serve_directory(socket_fd, path_origin);
      } else {
         http_serve_file(socket_fd, path, file_stat.st_size);
      }
      return;
   }

   http_make_error(socket_fd, 404);
   return;
}

void dispatch(int client_socket_number) {
   struct http_request *request = http_request_parse(client_socket_number);

   if (request == NULL || request->path[0] != '/') {
      http_make_error(client_socket_number, 400); return;
   }

   if (strstr(request->path, "..") != NULL) {
      http_make_error(client_socket_number, 403); return;
   }

   printf("%s\n", request->path);
   if (strcmp(request->path, "/report") == 0) {
      handle_report_request(client_socket_number, dotp_size);
      return;
   }

   if (strcmp(request->method, "GET") != 0) {
      printf("Sorry we only support GET method so far... %s\n", request->method);
      return;
   }

   // only support GET request
   void (*request_handler)(int, struct http_request*) = &handle_files_request;

   request_handler(client_socket_number, request);
   close(client_socket_number);

   sleep(5);      // Pretending we are doing some heavy computation...
}

/** Open a TCP socket on all interfaces. *socket_number stores
 * the fd number of the server socket in call request_handler
 * with the accepted socket fd number on an accepted connection.*/
void serve_forever(int *socket_number) {
   struct sockaddr_in server_address, client_address;
   size_t client_address_length = sizeof(client_address);
   int client_socket_number;

   *socket_number = socket(PF_INET, SOCK_STREAM, 0);
   if (*socket_number == -1) {
      perror("Failed to create new socket");
      exit(errno);
   }

   int socket_option = 1;
   if (setsockopt(*socket_number, SOL_SOCKET, SO_REUSEADDR, &socket_option,
                  sizeof(socket_option)) == -1) {
      perror("Failed to set socket options");
      exit(errno);
   }

   memset(&server_address, 0, sizeof(server_address));
   server_address.sin_family = AF_INET;
   server_address.sin_addr.s_addr = INADDR_ANY;
   server_address.sin_port = htons(server_port);

   if (bind(*socket_number, (struct sockaddr *)&server_address,
            sizeof(server_address)) == -1) {
      perror("Failed to bind on socket");
      exit(errno);
   }

   if (listen(*socket_number, 1024) == -1) {
      perror("Failed to listen on socket");
      exit(errno);
   }

   printf("Listening on port %d...\n", server_port);

   while (1) {
      client_socket_number = accept(*socket_number,
                                    (struct sockaddr *) &client_address,
                                    (socklen_t * ) & client_address_length);
      if (client_socket_number < 0) {
         perror("Error accepting socket");
         continue;
      }

      printf("Accepted connection from %s on port %d\n",
             inet_ntoa(client_address.sin_addr), client_address.sin_port);

      pid_t parent_pid = getpid();
#ifdef PROC
      // PART 2 TASK: Implement forking
      /* YOUR CODE HERE */

      if (/* YOUR CODE HERE */) {
         // This line kills the child process if parent dies
         int r = prctl(PR_SET_PDEATHSIG, SIGTERM);

         /* YOUR CODE HERE */
         
         // These lines exit the current process with code 1 
         // 1) when there was an error in prctl, 2) when the parent has been killed
         if (r == -1 || getppid() != parent_pid) {
            perror(0);
            exit(1);
         }

         /* YOUR CODE HERE */
      }
#else
      dispatch(client_socket_number);
#endif
   }
}