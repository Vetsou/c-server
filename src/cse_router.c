#include "include/cse_router.h"
#include <stdlib.h>
#include <string.h>

CSE_Route* CSE_InitRoute(const char *path, CSE_RouteHandler handler) {
  CSE_Route *route = (CSE_Route *)malloc(sizeof(CSE_Route));
  *route = (CSE_Route) {
    .handler = handler,
    .path = strdup(path),
    .left = NULL,
    .right = NULL
  };

  return route;
}

CSE_Route* CSE_AddRoute(CSE_Route *root, char *path, CSE_RouteHandler handler) {
  if (root == NULL) {
    return CSE_InitRoute(path, handler);
  }

  int comparator = strncmp(path, root->path, HTTP_REQ_URI_LIMIT);

  if (comparator == 0) {
    return NULL;
  } else if (comparator > 0) {
    root->right = CSE_AddRoute(root->right, path, handler);
  } else {
    root->left = CSE_AddRoute(root->left, path, handler);
  }

  return root;
}

CSE_Route* CSE_SearchRoute(CSE_Route *root, char *path) {
  if (root == NULL) {
    return NULL;
  }

  int comparator = strncmp(path, root->path, HTTP_REQ_URI_LIMIT);

  if (comparator == 0) {
    return root;
  } else if (comparator > 0) {
    return CSE_SearchRoute(root->right, path);
  } else {
    return CSE_SearchRoute(root->left, path);
  }
}

void CSE_FreeRoutes(CSE_Route *root) {
  if (root != NULL) {
    CSE_FreeRoutes(root->right);
    CSE_FreeRoutes(root->left);
    free(root);
  }
}