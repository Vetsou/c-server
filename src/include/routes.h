#ifndef _ROUTES_H
#define _ROUTES_H

#include "http_structs.h"

typedef struct Route {
  char *path;
  HttpResponse *response;
  struct Route *left, *right;
} Route;

extern Route* init_route(char *path, HttpResponse *res);
extern Route* add_route(Route *root, char *path, HttpResponse *res);
extern Route* search_routes(Route *root, char *path);
extern void free_routes(Route *root);

#endif // _ROUTES_H