#include "include/routes.h"
#include <stdlib.h>
#include <string.h>

extern Route* init_route(char *path, HttpResponse *res) {
  Route* route = (Route *)malloc(sizeof(Route));
  if (route != NULL) {
    route->path = path;
    route->response = malloc(sizeof(HttpResponse));
    memcpy(route->response, res, sizeof(HttpResponse));
    route->left = route->right = NULL;
  }
  return route;
}

extern Route* add_route(Route *root, char *path, HttpResponse *res) {
  if (root == NULL) {
    return init_route(path, res);
  }

  int comparator = strncmp(path, root->path, HTTP_PATH_SIZE);

  if (comparator == 0) {
    return NULL;
  } else if (comparator > 0) {
    root->right = add_route(root->right, path, res);
  } else {
    root->left = add_route(root->left, path, res);
  }

  return root;
}

extern Route* search_routes(Route *root, char *path) {
  if (root == NULL) {
    return NULL;
  }

  int comparator = strncmp(path, root->path, HTTP_PATH_SIZE);

  if (comparator == 0) {
    return root;
  } else if (comparator > 0) {
    return search_routes(root->right, path);
  } else {
    return search_routes(root->left, path);
  }
}

extern void free_routes(Route *root) {
  if (root != NULL) {
    free_routes(root->right);
    free_routes(root->left);
    free(root);
  }
}