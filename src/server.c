#include <server.h>

const char* response =
  "HTTP/1.1 200 OK\r\n"
  "Content-Type: text/html\r\n"
  "Content-Length: 47\r\n"
  "Connection: close\r\n"
  "\r\n"
  "<html><body><h1>Hello, World!</h1></body></html>";

int spawn_server(int *server_fd, struct sockaddr_in *address) {
  *server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == 0) {
    perror("socket failed");
    return -1;
  }

  address->sin_family = AF_INET;
  address->sin_addr.s_addr = INADDR_ANY;
  address->sin_port = htons(PORT);

  if (bind(*server_fd, (struct sockaddr *)address, sizeof(*address)) < 0) {
    perror("bind failed");
    return -1;
  }

  if (listen(*server_fd, 10) < 0) {
    perror("listen");
    return -1;
  }

  return PORT;
}

int spawn_listener(int *server_fd, struct sockaddr_in *address) {
  socklen_t addrlen = sizeof(*address);
  char buffer[4096] = {0};

  while (1) {
    int new_socket = accept(*server_fd, (struct sockaddr *)address, &addrlen);
    if (new_socket < 0) {
      perror("accept");
      return -1;
    }

    read(new_socket, buffer, sizeof(buffer) - 1);
    printf("Request:\n%s\n", buffer);

    send(new_socket, response, strlen(response), 0);
    close(new_socket);
  }

  return 0;
}
