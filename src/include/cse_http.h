#ifndef _CSE_HTTP
#define _CSE_HTTP

#include <stddef.h>

///////////////////////////////////////////////////////////
// HTTP headers utility functions and structures.
///////////////////////////////////////////////////////////

#define HTTP_HEADER_ITEM_LIMIT 256

typedef struct {
  char *name;
  char *value;
} CSE_HttpHeader;

typedef struct {
  CSE_HttpHeader *items;
  size_t capacity;
  size_t length;
} CSE_HttpHeaderList;

extern CSE_HttpHeaderList* CSE_InitHttpHeaders(const size_t capacity);
extern void CSE_AddHttpHeader(CSE_HttpHeaderList *header_list, const char *name, const char *value);
extern void CSE_FreeHttpHeaders(CSE_HttpHeaderList *header_list);


///////////////////////////////////////////////////////////
// HTTP response functions and structures.
///////////////////////////////////////////////////////////

#define HTTP_RES_HEADER_LIMIT 2048
#define HTTP_RES_BODY_LIMIT 63488
#define HTTP_RES_LEN_LIMIT (HTTP_RES_HEADER_LIMIT + HTTP_RES_BODY_LIMIT)

#define HTTP_STATUS_LABEL_SIZE 32

typedef enum {
  STATUS_OK = 0,
  STATUS_BAD_REQUEST,
  STATUS_NOT_FOUND,
  STATUS_INTERNAL_ERROR
} CSE_STATUS_CODE;

typedef struct {
  CSE_STATUS_CODE code;
  char *body;
} CSE_HttpResponse;

extern CSE_HttpResponse* CSE_InitHttpResponse(CSE_STATUS_CODE code, const CSE_HttpHeaderList *headers, const char *content);
extern void CSE_FreeHttpResponse(CSE_HttpResponse *res);

extern CSE_HttpResponse* CSE_CreateFileResponse(CSE_STATUS_CODE code, const char *html_path, char *res_type);

///////////////////////////////////////////////////////////
// HTTP request functions and structures.
///////////////////////////////////////////////////////////

#define HTTP_REQ_LEN_LIMIT 16384
#define HTTP_REQ_MIN_LEN 16

#define HTTP_REQ_URI_LIMIT 512
#define HTTP_REQ_VERSION_LIMIT 16

typedef enum {
  HTTP_UNKNOWN = 0,
  HTTP_GET,
  HTTP_POST,
  HTTP_PUT,
  HTTP_DELETE
} CSE_REQUEST_METHOD;

typedef struct {
  CSE_REQUEST_METHOD method;
  CSE_HttpHeaderList *headers;
  char *uri;
  char *version;
} CSE_HttpRequest;

extern CSE_HttpRequest* CSE_ParseHttpRequest(char *http_req_str);
extern void CSE_FreeHttpRequest(CSE_HttpRequest *req);

#endif // _CSE_HTTP