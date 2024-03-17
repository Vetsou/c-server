#ifndef _CSE_SOCKET
#define _CSE_SOCKET

#include <winsock2.h>

typedef SOCKET CSE_Socket;
typedef struct sockaddr_in CSE_Address;

extern CSE_Socket CSE_SocketTCP();
extern int CSE_SocketBind(CSE_Socket socket_fd, unsigned int port);
extern int CSE_SocketClose(CSE_Socket socket_fd);

#endif // _CSE_SOCKET