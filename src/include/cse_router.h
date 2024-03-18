#ifndef _CSE_ROUTER
#define _CSE_ROUTER

#include "cse_http.h"

typedef CSE_HttpResponse* (*CSE_RouteHandler)();

typedef struct Route {
  char *path;
  CSE_RouteHandler handler;
  struct Route *left, *right;
} CSE_Route;

extern CSE_Route* CSE_InitRoute(const char *path, CSE_RouteHandler handler);
extern CSE_Route* CSE_AddRoute(CSE_Route *root, char *path, CSE_RouteHandler handler);
extern CSE_Route* CSE_SearchRoute(CSE_Route *root, char *path);
extern void CSE_FreeRoutes(CSE_Route *root);

#endif // _CSE_ROUTER