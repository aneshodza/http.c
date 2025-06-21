#ifndef MIME_H

#define MIME_H

#define MAX_EXTENSIONS 16
#define MAX_EXTENSION 6

#include <string.h>

typedef struct {
    const char *extension;
    const char *mime;
} MimeMapping;

int get_file_extensions(const char *accept_header, char extensions[MAX_EXTENSIONS][MAX_EXTENSION]);

#endif

