#ifndef RESPONSE_CRAFTER_H

#define RESPONSE_CRAFTER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/stat.h>

#define MAX_LENGTH_DIGITS      20
#define BUILD_REPONSE_FAILED   ((char *)-1)

char *craft_response(char *header, char *content_type, char *connection, char *content);

#endif


