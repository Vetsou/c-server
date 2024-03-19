#include "include/cse_server.h"
#include "include/cse_http.h"
#include <errno.h>

static CSE_HttpResponse* CSE_HandleHomeRoute()    { return CSE_CreateFileResponse(STATUS_OK, "./static/index.html", "text/html");            }
static CSE_HttpResponse* CSE_HandleAboutRoute()   { return CSE_CreateFileResponse(STATUS_OK, "./static/about.html", "text/html");            }
static CSE_HttpResponse* CSE_HandleContactRoute() { return CSE_CreateFileResponse(STATUS_OK, "./static/contact.html", "text/html");          }
static CSE_HttpResponse* CSE_HandleStyles()       { return CSE_CreateFileResponse(STATUS_OK, "./static/style.css", "text/css");              }
static CSE_HttpResponse* CSE_HandleNotFound()     { return CSE_CreateFileResponse(STATUS_NOT_FOUND, "./static/not_found.html", "text/html"); }

static CSE_Route* CSE_InitServerRouter() {
  CSE_Route *router = NULL;
  router = CSE_AddRoute(router, "/", CSE_HandleHomeRoute);
  router = CSE_AddRoute(router, "*", CSE_HandleNotFound);
  router = CSE_AddRoute(router, "/style.css", CSE_HandleStyles);
  router = CSE_AddRoute(router, "/about", CSE_HandleAboutRoute);
  router = CSE_AddRoute(router, "/contact", CSE_HandleContactRoute);

  return router;
}

static CSE_HttpRequest* CSE_RecvServerRequest(CSE_Server* server, CSE_Socket client_sock) {
  char *buffer = (char *)malloc(HTTP_REQ_LEN_LIMIT);
  int bytes_read = recv(client_sock, buffer, HTTP_REQ_LEN_LIMIT, 0);

  if (bytes_read == SOCKET_ERROR) {
    CSE_LogMsg(server->logger, LOG_ERROR, "Error reading http request (WinErr: %d)", WSAGetLastError());
    CSE_SocketClose(client_sock);
    free(buffer);
    return NULL;
  }

  if (bytes_read >= HTTP_REQ_LEN_LIMIT) {
    CSE_LogMsg(server->logger, LOG_ERROR, "Error http request too large");
    CSE_SocketClose(client_sock);
    free(buffer);
    return NULL;
  }

  buffer[bytes_read] = '\0';
  CSE_HttpRequest *req = CSE_ParseHttpRequest(buffer);
  if (req == NULL) {
    CSE_LogMsg(server->logger, LOG_ERROR, "Error parsing http request");
    CSE_SocketClose(client_sock);
    free(buffer);
    return NULL;
  }

  return req;
}

static void CSE_SendServerResponse(CSE_Server* server, CSE_Socket client, CSE_HttpResponse *res) {
  if (send(client, res->body, strlen(res->body), 0) == SOCKET_ERROR) {
    CSE_LogMsg(server->logger, LOG_ERROR, "Error sending response (WinErr: %d)", WSAGetLastError());
  }
}



CSE_Server* CSE_InitServer(int port, CSE_LOG_MODE log_mode) {
  CSE_Logger *logger = CSE_InitLogger("./server_logs.log", log_mode);
  
  WSADATA wsa_data;
  if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
    CSE_LogMsg(logger, LOG_ERROR, "Error init winsock (WinErr: %d)", WSAGetLastError());
    CSE_FreeLogger(logger);
    exit(EXIT_FAILURE);
  }

  CSE_Socket socket_fd = CSE_SocketTCP();
  if (socket_fd == INVALID_SOCKET) {
    CSE_LogMsg(logger, LOG_ERROR, "Error creating socket (WinErr: %d)", WSAGetLastError());
    CSE_FreeLogger(logger);
    exit(EXIT_FAILURE);
  }

  if (CSE_SocketBind(socket_fd, port) != 0) {
    CSE_LogMsg(logger, LOG_ERROR, "Error binding socket (WinErr: %d)", WSAGetLastError());
    CSE_FreeLogger(logger);
    exit(EXIT_FAILURE);
  }

  CSE_Server *server = (CSE_Server *)malloc(sizeof(CSE_Server));
  *server = (CSE_Server) {
    .router = CSE_InitServerRouter(),
    .socket = socket_fd,
    .logger = logger,
    .port = port
  };

  return server;
}

void CSE_RunServer(CSE_Server *server) {
  if (listen(server->socket, 15) != 0) {
    CSE_LogMsg(server->logger, LOG_ERROR, "Error setting up listen socket (WinErr: %d)", WSAGetLastError());
    exit(EXIT_FAILURE);
  }

  CSE_LogMsg(server->logger, LOG_INFO, "Server started on port %d...", server->port);

  while (TRUE) {
    // Accept client
    CSE_Socket client_sock = accept(server->socket, NULL, NULL);
    if (client_sock == INVALID_SOCKET) {
      CSE_LogMsg(server->logger, LOG_ERROR, "Error accepting client (WinErr: %d)", WSAGetLastError());
      CSE_SocketClose(client_sock);
      continue;
    }

    // Try to parse client request
    CSE_HttpRequest *req = CSE_RecvServerRequest(server, client_sock);
    if (req == NULL) {
      CSE_SocketClose(client_sock);
      continue;
    }

    CSE_LogMsg(server->logger, LOG_INFO, "Accepted client [%d]: %d %s %s", 
      client_sock, req->method, req->uri, req->version
    );

    // Try to find route
    CSE_Route *found_route = CSE_SearchRoute(server->router, req->uri);
    if (found_route == NULL) {
      CSE_LogMsg(server->logger, LOG_WARN, "Route path %s not found", req->uri);

      // Send NOT_FOUND response
      CSE_Route *fallback_route = CSE_SearchRoute(server->router, "*");
      if (fallback_route != NULL) {
        CSE_HttpResponse *res = fallback_route->handler();
        CSE_SendServerResponse(server, client_sock, res);
        CSE_FreeHttpResponse(res);
      }

      CSE_SocketClose(client_sock);
      CSE_FreeHttpRequest(req);
      continue;
    }

    // Send response
    CSE_HttpResponse *res = found_route->handler();
    CSE_SendServerResponse(server, client_sock, res);
    CSE_FreeHttpResponse(res);

    CSE_FreeHttpRequest(req);
    CSE_SocketClose(client_sock);
  }
}

void CSE_FreeServer(CSE_Server *server) {
  CSE_LogMsg(server->logger, LOG_INFO, "Closing server...");

  CSE_FreeLogger(server->logger);
  CSE_FreeRoutes(server->router);
  CSE_SocketClose(server->socket);
  WSACleanup();

  free(server);
  server = NULL;
}