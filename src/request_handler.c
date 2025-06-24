#include <request_handler.h>

int handle_request(int client_fd) {
  char buffer[4096] = {0};

  read(client_fd, buffer, sizeof(buffer) - 1);
  buffer[4095] = '\0';

  HttpRequest req = {0};
  int result = parse_request(buffer, &req);
  (void)print_request(&req);

  char *response;

  if (req.method != GET && req.method != HEAD) {
    response = method_not_allowed();
  } else {
    char path[MAX_PATH];
    int file_result = get_full_path(req.path, req.accept, path);

    if (result == -1) {
      response = bad_request();
    } else if (result == -2) {
      response = unsupported_encoding();
    } else if (file_result == -2) {
      response = not_found();
    } else {
      int append_body = (req.method == GET);
      response = success_response(path, append_body);
      if (response == ERROR_500) {
        response = internal_server_error();
      }
    }
  }

  send(client_fd, response, strlen(response), 0);
  close(client_fd);

  free(response);

  return 0;
}

