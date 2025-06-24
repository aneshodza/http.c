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

  if (strcmp(accept_header, "*/*") == 0) {
    for (idx = 0; idx < sizeof(supported_types) / sizeof(MimeMapping); ++idx) {
      if (strcmp(supported_types[idx].mime, "*/*") == 0) continue;

      strncpy(extensions[count], supported_types[idx].extension, MAX_EXTENSION - 1);
      extensions[count][MAX_EXTENSION - 1] = '\0';
      count++;

      if (count == MAX_EXTENSIONS) break;
    }
    return count;
  }

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

int get_mime(const char *path, char *mime) {
  const char *ext = strrchr(path, '.');
  if (!ext) {
    strcpy(mime, FALLBACK_MIME);
    return 0;
  }

  unsigned long idx = 0;
  while (idx < sizeof(supported_types) / sizeof(MimeMapping)) {
    if (strcmp(ext, supported_types[idx].extension) == 0) {
      strcpy(mime, supported_types[idx].mime);
      return 0;
    }
    idx++;
  }

  strcpy(mime, FALLBACK_MIME);
  return 0;
}
