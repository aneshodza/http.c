#include <server.h>

int spawn_server(int *server_fd, struct sockaddr_in *address) {
  *server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (*server_fd < 0) {
    perror("socket failed");
    return -1;
  }

  address->sin_family = AF_INET;
  address->sin_addr.s_addr = INADDR_ANY;

  int port = PORT;
  int i = 0;
  while (i < PORT_INCREMENTS) {
    address->sin_port = htons(port);

    if (bind(*server_fd, (struct sockaddr *)address, sizeof(*address)) == 0) {
      if (listen(*server_fd, 10) < 0) {
        perror("listen");
        return -1;
      }
      return port;
    }

    if (errno == EADDRINUSE) {
      port++;
    } else {
      perror("bind failed");
      return -1;
    }

    i++;
  }

  perror("bind failed in range");
  return -1;
}

int spawn_listener(int *server_fd, struct sockaddr_in *address) {
  socklen_t addrlen = sizeof(*address);
  pthread_t threads[MAX_THREADS] = {0};
  int thread_count = 0;

  while (1) {
    int *client_fd = malloc(sizeof(int));
    *client_fd = accept(*server_fd, (struct sockaddr *)address, &addrlen);
    if (*client_fd < 0) {
      perror("accept");
      return -1;
    }

    pthread_create(&threads[thread_count], NULL, handle_request, client_fd);
    thread_count = (thread_count + 1) % MAX_THREADS;
  }

  return 0;
}
