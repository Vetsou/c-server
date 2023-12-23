#include "include/http_structs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const size_t HTTP_REQUEST_MAX_SIZE = 4096;
const size_t HTTP_RESPONSE_HEADER_SIZE = 4096;

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
  if (strcmp(method, "GET") == 0) {
    return HTTP_GET;
  }
  if (strcmp(method, "POST") == 0) {
    return HTTP_POST;
  }
  if (strcmp(method, "PUT") == 0) {
    return HTTP_PUT;
  }
  if (strcmp(method, "DELETE") == 0) {
    return HTTP_DELETE;
  }

  return HTTP_UNKNOWN; 
}

static void parse_http_header(HttpRequest *req, char *http_header) {
  char *tok_buff; // Buffer for tokens
  size_t ref_index = 0; // Tracks the start of the next token
  size_t token_count = 0;

  for (size_t i = 0; i < strlen(http_header); ++i) {
    if (http_header[i] == ' ') {
      switch(token_count) {
        // Parse method
        case 0:
          tok_buff = (char *)malloc((i + 1) * sizeof(char));
          strncpy(tok_buff, http_header, i);
          req->method = parse_req_method(tok_buff);
          free(tok_buff);
          break; 

        // Parse path
        case 1:
          req->path = malloc((i - ref_index) * sizeof(char));
          strncpy(req->path, &http_header[ref_index], i - ref_index);
          req->path[i - ref_index] = '\0';
          break;
      }

      ref_index = i + 1;
      token_count++;
    }
  }
}

extern int parse_request(HttpRequest *req, char *req_str) {
  if (strnlen(req_str, HTTP_REQUEST_MAX_SIZE) >= HTTP_REQUEST_MAX_SIZE) {
    return -1;
  }

  char *strtok_ctx = NULL;
  char *http_header = strtok_s(req_str, "\n", &strtok_ctx);
  parse_http_header(req, http_header);

  return 0;
}

extern int destroy_request(HttpRequest *req) {
  if (req->path) {
    free(req->path);
    req->path = NULL;
  }
  return 0;
}