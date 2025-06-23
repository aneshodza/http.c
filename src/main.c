#include "main.h"

void print_hello_message() {
  printf("Welcome to an HTTP-Server written by Anes Hodza!\n");
  printf("repository: github.com/aneshodza/http.c\n");
  printf("    __    __  __             \n");
  printf("   / /_  / /_/ /_____   _____\n");
  printf("  / __ \\/ __/ __/ __ \\ / ___/\n");
  printf(" / / / / /_/ /_/ /_/ // /__  \n");
  printf("/_/ /_/\\__/\\__/ .___(_)___/  \n");
  printf("             /_/             \n");
}

int main() {

  (void)print_hello_message();

  /* Create signal handler */
  int handled = create_handlers();
  if (handled == -1) {
    printf("Could not spawn signal handlers. Aborting...");
    return -1;
  }

  /* Spawn the actual server */
  int server_fd;
  struct sockaddr_in address;

  int port = spawn_server(&server_fd, &address);
  if (port == -1) {
    printf("The server could not start. Aborting...");
    return -1;
  }
  printf("Server is listening on port %d...\n", port);

  int created = create_threadpool();
  if (created == -1) {
    printf("The server could not spawn the workers");
    return -1;
  }
  spawn_listener(&server_fd, &address);

  return 0;
}
