#pragma once
#include <winsock2.h>

#define DEFAULT_PORT 27015
#define DEFAULT_PORT_STR "27015"

int recvBroadcastUDP(SOCKET &sock);