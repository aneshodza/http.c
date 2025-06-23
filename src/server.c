#include <server.h>

pthread_t workers[MAX_WORKERS] = {0};
int job_queue[MAX_QUEUE] = {0};
int queue_start = 0, queue_end = 0;
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_not_empty = PTHREAD_COND_INITIALIZER;
int waiting_workers = 0;

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

int create_threadpool() {
  int idx = 0;
  while (idx < MAX_WORKERS) {
    int thread = pthread_create(&workers[idx], NULL, work_queue, NULL);
    if (thread != 0) {
      perror("pthread_create");
      return -1;
    }
    idx++;
  }
  return 0;
}

int spawn_listener(int *server_fd, struct sockaddr_in *address) {
  socklen_t addrlen = sizeof(*address);

  while (1) {
    int client_fd = accept(*server_fd, (struct sockaddr *)address, &addrlen);
    if (client_fd < 0) {
      perror("accept");
      continue;
    }

    pthread_mutex_lock(&queue_mutex);
    job_queue[queue_end] = client_fd;
    queue_end = (queue_end + 1) % MAX_QUEUE;
    pthread_cond_signal(&queue_not_empty);
    pthread_mutex_unlock(&queue_mutex);
  }
}

void* work_queue() {
  while (1) {
    int client_fd;

    pthread_mutex_lock(&queue_mutex);

    waiting_workers++;

    while (queue_start == queue_end) {
      printf("\r[Queue: %d] [Waiting workers: %d]    ", 
             (queue_end - queue_start + MAX_QUEUE) % MAX_QUEUE, waiting_workers);
      fflush(stdout);
      pthread_cond_wait(&queue_not_empty, &queue_mutex);
    }

    waiting_workers --;

    client_fd = job_queue[queue_start];
    queue_start = (queue_start + 1) % MAX_QUEUE;

    printf("\r[Queue: %d] [Waiting workers: %d]    ", 
           (queue_end - queue_start + MAX_QUEUE) % MAX_QUEUE, waiting_workers);
    fflush(stdout);

    pthread_mutex_unlock(&queue_mutex);

    handle_request(client_fd);
    close(client_fd);
  }

  return NULL;
}
