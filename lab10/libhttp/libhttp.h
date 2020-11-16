/*
 * A simple HTTP library.
 * Usage example:
 *
 *     // Returns NULL if an error was encountered.
 *     struct http_request *request = http_request_parse(fd);
 *
 *     ...
 *
 *     http_start_response(fd, 200);
 *     http_send_header(fd, "Content-type", http_get_mime_type("index_backup.html"));
 *     http_send_header(fd, "Server", "httpserver/1.0");
 *     http_end_headers(fd);
 *     http_send_string(fd, "<html><body><a href='/'>Home</a></body></html>");
 *
 *     close(fd);
 */

#ifndef LIBHTTP_H
#define LIBHTTP_H

#include <stddef.h>

/** Functions for parsing an HTTP request. */
struct http_request {
  char *method;
  char *path;
};

struct http_request *http_request_parse(int fd);
char *http_get_response_message(int status_code);

/** Functions for sending an HTTP response. */
void http_start_response(int fd, int status_code);
void http_send_header(int fd, char *key, char *value);
void http_end_headers(int fd);
void http_send_string(int fd, char *data);
void http_send_data(int fd, char *data, size_t size);

/** Helper function: gets the Content-Type based on a file name. */
char *http_get_mime_type(char *file_name);

void route();

// some interesting macro for `route()`
#define ROUTE_START() if (0) {
#define ROUTE(METHOD, URI) \
  }                        \
  else if (strcmp(URI, uri) == 0 && strcmp(METHOD, method) == 0) {
#define ROUTE_GET(URI) ROUTE("GET", URI)
#define ROUTE_POST(URI) ROUTE("POST", URI)
#define ROUTE_END()                            \
  }                                            \
  else printf(                                 \
      "HTTP/1.1 500 Internal Server Error\n\n" \
      "The server has no handler to the request.\n");

#endif
