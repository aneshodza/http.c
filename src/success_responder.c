#include <success_responder.h>

char *success_header = "HTTP/1.1 200 OK\r\n";

char *success_response(const char *path, int append_body, Encoding encoding, int *response_len) {
  char mime[MAX_MIME_LENGTH];
  get_mime(path, mime);

  if (!should_gzip(mime)) {
    encoding = NONE;
  }

  char *file_buf = read_file((char *)path, ERROR_500, ERROR_500);
  if (file_buf == ERROR_500) return ERROR_500;

  char *connection = "close";
  char *response = craft_response(success_header, mime, connection, file_buf, append_body, encoding, response_len);

  free(file_buf);
  return response;
}

int should_gzip(const char *mime) {
  return (strstr(mime, "text/") != NULL || strstr(mime, "application/") != NULL);
}
