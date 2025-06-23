#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

/* All test files have their setup listed here */
void setup_test_http_request(CU_pSuite *suite);
void setup_test_file_fetcher(CU_pSuite *suite);
void setup_test_error_responder(CU_pSuite *suite);
void setup_test_file_reader(CU_pSuite *suite);
void setup_test_response_crafter(CU_pSuite *suite);
void setup_test_success_responder(CU_pSuite *suite);

int main() {
  CU_initialize_registry();

  CU_pSuite suite = CU_add_suite("HTTP Request Suite", NULL, NULL);
  setup_test_http_request(&suite);
  setup_test_file_fetcher(&suite);
  setup_test_error_responder(&suite);
  setup_test_file_reader(&suite);
  setup_test_response_crafter(&suite);
  setup_test_success_responder(&suite);

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return 0;
}

