#include "main.h"
#include "server.h"

int main() {

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
  int listened = spawn_listener(&server_fd, &address);

  return 0;
}
