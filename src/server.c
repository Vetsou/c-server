#include "include/server.h"
#include <stdio.h>
#include <string.h>

int init_server(ServerHttp *server, int port) {
  // Create socket
  SOCKET socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (socket_fd == INVALID_SOCKET) {
    printf("nit_sever: error creating socket: %d\n", WSAGetLastError());
    return -1;
  }

  // Define internet address 
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // Bind socket to address
  if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) != 0) {
    printf("init_sever: error binding socket: %d\n", WSAGetLastError());
    return -1;
  }

  // Listen for connections
  if (listen(socket_fd, 10) != 0) {
    printf("init_sever: error setting up listen socket: %d\n", WSAGetLastError());
    return -1;
  }

  // Set server params
  server->socket = socket_fd;
  server->port = port;

  return 0;
}

int close_server(ServerHttp *server) {
  return closesocket(server->socket);
}

int http_send(SOCKET dest_socket) {
  const char* http_res = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nConnection: Closed\r\n\r\nHello, World!";
  
  if (send(dest_socket, http_res, strlen(http_res), 0) == -1) {
    printf("http_send: error sending msg: %d\n", WSAGetLastError());
    return -1;
  }

  return 0;
}

int http_recv(SOCKET from_socket, char* buffer, unsigned long len) {
  int bytes_read = recv(from_socket, buffer, len, 0);

  if (bytes_read == -1) {
    printf("http_recv: error reading msg: %d\n", WSAGetLastError());
    return -1;
  }

  return bytes_read;
}