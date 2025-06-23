#ifndef SUCCESS_RESPONDER_H

#define SUCCESS_RESPONDER_H

#define ERROR_500 ((char *)-1)

#include <stdio.h>

#include <file_reader.h>
#include <mime.h>
#include <response_crafter.h>

char *success_response(const char *path);

#endif


