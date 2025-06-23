#ifndef FILE_READER_H

#define FILE_READER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

char *read_file(char *path, char *fallback, char *read_error);

#endif


