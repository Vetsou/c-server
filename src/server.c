#include "include/server.h"
#include <stdio.h>
#include <string.h>

extern int init_server(ServerHttp *server, ServerLogger logger, int port) {
  server->logger = logger;

  // Create socket
  SOCKET socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (socket_fd == INVALID_SOCKET) {
    log_message(&server->logger, LOG_LEVEL_ERROR, "Init_sever: error creating socket");
    return -1;
  }

  // Define internet address 
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // Bind socket to address
  if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) != 0) {
    log_message(&server->logger, LOG_LEVEL_ERROR, "Init_sever: error binding socket");
    return -1;
  }

  // Listen for connections
  if (listen(socket_fd, 10) != 0) {
    log_message(&server->logger, LOG_LEVEL_ERROR, "Init_sever: error setting up listen socket");
    return -1;
  }

  server->socket = socket_fd;
  server->port = port;

  return 0;
}

extern int close_server(ServerHttp *server) {
  destroy_logger(&server->logger);
  closesocket(server->socket);
  server = NULL;

  return 0;
}

extern int http_send_response(ServerHttp *server, SOCKET dest_socket, HttpResponse *res) {
  if (send(dest_socket, res->body, strlen(res->body), 0) == -1) {
    log_message(&server->logger, LOG_LEVEL_ERROR, "Http_send: error sending msg");
    return -1;
  }

  return 0;
}

extern int http_recv_request(ServerHttp *server, SOCKET from_socket, HttpRequest *req) {
  char *buffer = (char *)malloc(HTTP_REQUEST_MAX_SIZE * sizeof(char));
  int bytes_read = recv(from_socket, buffer, HTTP_REQUEST_MAX_SIZE, 0);

  if (bytes_read == -1) {
    log_message(&server->logger, LOG_LEVEL_ERROR, "Http_recv_request: error reading http request");
    return -1;
  }

  if (parse_request(req, buffer) != 0) {
    log_message(&server->logger, LOG_LEVEL_ERROR, "Http_recv_request: error parsing http request");
    return -1;
  }

  free(buffer);
  return bytes_read;
}