#ifndef _HTTP_OPERATIONS_H
#define _HTTP_OPERATIONS_H

#include "http_header.h"

/*
 * HTTP server request size constants
 */
#define MAX_HTTP_REQUEST_SIZE 16384
#define HTTP_PATH_SIZE 512
#define HTTP_VERSION_SIZE 10

#define HTTP_HEADER_KEY_SIZE 128
#define HTTP_HEADER_VALUE_SIZE 128

/*
 * HTTP server response size constants
 */
#define MAX_HTTP_RESPONSE_SIZE 8192
#define MAX_HTTP_RESPONSE_HEADER_SIZE 1024

typedef enum {
  STATUSCODE_OK = 0,
  STATUSCODE_CREATED,
  STATUSCODE_ACCEPTED,
  STATUSCODE_NO_CONTENT,
  STATUSCODE_BAD_REQUEST,
  STATUSCODE_UNAUTHORIZED,
  STATUSCODE_FORBIDDEN,
  STATUSCODE_NOT_FOUND,
  STATUSCODE_INTERNAL_ERROR,
  STATUSCODE_NOT_IMPLEMENTED
} StatusCode;

typedef struct {
  StatusCode code;
  char *body;
} HttpResponse;

// Create/Destroy reponse
extern int create_response(HttpResponse *res, StatusCode code, const char *body);
extern int free_response(HttpResponse *res);

typedef enum {
  HTTP_UNKNOWN = 0,
  HTTP_GET,
  HTTP_POST,
  HTTP_PUT,
  HTTP_DELETE
} RequestMethod;

typedef struct {
  RequestMethod method;
  HttpHeaders headers;
  char *path;
  char *version;
} HttpRequest;

// Create/Destroy request
extern int parse_request(HttpRequest *req, char *req_str);
extern int free_request(HttpRequest *req);

extern const char* get_method_name(RequestMethod req_enum);

#endif // _HTTP_OPERATIONS_H