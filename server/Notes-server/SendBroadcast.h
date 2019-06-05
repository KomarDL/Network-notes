#pragma once
#include <winsock2.h>

#define IP_ADAPTER_BUFFER_SIZE 15000
#define DEFAULT_PORT 27015
#define DEFAULT_PORT_STR "27015"

ULONG getBroadcastAddress(PULONG local_machine_IP);

void sendBroadcastUDP();