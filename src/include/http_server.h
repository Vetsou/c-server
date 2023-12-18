#ifndef _HTTP_SERVER_H
#define _HTTP_SERVER_H

#include <winsock2.h>

typedef struct ServerHttp {
  SOCKET socket;
  int port;
} ServerHttp;

int init_server(ServerHttp *server, int port);
int close_server(ServerHttp *server);

#endif // _HTTP_SERVER_H