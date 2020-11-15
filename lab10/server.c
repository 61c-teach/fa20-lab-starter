#include "server_utils.h"

char *USAGE = "--files directory/ [--port 8000 --concurrency 5]\n";
char *report = "report.txt";

int main(int argc, char **argv) {
   signal(SIGINT, signal_callback_handler);

// default configs
   server_port = 8000;
   server_files_directory = "./files/";

  int i;
  for (i = 1; i < argc; i++) {
    if (strcmp("--files", argv[i]) == 0) {
      server_files_directory = argv[++i];
      if (!server_files_directory) {
        fprintf(stderr, "Expected argument after --files\n");
        exit_with_usage(argv[0]);
      }
    } else if (strcmp("--dotp-size", argv[i]) == 0) {
       char *size_str = argv[++i];
       if (!size_str) {
          fprintf(stderr, "Expected argument after --dotp-size\n");
          exit_with_usage(argv[0]);
       }
       dotp_size = atoi(size_str);
    } else if (strcmp("--port", argv[i]) == 0) {
      char *server_port_string = argv[++i];
      if (!server_port_string) {
        fprintf(stderr, "Expected argument after --port\n");
        exit_with_usage(argv[0]);
      }
      server_port = atoi(server_port_string);
    } else if (strcmp("--help", argv[i]) == 0) {
      exit_with_usage(argv[0]);
    } else {
      fprintf(stderr, "Unrecognized option: %s\n", argv[i]);
      exit_with_usage(argv[0]);
    }
  }

  chdir(server_files_directory);
  serve_forever(&server_fd);
  exit(EXIT_SUCCESS);
}
