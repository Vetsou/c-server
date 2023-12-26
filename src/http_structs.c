#include "include/http_structs.h"
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

//
// HTTP RESPONSE
//
extern int create_response(HttpResponse *res, StatusCode code, const char *body) {
  size_t response_body_size = HTTP_RESPONSE_HEADER_SIZE + strlen(body) + 1;
  res->body = (char *)malloc(response_body_size * sizeof(char));

  // Fill header
  strncpy(res->body, STATUS_CODE_LABEL[code], 50);
  strncat(res->body, "Content-Type: text/plain\r\n", 27);
  strncat(res->body, "Connection: Closed\r\n\r\n", 23);

  // Fill body
  strncat(res->body, body, strlen(body));

  return 0;
}

extern int destroy_response(HttpResponse *res) {
  if (res->body) {
    free(res->body);
    res->body = NULL;
  }

  return 0;
}

//
// HTTP REQUEST
//
static RequestMethod parse_req_method(const char *method) {
  if (strcmp(method, "GET") == 0) return HTTP_GET;
  if (strcmp(method, "POST") == 0) return HTTP_POST;
  if (strcmp(method, "DELETE") == 0) return HTTP_DELETE;
  if (strcmp(method, "PUT") == 0) return HTTP_PUT;

  return HTTP_UNKNOWN; 
}

static int parse_req_line(HttpRequest *req, char *http_header) {
  char *token;

  // Set request method
  req->method = parse_req_method(strtok_s(http_header, " ", &http_header));
  if (req->method == HTTP_UNKNOWN) return -1;

  // Copy HTTP path
  token = strtok_s(NULL, " ", &http_header);
  if (token == NULL || strlen(token) > HTTP_PATH_SIZE) return -1;
  memcpy(req->path, token, sizeof(req->path));

  // Copy HTTP version
  token = strtok_s(NULL, "\r\n", &http_header);
  if (token == NULL || strlen(token) > HTTP_VERSION_SIZE) return -1;
  memcpy(req->version, token, sizeof(req->version));

  return 0;
}

extern int parse_request(HttpRequest *req, char *req_str) {
  if (strnlen(req_str, MAX_HTTP_REQUEST_SIZE) >= MAX_HTTP_REQUEST_SIZE) {
    return -1;
  }

  char *req_buffer = req_str;
  char *http_line = strtok_s(req_buffer, "\n", &req_buffer);

  if (parse_req_line(req, http_line) != 0) {
    return -1;
  }

  // Parse request fields
  while ((http_line = strtok_s(req_buffer, "\n", &req_buffer)) != NULL) {
    if (strlen(http_line) == 1) break;

    char* field_name = strtok_s(http_line, ":", &http_line);
    // Trim spaces
    while (http_line[0] == ' ') http_line++;
    char* token = strtok_s(NULL, "\n", &http_line);

    // TODO Dynamic array (K,V)
  }

  return 0;
}