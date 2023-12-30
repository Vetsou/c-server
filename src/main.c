#include <stdio.h>
#include <winsock2.h>

#include "include/server.h"

// Server config
const int SERVER_PORT = 8080;

int init_server_routes(ServerHttp *server) {
  HttpHeaders headers;
  init_http_headers(&headers, 3);
  add_http_header(&headers, "Content-Type", "text/html");
  add_http_header(&headers, "Connection", "Closed");

  HttpResponse res;

  create_file_response(&res, &headers, STATUSCODE_OK, "./html/main.html");
  server_add_route(server, "/", &res);

  create_file_response(&res, &headers, STATUSCODE_OK, "./html/home.html");
  server_add_route(server, "/home", &res);
  
  create_file_response(&res, &headers, STATUSCODE_OK, "./html/contact.html");
  server_add_route(server, "/contact", &res);

  HttpHeaders css_header;
  init_http_headers(&css_header, 3);
  add_http_header(&css_header, "Content-Type", "text/css");
  add_http_header(&css_header, "Connection", "Closed");

  create_file_response(&res, &css_header, STATUSCODE_OK, "./css/styles.css");
  server_add_route(server, "/css/styles.css", &res);

  return 0;
}

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

  // Add server routes
  if (init_server_routes(&server) != 0) {
    close_server(&server);
    WSACleanup();
    return -1;
  }

  // Start server
  if (server_listen(&server) != 0) {
    close_server(&server);
    WSACleanup();
    return -1;
  }

  return 0;
}