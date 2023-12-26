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

extern int init_server(ServerHttp *server, ServerLogger logger, int port);
extern int server_listen(ServerHttp *server);
extern int close_server(ServerHttp *server);

#endif // _HTTP_SERVER_H