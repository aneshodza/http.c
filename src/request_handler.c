#include <request_handler.h>

const char* response =
  "HTTP/1.1 200 OK\r\n"
  "Content-Type: text/html\r\n"
  "Content-Length: 47\r\n"
  "Connection: close\r\n"
  "\r\n"
  "<html><body><h1>Hello, World!</h1></body></html>";

int handle_request(int client_fd) {
  char buffer[4096] = {0};

  read(client_fd, buffer, sizeof(buffer) - 1);
  buffer[4095] = '\0';

  HttpRequest req = {0};
  int result = parse_request(buffer, &req);
  (void)print_request(&req);

  char path[MAX_PATH];
  int file_result = get_full_path(req.path, req.accept, path);
  char *response;

  if (result == -1) {
    response = bad_request();
  } else if (result == -2) {
    response = unsupported_encoding();
  } else if (file_result == -2) {
    response = not_found();
  } else {
    response = strdup("test123");
  }
  send(client_fd, response, strlen(response), 0);
  close(client_fd);

  free(response);

  return 0;
}

