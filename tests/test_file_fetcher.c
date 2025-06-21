#include <CUnit/CUnit.h>
#include <file_fetcher.h>

void test_find_index_page() {
  const char *requested_path = "/";
  const char *accept_header = "text/html";

  char full_path[MAX_PATH];
  get_full_path(requested_path, accept_header, full_path);
  CU_ASSERT_STRING_EQUAL(full_path, BASE_PATH "/index.html")
}

void test_find_subdir_index_page() {
  const char *requested_path = "/test";
  const char *accept_header = "text/html";

  char full_path[MAX_PATH];
  get_full_path(requested_path, accept_header, full_path);
  CU_ASSERT_STRING_EQUAL(full_path, BASE_PATH "/test/index.html")
}

void test_find_css_file() {
  const char *requested_path = "/";
  const char *accept_header = "text/css";

  char full_path[MAX_PATH];
  get_full_path(requested_path, accept_header, full_path);
  CU_ASSERT_STRING_EQUAL(full_path, BASE_PATH "/index.css")
}

void test_tells_when_no_file_is_found() {
  const char *requested_path = "/";
  const char *accept_header = "image/x-icon";

  char full_path[MAX_PATH];
  int found = get_full_path(requested_path, accept_header, full_path);
  CU_ASSERT_EQUAL(found, -2);
}

void test_disallows_double_dot() {
  const char *requested_path = "/../";
  const char *accept_header = "text/html";

  char full_path[MAX_PATH];
  int found = get_full_path(requested_path, accept_header, full_path);
  CU_ASSERT_EQUAL(found, -1);

}

void setup_test_file_fetcher(CU_pSuite *suite) {
  CU_add_test(*suite, "Finds index page", test_find_index_page);
  CU_add_test(*suite, "Finds index page of subdir", test_find_subdir_index_page);
  CU_add_test(*suite, "Finds css files", test_find_css_file);
  CU_add_test(*suite, "Tells when a file is not found", test_tells_when_no_file_is_found);
  CU_add_test(*suite, "Disallows double dot", test_disallows_double_dot);
}
