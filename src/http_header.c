#include "include/http_header.h"
#include <stdlib.h>
#include <string.h>

extern int init_http_headers(HttpHeaders *headers, const size_t init_capacity) {
  headers->items = (PairItem *)malloc(sizeof(PairItem) * init_capacity);
  headers->capacity = init_capacity;
  headers->length = 0;

  for (size_t i = 0; i < init_capacity; ++i) {
    headers->items[i].key = NULL;
    headers->items[i].value = NULL;
  }

  return 0;
}

extern int add_http_header(HttpHeaders *headers, char *key, char *value) {
  headers->items[headers->length].key = strdup(key);
  headers->items[headers->length].value = strdup(value);
  headers->length++;

  if (headers->length == headers->capacity) {
    PairItem *new_items = (PairItem *)realloc(headers->items, headers->capacity * 2 * sizeof(PairItem));
    if (!new_items) return -1;

    headers->capacity *= 2;
    headers->items = new_items;
  }

  return 0;
}

extern void free_http_headers(HttpHeaders *headers) {
  for (size_t i = 0; i < headers->length; ++i) {
    free(headers->items[i].key);
		headers->items[i].key = NULL;

		free(headers->items[i].value);
		headers->items[i].value = NULL;
  }

  free(headers->items);
}