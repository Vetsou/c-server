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

  if (server_listen(&server) != 0) {
    close_server(&server);
    WSACleanup();
    return -1;
  }

  return 0;
}