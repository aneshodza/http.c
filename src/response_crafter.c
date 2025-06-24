#include <response_crafter.h>

char *craft_response(char *header, char *content_type, char *connection,
                     char *content, int append_body) {
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
  if (append_body) offset += sprintf(response + offset, "%s", content);

  return response;
}
