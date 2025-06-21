#include <mime.h>

MimeMapping supported_types[] = {
  {".html", "text/html"},
  {".htm", "text/html"},
  {".txt", "text/plain"},
  {".json", "application/json"},
  {".css", "text/css"},
  {".js", "application/javascript"},
  {".jpg", "image/jpeg"},
  {".png", "image/png"},
  {".ico", "image/x-icon"},
  {"", "*/*"}
};

int get_file_extensions(const char *accept_header, char extensions[MAX_EXTENSIONS][MAX_EXTENSION]) {
  unsigned long idx = 0;
  int count = 0;

  while (idx < sizeof(supported_types) / sizeof(MimeMapping)) {
    if (strstr(accept_header, supported_types[idx].mime) != NULL) {
      strncpy(extensions[count], supported_types[idx].extension, MAX_EXTENSION - 1);
      extensions[count][MAX_EXTENSION - 1] = '\0';
      count++;

      if (count == MAX_EXTENSIONS) break;
    }
    idx++;

  }

  return count;
}
