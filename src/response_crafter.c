#include <response_crafter.h>

char *craft_response(char *header, char *content_type, char *connection,
                     char *content, int append_body, Encoding encoding,
                     int *response_len) {
  int content_length = strlen(content);
  char compression[64] = {0};

  if (encoding == GZIP) {
    char *gzipped = NULL;
    int gzipped_len = 0;

    if (gzip_compress(content, content_length, &gzipped, &gzipped_len)) {
      content = gzipped;
      content_length = gzipped_len;
      strcpy(compression, "Content-Encoding: gzip\r\n");
    }
  }
  char length_str[MAX_LENGTH_DIGITS + 1];
  int length_digits =
      snprintf(length_str, sizeof(length_str), "%d", content_length);

  int full_length =
      strlen(header) + strlen("Content-Type: ") + strlen(content_type) +
      strlen(compression) +
      strlen("\r\n") + strlen("Content-Length: ") + length_digits +
      strlen("\r\n") + strlen("Connection: ") + strlen(connection) +
      strlen("\r\n") + strlen("\r\n") + content_length;

  if (response_len != NULL) *response_len = full_length;
  char *response = malloc(full_length);
  if (!response)
    return BUILD_REPONSE_FAILED;

  size_t offset = 0;
  offset += sprintf(response + offset, "%s", header);
  offset += sprintf(response + offset, "Content-Type: %s\r\n", content_type);
  offset += sprintf(response + offset, "Content-Length: %s\r\n", length_str);
  offset += sprintf(response + offset, "Connection: %s\r\n", connection);
  offset += sprintf(response + offset, "%s", compression);
  offset += sprintf(response + offset, "\r\n");
  if (append_body) {
    memcpy(response + offset, content, content_length);
    offset += content_length;
  }

  return response;
}

int gzip_compress(const char *input, int input_len, char **output, int *output_len) {
    z_stream stream;
    memset(&stream, 0, sizeof(stream));

    if (deflateInit2(&stream, Z_BEST_COMPRESSION, Z_DEFLATED,
                     15 + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        return 0;
    }

    size_t buffer_size = compressBound(input_len);
    char *gzipped = malloc(buffer_size);
    if (!gzipped) {
        deflateEnd(&stream);
        return 0;
    }

    stream.next_in = (Bytef *)input;
    stream.avail_in = input_len;
    stream.next_out = (Bytef *)gzipped;
    stream.avail_out = buffer_size;

    while (1) {
        int ret = deflate(&stream, Z_FINISH);
        if (ret == Z_STREAM_END) {
            break;
        } else if (ret == Z_OK) {
            size_t used = stream.total_out;
            buffer_size *= 2;
            char *new_buf = realloc(gzipped, buffer_size);
            if (!new_buf) {
                free(gzipped);
                deflateEnd(&stream);
                return 0;
            }
            gzipped = new_buf;
            stream.next_out = (Bytef *)(gzipped + used);
            stream.avail_out = buffer_size - used;
        } else {
            free(gzipped);
            deflateEnd(&stream);
            return 0;
        }
    }

    *output = gzipped;
    *output_len = stream.total_out;
    deflateEnd(&stream);
    return 1;
}
