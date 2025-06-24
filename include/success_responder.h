#ifndef SUCCESS_RESPONDER_H

#define SUCCESS_RESPONDER_H

#define ERROR_500 ((char *)-1)

#include <stdio.h>

#include <file_reader.h>
#include <mime.h>
#include <response_crafter.h>
#include <http_request.h>

char *success_response(const char *path, int append_body, Encoding encoding, int *response_len);
int should_gzip(const char *mime);

#endif


