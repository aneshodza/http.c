#include <CUnit/CUnit.h>
#include <response_crafter.h>

void test_craft_response() {
  char *header = "HEADER";
  char *content_type = "test/case";
  char *connection = "something";
  char *content = "<h1>This is a test</h1>";

  char *response = craft_response(header, content_type, connection, content, 1, 0, NULL);
  CU_ASSERT_STRING_EQUAL(response, "HEADERContent-Type: test/case\r\nContent-Length: 23\r\nConnection: something\r\n\r\n<h1>This is a test</h1>");
}

void test_craft_response_no_body() {
  char *header = "HEADER";
  char *content_type = "test/case";
  char *connection = "something";
  char *content = "<h1>This is a test</h1>";
  char *response = craft_response(header, content_type, connection, content, 0, 0, NULL);
  CU_ASSERT_STRING_EQUAL(response, "HEADERContent-Type: test/case\r\nContent-Length: 23\r\nConnection: something\r\n\r\n");
}

void test_gzip_response() {
  char *header = "HEADER";
  char *content_type = "text/plain";
  char *connection = "close";
  char *content = "This is a test string that will be compressed using gzip.";
  int response_len = 0;

  char *response = craft_response(header, content_type, connection, content, 1, GZIP, &response_len);
  printf("Reponse: %s\n", response);
  char *body = strstr(response, "\r\n\r\n");

  CU_ASSERT_PTR_NOT_NULL(body);

  body += 4; 
  unsigned char *bytes = (unsigned char *)body;

  CU_ASSERT(bytes[0] == 0x1f && bytes[1] == 0x8b);
  CU_ASSERT_PTR_NOT_NULL(strstr(response, "Content-Encoding: gzip"));
}

void setup_test_response_crafter(CU_pSuite *suite) {
  CU_add_test(*suite, "Properly crafts the reponse with all details", test_craft_response);
  CU_add_test(*suite, "Properly crafts the reponse with no body", test_craft_response_no_body);
  CU_add_test(*suite, "Properly crafts the reponse with gzip encoding", test_gzip_response);
}
