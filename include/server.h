#ifndef SERVER_H

#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8080

int spawn_server(int *server_fd, struct sockaddr_in *address);
int spawn_listener(int *server_fd, struct sockaddr_in *address);

#endif

