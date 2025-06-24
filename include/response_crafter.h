#ifndef RESPONSE_CRAFTER_H

#define RESPONSE_CRAFTER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <zlib.h>

#include <http_request.h>

#define MAX_LENGTH_DIGITS      20
#define BUILD_REPONSE_FAILED   ((char *)-1)

char *craft_response(char *header, char *content_type, char *connection, char *content, int append_body, Encoding encoding, int *response_len);
int gzip_compress(const char *input, int input_len, char **output, int *output_len);


#endif


