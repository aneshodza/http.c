#ifndef REQUEST_HANDLER_H

#define REQUEST_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#include <error_responder.h>
#include <http_request.h>
#include <file_fetcher.h>

int handle_request(int client_fd);

#endif

