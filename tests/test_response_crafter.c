#include <CUnit/CUnit.h>
#include <response_crafter.h>

void test_craft_response() {
  char *header = "HEADER";
  char *content_type = "test/case";
  char *connection = "something";
  char *content = "<h1>This is a test</h1>";

  char *response = craft_response(header, content_type, connection, content);
  CU_ASSERT_STRING_EQUAL(response, "HEADERContent-Type: test/case\r\nContent-Length: 23\r\nConnection: something\r\n\r\n<h1>This is a test</h1>");
}

void setup_test_response_crafter(CU_pSuite *suite) {
  CU_add_test(*suite, "Properly crafts the reponse with all details", test_craft_response);
}
