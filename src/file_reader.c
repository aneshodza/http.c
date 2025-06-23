#include <file_reader.h>

char *read_file(char *path, char *fallback, char *read_error) {
  struct stat path_stat;
  if (stat(path, &path_stat) == 0 && S_ISREG(path_stat.st_mode)) {
    FILE *response_fd = fopen(path, "r");
    if (!response_fd) {
      perror("opening error_page");
      return read_error;
    }

    fseek(response_fd, 0, SEEK_END);
    long file_size = ftell(response_fd);
    rewind(response_fd);

    char *buffer = malloc(file_size + 1);
    if (!buffer) {
      perror("allocating buffer");
      fclose(response_fd);
      return read_error;
    }
    size_t bytes_read = fread(buffer, 1, file_size, response_fd);
    if (bytes_read != (size_t) file_size) {
      perror("reading full file");
      free(buffer);
      fclose(response_fd);
      return read_error;
    }
    buffer[file_size] = '\0';
    fclose(response_fd);

    return buffer;
  } else {
    return fallback;
  }
}
