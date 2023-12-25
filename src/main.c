#include <stdio.h>
#include <winsock2.h>

#include "include/server.h"

// Server config
const int SERVER_PORT = 8080;

int main(void) {
  // Initialize server logger
  ServerLogger logger;
  create_logger(&logger, "./server_logs.log", LOG_MODE_CONSOLE | LOG_MODE_FILE);

  // Initialize Winsock
  WSADATA wsa_data;
  if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
    log_message(&logger, LOG_LEVEL_ERROR, "Init_sever: error initializing winsock");
    return -1;
  }

  // Create http server
  ServerHttp server;
  if (init_server(&server, logger, SERVER_PORT) != 0) {
    close_server(&server);
    WSACleanup();
    return -1;
  }

  log_message(&logger, LOG_LEVEL_INFO, "Server started on port %d...\n", SERVER_PORT);

  HttpResponse response;
  create_response(&response, STATUSCODE_OK, "Hello from the server");

  SOCKET client_socket;

  while (TRUE) {
    client_socket = accept(server.socket, NULL, NULL);
    if (client_socket != INVALID_SOCKET) {
      printf("Accepted connection from client %I64d\n", client_socket);

      HttpRequest req;
      http_recv_request(&server, client_socket, &req);

      printf("%d %s %s\n", req.method, req.path, req.version);

      http_send_response(&server, client_socket, &response);
      closesocket(client_socket);
    }
  }

  // Clean up
  printf("Server closing...\n");
  close_server(&server);
  WSACleanup();

  return 0;
}