#ifndef SERVER_H

#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>

#include <request_handler.h>

#define PORT 8080
#define PORT_INCREMENTS 100
#define MAX_WORKERS 4
#define MAX_QUEUE 1024

int spawn_server(int *server_fd, struct sockaddr_in *address);
int create_threadpool();
int spawn_listener(int *server_fd, struct sockaddr_in *address);
void* work_queue(void* args);

#endif

