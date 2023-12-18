#include <stdio.h>
#include <winsock2.h>

#include "include/sys_sock.h"
#include "include/http_server.h"

// Server config
const int SERVER_PORT = 8080;

int main(void) {
  // Initialize Winsock
  WSADATA wsa_data;
  if (init_winsock(&wsa_data, 2, 2) != 0) {
    return 1;
  }

  // Create http server
  ServerHttp server;
  if (init_server(&server, SERVER_PORT) != 0) {
    close_server(&server);
    cleanup_winsock();
    return 1;
  }

  printf("Server listening on port %d...\n", server.port);
  unsigned long client_socket;

  while (TRUE) {
    client_socket = accept(server.socket, NULL, NULL);
    printf("Accepted connection from client %lu\n", client_socket);
  }

  // Clean up
  printf("Server closing...\n");
  close_server(&server);
  cleanup_winsock();

  return 0;
}