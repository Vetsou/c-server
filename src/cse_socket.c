#include "include/cse_socket.h"

CSE_Socket CSE_SocketTCP() {
  CSE_Socket socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  return socket_fd;
}

int CSE_SocketBind(CSE_Socket socket_fd, unsigned int port) {
  CSE_Address server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  return bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
}

int CSE_SocketClose(CSE_Socket socket_fd) {
  return closesocket(socket_fd);
}