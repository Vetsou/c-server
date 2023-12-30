#include "include/http_operations.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

static const char REQUEST_METHOD_NAME[5][7] = {
  "UNKNOWN",
  "GET",
  "POST",
  "PUT",
  "DELETE"
};

//
// HTTP RESPONSE
//
extern int create_response(HttpResponse *res, HttpHeaders *headers, StatusCode code, const char *body) {
  size_t response_body_size = MAX_HTTP_RESPONSE_HEADER_SIZE + strlen(body) + 1;
  if (response_body_size >= MAX_HTTP_RESPONSE_SIZE) {
    return -1;
  }

  res->body = NULL;
  res->body = (char *)malloc(response_body_size * sizeof(char));

  // Fill header
  strncpy(res->body, STATUS_CODE_LABEL[code], 50);
  for (size_t i = 0; i < headers->length; ++i) {
    strncat(res->body, headers->items[i].key, HTTP_HEADER_KEY_SIZE);
    strcat(res->body, ": ");
    strncat(res->body, headers->items[i].value, HTTP_HEADER_VALUE_SIZE);
    strcat(res->body, "\r\n");
  }
  strcat(res->body, "\r\n");

  // Fill body
  strncat(res->body, body, strlen(body));

  return 0;
}

extern int create_file_response(HttpResponse *res, HttpHeaders *headers, StatusCode code, const char *file_path) {
  FILE *file = fopen(file_path, "rb");
  if (file == NULL) return -1;

  fseek(file, 0, SEEK_END);
  size_t file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  size_t response_body_size = MAX_HTTP_RESPONSE_HEADER_SIZE + file_size + 1;
  if (response_body_size >= MAX_HTTP_RESPONSE_SIZE) {
    fclose(file);
    return -1;
  }

  res->body = NULL;
  res->body = (char *)malloc(response_body_size * sizeof(char));

  strncpy(res->body, STATUS_CODE_LABEL[code], 50);
  for (size_t i = 0; i < headers->length; ++i) {
    strncat(res->body, headers->items[i].key, HTTP_HEADER_KEY_SIZE);
    strcat(res->body, ": ");
    strncat(res->body, headers->items[i].value, HTTP_HEADER_VALUE_SIZE);
    strcat(res->body, "\r\n");
  }
  strcat(res->body, "\r\n");

  fread(res->body + strlen(res->body), 1, file_size, file);
  fclose(file);
  return 0;
}

extern int free_response(HttpResponse *res) {
  if (res->body != NULL) {
    free(res->body);
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
  size_t buffer_size;

  req->path = NULL;
  req->version = NULL;

  // Set request method
  req->method = parse_req_method(strtok_s(http_header, " ", &http_header));
  if (req->method == HTTP_UNKNOWN) return -1;

  // Copy HTTP path
  token = strtok_s(NULL, " ", &http_header);
  if (token == NULL || strlen(token) > HTTP_PATH_SIZE) return -1;
  
  buffer_size = strlen(token) * sizeof(char) + 1;
  req->path = (char *)malloc(buffer_size);
  memcpy(req->path, token, buffer_size);

  // Copy HTTP version
  token = strtok_s(NULL, "\r\n", &http_header);
  if (token == NULL || strlen(token) > HTTP_VERSION_SIZE) return -1;

  buffer_size = strlen(token) * sizeof(char) + 1;
  req->version = (char *)malloc(buffer_size);
  memcpy(req->version, token, buffer_size);

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
  init_http_headers(&req->headers, 5);
  while ((http_line = strtok_s(req_buffer, "\n", &req_buffer)) != NULL) {
    if (strlen(http_line) == 1) break;

    char* field_name = strtok_s(http_line, ":", &http_line);
    // Trim spaces
    while (http_line[0] == ' ') http_line++;
    char* token = strtok_s(NULL, "\n", &http_line);

    add_http_header(&req->headers, field_name, token);
  }

  return 0;
}

extern int free_request(HttpRequest *req) {
  if (req->path != NULL) {
    free(req->path);
  }

  if (req->version != NULL) {
    free(req->version);
  }

  free_http_headers(&req->headers);
  return 0;
}

extern const char* get_method_name(RequestMethod req_enum) {
  return REQUEST_METHOD_NAME[req_enum];
}