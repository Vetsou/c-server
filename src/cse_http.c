#include "include/cse_http.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static const char* CSE_ParseHttpStatusLabel(CSE_STATUS_CODE status_code) {
  switch (status_code) {
    case STATUS_OK:
      return "HTTP/1.1 200 OK\r\n";
    case STATUS_BAD_REQUEST:
      return "HTTP/1.1 400 Bad request\r\n";
    case STATUS_NOT_FOUND:
      return "HTTP/1.1 404 Not found\r\n";
    case STATUS_INTERNAL_ERROR:
      return "HTTP/1.1 500 Internal Error\r\n";
    default:
      return "HTTP/1.1 501 Not implemented\r\n";
  }
}

static CSE_REQUEST_METHOD CSE_ParseHttpReqMethod(const char *method) {
  if (method == NULL) return HTTP_UNKNOWN;
  if (strcmp(method, "GET") == 0) return HTTP_GET;
  if (strcmp(method, "POST") == 0) return HTTP_POST;
  if (strcmp(method, "DELETE") == 0) return HTTP_DELETE;
  if (strcmp(method, "PUT") == 0) return HTTP_PUT;
  return HTTP_UNKNOWN;
}

static CSE_HttpRequest* CSE_ParseHttpReqLine(char *http_req_line) {
  size_t token_size = 0;
  char *token_buff = NULL;

  CSE_HttpRequest *req = (CSE_HttpRequest *)malloc(sizeof(CSE_HttpRequest));
  *req = (CSE_HttpRequest) {
    .method = HTTP_UNKNOWN,
    .headers = NULL,
    .version = NULL,
    .uri = NULL
  };

  // Parse request method
  req->method = CSE_ParseHttpReqMethod(strtok_s(http_req_line, " ", &http_req_line));
  if (req->method == HTTP_UNKNOWN) {
    CSE_FreeHttpRequest(req);
    return NULL;
  }

  // Parse request uri
  token_buff = strtok_s(http_req_line, " ", &http_req_line);
  if (token_buff == NULL || strlen(token_buff) >= HTTP_REQ_URI_LIMIT) {
    CSE_FreeHttpRequest(req);
    return NULL;
  }
  
  token_size = strlen(token_buff) + 1;
  req->uri = (char *)malloc(token_size);
  memcpy(req->uri, token_buff, token_size);

  // Parse HTTP version
  token_buff = strtok_s(http_req_line, " ", &http_req_line);
  if (token_buff == NULL || strlen(token_buff) > HTTP_REQ_VERSION_LIMIT) {
    CSE_FreeHttpRequest(req);
    return NULL;
  }
  
  token_size = strlen(token_buff) + 1;
  req->version = (char *)malloc(token_size);
  memcpy(req->version, token_buff, token_size);

  return req;
}



CSE_HttpHeaderList* CSE_InitHttpHeaders(const size_t capacity) {
  CSE_HttpHeaderList *header_list = (CSE_HttpHeaderList *)malloc(sizeof(CSE_HttpHeaderList));

  CSE_HttpHeader *headers = (CSE_HttpHeader *)malloc(sizeof(CSE_HttpHeader) * capacity);
  for (size_t i = 0; i < capacity; ++i) {
    headers[i].name = NULL;
    headers[i].value = NULL;
  }

  *header_list = (CSE_HttpHeaderList) {
    .capacity = capacity,
    .items = headers,
    .length = 0
  };

  return header_list;
}

void CSE_AddHttpHeader(CSE_HttpHeaderList *header_list, const char *name, const char *value) {
  if (strnlen(name, HTTP_HEADER_ITEM_LIMIT) >= HTTP_HEADER_ITEM_LIMIT) {
    return;
  }

  if (strnlen(value, HTTP_HEADER_ITEM_LIMIT) >= HTTP_HEADER_ITEM_LIMIT) {
    return;
  }

  header_list->items[header_list->length].name = strdup(name);
  header_list->items[header_list->length].value = strdup(value);
  header_list->length++;

  if (header_list->length == header_list->capacity) {
    size_t new_size = header_list->capacity * 2 * sizeof(CSE_HttpHeader);
    CSE_HttpHeader *new_items = (CSE_HttpHeader *)realloc(header_list->items, new_size);

    header_list->capacity *= 2;
    header_list->items = new_items;
  }
}

