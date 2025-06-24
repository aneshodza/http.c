#include <CUnit/CUnit.h>
#include <http_request.h>

void test_parse_valid_method() {
  const char *raw = "GET /index.html HTTP/1.1\r\n";
  HttpRequest req;
  CU_ASSERT_EQUAL(parse_request(raw, &req), 0);
  CU_ASSERT_EQUAL(req.method, GET);
}

void test_parse_valid_path() {
  const char *raw = "GET /index.html HTTP/1.1\r\n";
  HttpRequest req;
  CU_ASSERT_EQUAL(parse_request(raw, &req), 0);
  CU_ASSERT_STRING_EQUAL(req.path, "/index.html");
}

void test_parse_valid_version() {
  const char *raw = "GET /index.html HTTP/1.1\r\n";
  HttpRequest req;
  CU_ASSERT_EQUAL(parse_request(raw, &req), 0);
  CU_ASSERT_STRING_EQUAL(req.http_version, "HTTP/1.1");
}

void test_parse_valid_accept() {
  const char *raw = "GET /index.html HTTP/1.1\r\nAccept: image/jpeg\r\n";
  HttpRequest req;
  CU_ASSERT_EQUAL(parse_request(raw, &req), 0);
  CU_ASSERT_STRING_EQUAL(req.accept, "image/jpeg");
}

void test_parse_empty_accept() {
  const char *raw = "GET /index.html HTTP/1.1\r\n";
  HttpRequest req;
  CU_ASSERT_EQUAL(parse_request(raw, &req), 0);
  CU_ASSERT_STRING_EQUAL(req.accept, "*/*");
}

void test_parse_gzip_accept_encoding() {
  const char *raw = "GET /index.html HTTP/1.1\r\nAccept-Encoding: gzip, anything\r\n";
  HttpRequest req;
  CU_ASSERT_EQUAL(parse_request(raw, &req), 0);
  CU_ASSERT_EQUAL(req.accept_encoding, GZIP);
}

void test_parse_empty_accept_encoding() {
  const char *raw = "GET /index.html HTTP/1.1\r\n";
  HttpRequest req;
  CU_ASSERT_EQUAL(parse_request(raw, &req), 0);
  CU_ASSERT_EQUAL(req.accept_encoding, NONE);
}

void test_parse_unsupported_encoding() {
  const char *raw = "GET /index.html HTTP/1.1\r\nAccept-Encoding: nothing\r\n";
  HttpRequest req;
  CU_ASSERT_EQUAL(parse_request(raw, &req), -2);
  CU_ASSERT_EQUAL(req.accept_encoding, UNSUPPORTED);
}

void test_throw_out_url_params() {
  const char *raw = "GET /index.html?foo=bar HTTP/1.1\r\n";
  HttpRequest req;
  parse_request(raw, &req);
  CU_ASSERT_STRING_EQUAL(req.path, "/index.html");
  CU_ASSERT_STRING_EQUAL(req.http_version, "HTTP/1.1");
  CU_ASSERT_EQUAL(req.method, GET);
}

void setup_test_http_request(CU_pSuite *suite) {
  CU_add_test(*suite, "Valid Method Parsing", test_parse_valid_method);
  CU_add_test(*suite, "Valid Path Parsing", test_parse_valid_path);
  CU_add_test(*suite, "Valid Version Parsing", test_parse_valid_version);
  CU_add_test(*suite, "Valid Accept Parsing", test_parse_valid_accept);
  CU_add_test(*suite, "Valid Accept Parsing (Empty)", test_parse_empty_accept);
  CU_add_test(*suite, "Parsing GZIP as the encoding type", test_parse_gzip_accept_encoding);
  CU_add_test(*suite, "Parsing nothing as the encoding type", test_parse_empty_accept_encoding);
  CU_add_test(*suite, "Parsing only unsupported encoding types", test_parse_unsupported_encoding);
  CU_add_test(*suite, "Throwing out URL parameters", test_throw_out_url_params);
}
