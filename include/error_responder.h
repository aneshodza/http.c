#ifndef RESPONDER_H

#define RESPONDER_H

#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/stat.h>

#include <file_fetcher.h>

#define READ_FILE_FAILED       ((char *)-1)
#define USE_DEFAULT_FILE       ((char *)-2)
#define BUILD_REPONSE_FAILED   ((char *)-1)
#define MAX_LENGTH_DIGITS      20

char *bad_request();
char *unsupported_encoding();
char *not_found();

char *fetch_error(char *path);
char *build_response(char *header, char *content_type, char *connection, char *content);
char *build_error_response(const char *header, const char *default_body, const char *error_file_path);

#endif

