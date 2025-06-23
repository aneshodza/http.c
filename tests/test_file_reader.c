#include <CUnit/CUnit.h>
#include <file_reader.h>

void test_reads_file_correctly() {
  char *path = "./public_test/index.html";
  char *file = read_file(path, (char*)-1, (char*)-1);

  CU_ASSERT_STRING_EQUAL(file, "<h1>Test File index.html</h1>\n");
  free(file);
}

void test_returns_fallback_on_missing_file() {
  char *path = "./public_test/inexistant_file.html";
  char *file = read_file(path, (char*)-1, (char*)-2);

  CU_ASSERT_PTR_EQUAL(file, (char *)-1);
}


void setup_test_file_reader(CU_pSuite *suite) {
  CU_add_test(*suite, "Reads existing file correctly", test_reads_file_correctly);
  CU_add_test(*suite, "Returns the fallback value if the file doesn't exist", test_returns_fallback_on_missing_file);
}

