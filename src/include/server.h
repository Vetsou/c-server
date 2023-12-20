#ifndef _HTTP_SERVER_H
#define _HTTP_SERVER_H

#include <winsock2.h>

typedef struct ServerHttp {
  SOCKET socket;
  int port;
} ServerHttp;


int init_server(ServerHttp *server, int port);
int close_server(ServerHttp *server);

// Send/Recv data
int http_send(SOCKET dest_socket);
int http_recv(SOCKET from_socket, char* buffer, unsigned long len);

#endif // _HTTP_SERVER_H