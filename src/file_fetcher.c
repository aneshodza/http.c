#include <file_fetcher.h>

int get_full_path(const char *requested_path, const char *accept_header, char *full_path) {
  if (!requested_path || !accept_header || !full_path) return -1;
  if (contains_dir_traversal(requested_path)) return -1;

  char extensions[MAX_EXTENSIONS][MAX_EXTENSION] = {0};
  get_file_extensions(accept_header, extensions);

  char appended_path[MAX_PATH];
  strncpy(appended_path, requested_path, MAX_PATH);
  append_index(appended_path);

  int found = find_matching_file(requested_path, extensions, full_path);
  if (found == -1) {
    found = find_matching_file(appended_path, extensions, full_path);
    if (found == -1) return -2;
  }

  return 0;
}

int contains_dir_traversal(const char *path) {
  return strstr(path, "..") != NULL;
}

int append_index(char *path) {
  int idx = 0;
  while (path[idx]) {
    idx++;
  }

  if (path[idx - 1] != '/') {
    path[idx] = '/';
    idx++;
  }

  char *append = "index";
  while (*append) {
    path[idx] = *append;
    append++;
    idx++;
  }

  return 0;
}

int find_matching_file(const char *requested_path, const char extensions[MAX_EXTENSIONS][MAX_EXTENSION], char *resolved_path) {
  int end_of_path = 0;
  while (requested_path[end_of_path]) end_of_path++;

  int checked_extensions = 0;
  while (checked_extensions < MAX_EXTENSIONS) {
    char new_try[MAX_PATH] = {0};
    int end_of_try = 0;
    while(BASE_PATH[end_of_try]) {
      new_try[end_of_try] = BASE_PATH[end_of_try];
      end_of_try++;
    }

    int requested_path_offset = 0;
    while (requested_path[requested_path_offset]) {
      new_try[end_of_try] = requested_path[requested_path_offset];
      end_of_try++;
      requested_path_offset++;
    }

    int extension_inc = 0;
    while (extensions[checked_extensions][extension_inc]) {
      new_try[end_of_try + extension_inc] = extensions[checked_extensions][extension_inc];
      extension_inc++;
    }

    struct stat path_stat;
    if (stat(new_try, &path_stat) == 0 && S_ISREG(path_stat.st_mode)) {
      strncpy(resolved_path, new_try, MAX_PATH);
      return 0;
    } 

    checked_extensions++;
  }

  return -1;
}
