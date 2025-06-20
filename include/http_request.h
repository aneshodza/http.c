#ifndef HTTP_REQUEST_H

#define HTTP_REQUEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

#define PATH_LENGTH 128
#define VERSION_LENGTH 16
#define ACCEPT_LENGTH 256
#define ACCEPT_ENCODING_LENGTH 256

typedef enum {
  INVALID_METHOD = -1,
  GET,
  POST,
  HEAD,
  PUT,
  DELETE,
  OPTIONS
} Method;

typedef enum {
  UNSUPPORTED = -1,
  NONE,
  GZIP
} Encoding;

typedef struct {
  Method method;
  char path[PATH_LENGTH];
  char http_version[VERSION_LENGTH];
  char accept[ACCEPT_LENGTH];
  Encoding accept_encoding;
} HttpRequest;

int parse_request(const char *raw, HttpRequest *req);
void skip_lines(const char **raw, int skip_amount);
Method get_method(const char **raw);
int get_path(const char **raw, char *path);
int get_http_version(const char **raw, char *version);
int get_accept(const char *raw, char *accept);
Encoding get_accept_encoding(const char *raw);

void print_request(HttpRequest *req);
const char* method_to_string(Method method);
const char* encoding_to_string(Encoding encoding);

#endif

