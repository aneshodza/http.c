#include <CUnit/CUnit.h>
#include <success_responder.h>

void test_success_response() {
  char *path = "./public_test/index.html";
  char *response = success_response(path);

  CU_ASSERT_STRING_EQUAL(response, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 30\r\nConnection: close\r\n\r\n<h1>Test File index.html</h1>\n");
}

void test_alternative_mimes() {
  char *path = "./public_test/index.css";
  char *response = success_response(path);

  CU_ASSERT_PTR_NOT_NULL(strstr(response, "Content-Type: text/css"));
}

void test_500_on_file_not_found() {
  char *path = "./public_test/some_nonexistent_file";
  char *response = success_response(path);

  CU_ASSERT_PTR_EQUAL(response, ERROR_500);
}

void setup_test_success_responder(CU_pSuite *suite) {
  CU_add_test(*suite, "The success response has all the correct details", test_success_response);
  CU_add_test(*suite, "Success responses also work with different mimes", test_alternative_mimes);
  CU_add_test(*suite, "The server should give 500 when something goes wrong here", test_500_on_file_not_found);
}

