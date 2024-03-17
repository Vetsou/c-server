#include "include/cse_server.h"
#include "include/cse_http.h"

int main(void) {
  CSE_Server *server = CSE_InitServer(8080, LOG_TARGET_FILE | LOG_TARGET_CONSOLE);
  CSE_RunServer(server);
  CSE_FreeServer(server);
  return 0;
}