#ifndef FILE_FETCHER_H

#define FILE_FETCHER_H

#ifdef TEST_ENV
#define BASE_PATH "./public_test"
#else
#define BASE_PATH "./public"
#endif

#define ERROR_PATH "/error_pages"
#define MAX_PATH 256

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#include <mime.h>

int get_full_path(const char *requested_path, const char *accept_header, char *full_path);
int contains_dir_traversal(const char *path);
int append_index(char *path);
int find_matching_file(const char *requested_path, const char extensions[MAX_EXTENSIONS][MAX_EXTENSION], char *resolved_path);

#endif

