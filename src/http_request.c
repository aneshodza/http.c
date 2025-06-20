#include <http_request.h>

int parse_request(const char *raw, HttpRequest *req) {
  Method method = get_method(&raw);
  if (method == INVALID_METHOD) {
    perror("method");
    return -1;
  }
  req->method = method;

  if (get_path(&raw, req->path) == -1) {
    perror("path");
    return -1;
  }

  if (get_http_version(&raw, req->http_version) == -1) {
    perror("version");
    return -1;
  }

  skip_lines(&raw, 2);

  if (get_accept(&raw, req->accept) == -1) {
    perror("accept");
    return -1;
  }

  skip_lines(&raw, 1);

  int gzip = check_gzip(&raw);
  if (gzip < 0) {
    perror("gzip");
    return gzip;
  }

  return 0;
}

void skip_lines(const char **raw, int skip_amount) {
  while (**raw != '\n' && **raw != '\0') (*raw)++;

  if (**raw == '\n') (*raw)++;

  if (skip_amount > 1) {
    skip_lines(raw, skip_amount - 1);
  }
}

Method get_method(const char **raw) {
  char method[10] = {0};
  int idx = 0;
  while ((*raw)[idx] != ' ') {
    method[idx] = (*raw)[idx];
    idx++;
  }
  method[idx] = '\0';
  *raw = *raw + idx + 1;

  if (strcmp(method, "GET") == 0) return GET;
  if (strcmp(method, "POST") == 0) return POST;
  if (strcmp(method, "HEAD") == 0) return HEAD;
  if (strcmp(method, "PUT") == 0) return PUT;
  if (strcmp(method, "DELETE") == 0) return DELETE;
  if (strcmp(method, "OPTIONS") == 0) return OPTIONS;

  return INVALID_METHOD;
}

int get_path(const char **raw, char *path) {
  int idx = 0;
  if ((*raw)[0] != '/') {
    return -1;
  }

  while ((*raw)[idx] != ' ') {
    path[idx] = (*raw)[idx];
    idx++;
  }
  path[idx] = '\0';
  *raw = *raw + idx + 1;

  return 0;
}

int get_http_version(const char **raw, char *version) {
  int idx = 0;

  while ((*raw)[idx] != '\n') {
    version[idx] = (*raw)[idx];
    idx++;
  }
  version[idx] = '\0';
  *raw = *raw + idx + 1;

  return 0;
}

int get_accept(const char **raw, char *accept) {
  char accept_preamble[] = "Accept: ";
  int accept_size = strlen(accept_preamble);
  int idx = 0;
  while (idx < accept_size) {
    if ((*raw)[idx] != accept_preamble[idx]) return -1;

    idx++;
  }

  while ((*raw)[idx] != '\n') {
    accept[idx - accept_size] = (*raw)[idx];
    idx++;
  }
  accept[idx - accept_size] = '\0';
  *raw = *raw + idx + 1;

  return 0;
}

int check_gzip(const char **raw) {
  char accept_encoding_preamble[] = "Accept-Encoding: ";
  int accept_size = strlen(accept_encoding_preamble);
  int idx = 0;
  while (idx < accept_size) {
    if ((*raw)[idx] != accept_encoding_preamble[idx]) return -1;

    idx++;
  }

  while ((*raw)[idx] != '\n' && (*raw)[idx] != '\0') {
    if (strncmp(&(*raw)[idx], "gzip", 4) == 0) {
      return 0;
    }
    idx++;
  }

  return -2;
}
