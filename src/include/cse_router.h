#ifndef _CSE_ROUTER
#define _CSE_ROUTER

#include "cse_http.h"

typedef struct Route {
  char *path;
  CSE_HttpResponse *res;
  struct Route *left, *right;
} CSE_Route;

extern CSE_Route* CSE_InitRoute(const char *path, CSE_HttpResponse *res);
extern CSE_Route* CSE_AddRoute(CSE_Route *root, char *path, CSE_HttpResponse *res);
extern CSE_Route* CSE_SearchRoute(CSE_Route *root, char *path);
extern void CSE_FreeRoutes(CSE_Route *root);

#endif // _CSE_ROUTER