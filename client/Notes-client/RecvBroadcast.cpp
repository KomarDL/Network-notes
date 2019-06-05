#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "RecvBroadcast.h"

int recvBroadcastUDP(SOCKET &sock)
{
	int ret_val = 0, res;

	struct addrinfo *result = NULL, *ptr = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the client
	res = getaddrinfo(NULL, DEFAULT_PORT_STR, &hints, &result);
	if (res != 0)
	{
		printf("getaddrinfo failed: %d\n", res);
		ret_val = 1;
	}
	else
	{
		SOCKET listen_sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (listen_sock == INVALID_SOCKET)
		{
			printf_s("Unable to accept incoming connections");
			ret_val = 1;
		}
		else
		{
			res = bind(listen_sock, result->ai_addr, (int)result->ai_addrlen);
			if (res == SOCKET_ERROR)
			{
				printf("bind failed with error: %d\n", WSAGetLastError());
				ret_val = 1;
			}
			else
			{
				ULONG server_IP;
				res = recv(listen_sock, (char*)&server_IP, sizeof(ULONG), 0);
				if (res == SOCKET_ERROR)
				{
					printf("recv failed with error: %d\n", WSAGetLastError());
					ret_val = 1;
				}
				else
				{
					sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
					if (sock == INVALID_SOCKET)
					{
						printf("socket failed with error: %d\n", WSAGetLastError());
						ret_val = 1;
					}
					else
					{
						sockaddr_in adr;
						adr.sin_family = AF_INET;
						adr.sin_port = htons((USHORT)DEFAULT_PORT);
						adr.sin_addr.s_addr = server_IP;

						res = connect(sock, (SOCKADDR*)&adr, sizeof(adr));
						if (res == SOCKET_ERROR)
						{
							printf("connect failed with error: %d\n", WSAGetLastError());
							closesocket(sock);
							ret_val = 1;
						}
					}
				}
			}
		}
		freeaddrinfo(result);
	}
	return ret_val;
}
