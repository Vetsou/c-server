#ifndef _HTTP_HEADER_H
#define _HTTP_HEADER_H

#include <stddef.h>

typedef struct {
  char *key;
  char *value;
} PairItem;

typedef struct {
  PairItem *items;
  size_t capacity;
  size_t length;
} HttpHeaders;

extern int init_http_headers(HttpHeaders *headers, const size_t init_capacity);
extern int add_http_header(HttpHeaders *headers, char *key, char *value);
extern void free_http_headers(HttpHeaders *headers);

#endif // _HTTP_HEADER_H