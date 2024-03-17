#ifndef _CSE_SERVER
#define _CSE_SERVER

#include "cse_socket.h"
#include "cse_logger.h"
#include "cse_router.h"

typedef struct {
  CSE_Route *router;
  CSE_Logger *logger;
  CSE_Socket socket;
  int port;
} CSE_Server;

extern CSE_Server* CSE_InitServer(int port, CSE_LOG_MODE log_mode);
extern void CSE_RunServer(CSE_Server *server);
extern void CSE_FreeServer(CSE_Server *server);

#endif // _CSE_SERVER