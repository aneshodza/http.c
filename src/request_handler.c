#include "http_request.h"
#include <request_handler.h>

const char* bad_request_response =
  "HTTP/1.1 400 Bad Request\r\n"
  "Content-Type: text/html\r\n"
  "Content-Length: 55\r\n"
  "Connection: close\r\n"
  "\r\n"
  "<html><body><h1>400 - Bad Request</h1></body></html>";

const char* unsupported_encoding_response =
  "HTTP/1.1 406 Not Acceptable\r\n"
  "Content-Type: text/html\r\n"
  "Content-Length: 66\r\n"
  "Connection: close\r\n"
  "\r\n"
  "<html><body><h1>406 - Encoding Not Supported</h1></body></html>";

const char* response =
  "HTTP/1.1 200 OK\r\n"
  "Content-Type: text/html\r\n"
  "Content-Length: 47\r\n"
  "Connection: close\r\n"
  "\r\n"
  "<html><body><h1>Hello, World!</h1></body></html>";

void* handle_request(void* args) {
  char buffer[4096] = {0};
  int new_socket = *(int*)args;
  free(args);

  read(new_socket, buffer, sizeof(buffer) - 1);
  buffer[4095] = '\0';
  printf("==============================================\n");
  printf("%s", buffer);
  printf("==============================================\n");

  HttpRequest req = {0};
  int result = parse_request(buffer, &req);

  if (result == -1) {
    printf("Bad request");
    send(new_socket, bad_request_response, strlen(bad_request_response), 0);
  } else if (result == -2) {
    printf("Unsupported encoding\n");
    send(new_socket, unsupported_encoding_response, strlen(unsupported_encoding_response), 0);
  } else {
    send(new_socket, response, strlen(response), 0);
  }
  close(new_socket);

  return NULL;
}

