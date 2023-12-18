#include "include/sys_sock.h"
#include <stdio.h>

int init_winsock(WSADATA *wsadata, short majorVer, short minorVer) {
  if (WSAStartup(MAKEWORD(majorVer, minorVer), wsadata) != 0) {
    printf("WSAStartup failed with error: %d\n", WSAGetLastError());
    return 1;
  }

  return 0;
}

int cleanup_winsock() {
  return WSACleanup();
}