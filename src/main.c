#include <stdio.h>
#include <winsock2.h>

#include "include/server.h"

// Server config
const int SERVER_PORT = 8080;

int main(void) {
  // Initialize Winsock
  WSADATA wsa_data;
  if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
    printf("WSAStartup failed with error: %d\n", WSAGetLastError());
    return -1;
  }

  // Initialize server logger
  ServerLogger logger;
  create_logger(&logger, LOG_MODE_CONSOLE);

  // Create http server
  ServerHttp server;
  if (init_server(&server, logger, SERVER_PORT) != 0) {
    close_server(&server);
    WSACleanup();
    return -1;
  }

  printf("Server listening on port %d...\n", server.port);
  
  SOCKET client_socket;
  char buffer[1024] = {0};

  while (TRUE) {
    client_socket = accept(server.socket, NULL, NULL);
    if (client_socket != INVALID_SOCKET) {
      printf("Accepted connection from client %I64d\n", client_socket);

      http_recv(&server, client_socket, buffer, 1024);
      printf("Result %s", buffer);
      http_send(&server, client_socket);

      closesocket(client_socket);
    }
  }

  // Clean up
  printf("Server closing...\n");
  close_server(&server);
  WSACleanup();

  return 0;
}