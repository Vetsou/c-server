#ifndef _HTTP_SERVER_H
#define _HTTP_SERVER_H

#include <winsock2.h>
#include "http_structs.h"
#include "logger.h"

typedef struct ServerHttp {
  ServerLogger logger;
  SOCKET socket;
  int port;
} ServerHttp;

// Init/Close server
extern int init_server(ServerHttp *server, ServerLogger logger, int port);
extern int close_server(ServerHttp *server);

// Send/Recv data
extern int http_send(ServerHttp *server, SOCKET dest_socket, HttpResponse response);
extern int http_recv(ServerHttp *server, SOCKET from_socket, char *buffer, unsigned long len);

#endif // _HTTP_SERVER_H