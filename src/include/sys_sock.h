#ifndef _SYS_SOCK_H
#define _SYS_SOCK_H

#include <winsock2.h>

int init_winsock(WSADATA *wsadata, short majorVer, short minorVer);
int cleanup_winsock();

#endif // _SYS_SOCK_H
