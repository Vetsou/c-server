#ifndef _HTTP_RESPONSE_H
#define _HTTP_RESPONSE_H

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

void create_response(HttpResponse *res, StatusCode code, char *body);
void destroy_response(HttpResponse *res);

#endif // _HTTP_RESPONSE_H