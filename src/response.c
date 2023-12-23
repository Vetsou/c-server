#include "include/response.h"
#include <stdlib.h>
#include <string.h>

static const char STATUS_CODE_LABEL[10][50] = {
	"HTTP/1.1 200 OK\r\n",
	"HTTP/1.1 201 Created\r\n",
	"HTTP/1.1 202 Accepted\r\n",
	"HTTP/1.1 204 No Content\r\n",
	"HTTP/1.1 400 Bad request\r\n",
	"HTTP/1.1 401 Unauthorized\r\n",
	"HTTP/1.1 403 Forbidden\r\n",
	"HTTP/1.1 404 Not found\r\n",
	"HTTP/1.1 500 Internal Error\r\n",
	"HTTP/1.1 501 Not implemented\r\n"
};

void create_response(HttpResponse *res, StatusCode code, char *body) {
  size_t response_body_size = 4096 + strlen(body) + 1;
  res->body = (char *)malloc(response_body_size * sizeof(char));

  // Fill header
  strncpy(res->body, STATUS_CODE_LABEL[code], 50);
  strncat(res->body, "Content-Type: text/plain\r\n", 27);
  strncat(res->body, "Connection: Closed\r\n\r\n", 23);

  // Fill body
  strncat(res->body, body, strlen(body));
}

void destroy_response(HttpResponse *res) {
  if (res->body) {
    free(res->body);
    res->body = NULL;
  }
}