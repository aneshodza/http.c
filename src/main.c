#include "main.h"
#include "server.h"

int main() {
  int server_fd;
  struct sockaddr_in address;

  int port = spawn_server(&server_fd, &address);
  if (port == -1) {
    printf("The server could not start. Aborting...");
    return -1;
  }
  printf("Server is listening on port %d...\n", port);

  int listened = spawn_listener(&server_fd, &address);
  if (listened == -1) {
    printf("The server could not listen to the port. Aborting...");
    return -1;
  }

  return 0;
}