void CSE_FreeHttpHeaders(CSE_HttpHeaderList *header_list) {
  for (size_t i = 0; i < header_list->length; ++i) {
    free(header_list->items[i].name);
    header_list->items[i].name = NULL;

    free(header_list->items[i].value);
    header_list->items[i].value = NULL;
  }

  free(header_list->items);
  header_list->items = NULL;

  free(header_list);
  header_list = NULL;
}



CSE_HttpResponse* CSE_InitHttpResponse(CSE_STATUS_CODE code, const CSE_HttpHeaderList *headers, const char *content) {
  size_t res_body_size = HTTP_RES_HEADER_LIMIT + strnlen(content, HTTP_RES_BODY_LIMIT) + 1;
  if (res_body_size >= HTTP_RES_LEN_LIMIT) {
    return NULL;
  }

  char *res_body = (char *)malloc(res_body_size);
  strncpy(res_body, CSE_ParseHttpStatusLabel(code), HTTP_STATUS_LABEL_SIZE);

  for (size_t i = 0; i < headers->length; ++i) {
    strncat(res_body, headers->items[i].name, HTTP_HEADER_ITEM_LIMIT);
    strcat(res_body, ": ");
    strncat(res_body, headers->items[i].value, HTTP_HEADER_ITEM_LIMIT);
    strcat(res_body, "\r\n");
  }
  strcat(res_body, "\r\n");
  strcat(res_body, content);

  CSE_HttpResponse *res = (CSE_HttpResponse *)malloc(sizeof(CSE_HttpResponse));
  *res = (CSE_HttpResponse) {
    .body = res_body,
    .code = code
  };

  return res;
}

void CSE_FreeHttpResponse(CSE_HttpResponse *res) {
  if (res->body) {
    free(res->body);
    res->body = NULL;
  }

  free(res);
  res = NULL;
}

CSE_HttpResponse* CSE_CreateHtmlResponse(CSE_STATUS_CODE code, const char *html_path) {
  FILE *fd = fopen64(html_path, "rt");
  if (fd == NULL) {
    return NULL;
  }

  size_t file_start = ftello64(fd);

  fseeko64(fd, 0L, SEEK_END);
  size_t file_size = ftello64(fd);
  if (file_size >= HTTP_RES_BODY_LIMIT) {
    return NULL;
  }

  fseeko64(fd, file_start, SEEK_SET);
  char *html_buff = (char *)malloc(file_size + 1);
  size_t bytes_read = fread(html_buff, sizeof(char), file_size, fd);
  html_buff[bytes_read] = '\0';
  fclose(fd);

  CSE_HttpHeaderList *headers = CSE_InitHttpHeaders(3);
  CSE_AddHttpHeader(headers, "Content-Type", "text/html");
  CSE_AddHttpHeader(headers, "Connection", "Closed");

  CSE_HttpResponse *res = CSE_InitHttpResponse(code, headers, html_buff);
  CSE_FreeHttpHeaders(headers);
  free(html_buff);

  return res;
}



CSE_HttpRequest* CSE_ParseHttpRequest(char *http_req_str) {
  size_t req_size = strnlen(http_req_str, HTTP_REQ_LEN_LIMIT);
  if (req_size >= HTTP_REQ_LEN_LIMIT || req_size < HTTP_REQ_MIN_LEN) {
    return NULL;
  }

  char *req_buffer = http_req_str;
  char *line_buffer = strtok_s(http_req_str, "\r\n", &req_buffer);

  CSE_HttpRequest *req = CSE_ParseHttpReqLine(line_buffer);
  if (req == NULL) {
    return NULL;
  }

  req->headers = CSE_InitHttpHeaders(5);
  while ((line_buffer = strtok_s(req_buffer, "\r\n", &req_buffer)) != NULL) {
    if (strlen(line_buffer) == 1) break;

    char* field_name = strtok_s(line_buffer, ":", &line_buffer);

    while (line_buffer[0] == ' ') line_buffer++;
    char* token = strtok_s(NULL, "\r\n", &line_buffer);

    CSE_AddHttpHeader(req->headers, field_name, token);
  }

  return req;
}

void CSE_FreeHttpRequest(CSE_HttpRequest *req) {
  if (req->uri) {
    free(req->uri);
    req->uri = NULL;
  }

  if (req->headers) {
    CSE_FreeHttpHeaders(req->headers);
    req->headers = NULL;
  }

  if (req->version) {
    free(req->version);
    req->version = NULL;
  }

  free(req);
  req = NULL;
}