#include <error_responder.h>

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

char *fetch_error(char *path) {
  struct stat path_stat;
  if (stat(path, &path_stat) == 0 && S_ISREG(path_stat.st_mode)) {
    FILE *response_fd = fopen(path, "r");
    if (!response_fd) {
      perror("opening error_page");
      return READ_FILE_FAILED;
    }

    fseek(response_fd, 0, SEEK_END);
    long file_size = ftell(response_fd);
    rewind(response_fd);

    char *buffer = malloc(file_size + 1);
    if (!buffer) {
      perror("allocating buffer");
      fclose(response_fd);
      return READ_FILE_FAILED;
    }
    size_t bytes_read = fread(buffer, 1, file_size, response_fd);
    if (bytes_read != file_size) {
      perror("reading full file");
      free(buffer);
      fclose(response_fd);
      return READ_FILE_FAILED;
    }
    buffer[file_size] = '\0';
    fclose(response_fd);

    return buffer;
  } else {
    return USE_DEFAULT_FILE;
  }
}

char *build_response(char *header, char *content_type, char *connection,
                     char *content) {
  int content_length = strlen(content);

  char length_str[MAX_LENGTH_DIGITS + 1];
  int length_digits =
      snprintf(length_str, sizeof(length_str), "%d", content_length);

  int full_length =
      strlen(header) + strlen("Content-Type: ") + strlen(content_type) +
      strlen("\r\n") + strlen("Content-Length: ") + length_digits +
      strlen("\r\n") + strlen("Connection: ") + strlen(connection) +
      strlen("\r\n") + strlen("\r\n") + content_length;

  char *response = malloc(full_length);
  if (!response)
    return BUILD_REPONSE_FAILED;

  size_t offset = 0;
  offset += sprintf(response + offset, "%s", header);
  offset += sprintf(response + offset, "Content-Type: %s\r\n", content_type);
  offset += sprintf(response + offset, "Content-Length: %s\r\n", length_str);
  offset += sprintf(response + offset, "Connection: %s\r\n", connection);
  offset += sprintf(response + offset, "\r\n");
  offset += sprintf(response + offset, "%s", content);

  return response;
}

char *build_error_response(const char *header,
                        const char *default_body, const char *error_file_path) {
  int should_free = 1;
  char *file_buf = fetch_error((char *)error_file_path);

  if (file_buf == READ_FILE_FAILED || file_buf == USE_DEFAULT_FILE) {
    file_buf = (char *)default_body;
    should_free = 0;
  }

  char *content_type = "text/html";
  char *connection = "close";
  char *response = build_response((char *)header, (char *)content_type,
                                  (char *)connection, file_buf);

  if (should_free) {
    free(file_buf);
  }

  return response;
}
