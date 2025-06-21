#include <error_responder.h>
#include <CUnit/CUnit.h>
#include <http_request.h>

void test_file_used() {
  char *response = not_found();

  CU_ASSERT_TRUE(strstr(response, "404 returned") != NULL);
}

void test_fallback() {
  char *response = bad_request();

  CU_ASSERT_TRUE(strstr(response, "<html><body><h1>400 - Bad Request</h1></body></html>") != NULL);
}

void test_content_length() {
  char *response = bad_request();

  CU_ASSERT_TRUE(strstr(response, "Content-Length: 52") != NULL);
}

void test_close_connection_on_error() {
  char *response = bad_request();

  CU_ASSERT_TRUE(strstr(response, "Connection: close") != NULL);
}

void test_type_text_html_on_error() {
  char *response = bad_request();

  CU_ASSERT_TRUE(strstr(response, "Content-Type: text/html") != NULL);
}

void setup_test_error_responder(CU_pSuite *suite) {
  CU_add_test(*suite, "File is used for error if available", test_file_used);
  CU_add_test(*suite, "Fallback is used for error if no file is available", test_fallback);
  CU_add_test(*suite, "Properly fills the content-length field", test_content_length);
  CU_add_test(*suite, "Tells connection close on error", test_close_connection_on_error);
  CU_add_test(*suite, "Tells Content-Type text/html on error", test_type_text_html_on_error);
}
