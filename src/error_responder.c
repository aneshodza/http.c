#include <error_responder.h>

char *internal_server_error() {
  return build_error_response(
      "HTTP/1.1 500 Internal Server Error\r\n",
      "<html><body><h1>500 - Internal Server Error</h1></body></html>",
      BASE_PATH ERROR_PATH "/500.html");
}

char *bad_request() {
  return build_error_response(
      "HTTP/1.1 400 Bad Request\r\n",
      "<html><body><h1>400 - Bad Request</h1></body></html>",
      BASE_PATH ERROR_PATH "/400.html");
}

char *unsupported_encoding() {
  return build_error_response(
      "HTTP/1.1 406 Not Acceptable\r\n",
      "<html><body><h1>406 - Not Acceptable</h1></body></html>",
      BASE_PATH ERROR_PATH "/406.html");
}

char *not_found() {
  return build_error_response(
      "HTTP/1.1 404 Not Found\r\n",
      "<html><body><h1>404 - File Not Found</h1></body></html>",
      BASE_PATH ERROR_PATH "/404.html");
}

char *build_error_response(const char *header,
                        const char *default_body, const char *error_file_path) {
  int should_free = 1;
  char *file_buf = read_file((char *)error_file_path, USE_DEFAULT_FILE, READ_FILE_FAILED);

  if (file_buf == READ_FILE_FAILED || file_buf == USE_DEFAULT_FILE) {
    file_buf = (char *)default_body;
    should_free = 0;
  }

  char *content_type = "text/html";
  char *connection = "close";
  char *response = craft_response((char *)header, (char *)content_type,
                                  (char *)connection, file_buf);

  if (should_free) {
    free(file_buf);
  }

  return response;
}
