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

  if (get_accept(raw, req->accept) == -1) {
    perror("accept");
    return -1;
  }

  Encoding encoding = get_accept_encoding(raw);
  req->accept_encoding = encoding;
  if (encoding == UNSUPPORTED) return -2;

  return 0;
}

Method get_method(const char **raw) {
  char method[10] = {0};
  int idx = 0;
  while ((*raw)[idx] != ' ' && (*raw)[idx] != '\0' && idx < 9) {
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

  /* Throw out URL parameters by changing any ? to \0 */
  int param_search = idx;
  while (param_search > 0) {
    if (path[param_search] == '?') {
      path[param_search] = '\0';
    }
    param_search--;
  }

  *raw = *raw + idx + 1;

  return 0;
}

int get_http_version(const char **raw, char *version) {
  int idx = 0;

  while ((*raw)[idx] != '\n' && (*raw)[idx] != '\r') {
    version[idx] = (*raw)[idx];
    idx++;
  }
  version[idx] = '\0';
  *raw = *raw + idx + 1;

  return 0;
}

int get_accept(const char *raw, char *accept) {
  char *accept_header = "Accept: ";
  char *start = strstr(raw, accept_header);

  if (start == NULL) {
    accept[0] = '*';
    accept[1] = '/';
    accept[2] = '*';
    accept[3] = '\0';
    return 0;
  }

  int idx = 0;
  start += strlen(accept_header);
  while (start[idx] != '\n' && start[idx] != '\r') {
    accept[idx] = start[idx];
    idx++;
  }
  return 0;
}

Encoding get_accept_encoding(const char *raw) {
  char *accept_header = "Accept-Encoding: ";
  char *start = strstr(raw, accept_header);

  if (start == NULL) {
    return NONE;
  }

  char accept_encoding[ACCEPT_ENCODING_LENGTH];
  int idx = 0;
  while (start[idx] != '\n' && start[idx] != '\r') {
    accept_encoding[idx] = start[idx];
    idx++;
  }

  if (strstr(accept_encoding, "gzip") != NULL) {
    return GZIP;
  }

  return UNSUPPORTED;
}

void print_request(HttpRequest *req) {
  printf("\nIncoming request: \n");
  printf("Method:             %s\n", method_to_string(req->method));
  printf("Path:               %s\n", req->path);
  printf("HTTP Version:       %s\n", req->http_version);
  printf("Accept:             %s\n", req->accept);
  printf("Accept-Encoding:    %s\n", encoding_to_string(req->accept_encoding));
  fflush(stdout);
}

const char* method_to_string(Method method) {
  switch (method) {
    case GET:
      return "GET";
    case POST:
      return "POST";
    case HEAD:
      return "HEAD";
    case PUT:
      return "PUT";
    case DELETE:
      return "DELETE";
    case OPTIONS:
      return "OPTIONS";
    case INVALID_METHOD:
    default:
      return "INVALID_METHOD";
  }
}

const char* encoding_to_string(Encoding encoding) {
  switch (encoding) {
    case NONE:
      return "NONE";
    case GZIP:
      return "GZIP";
    case UNSUPPORTED:
    default:
      return "UNSUPPORTED";
  }

}
