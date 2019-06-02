#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <thread>
#include <ws2tcpip.h>
#include <list>

#include "WorkWithClient.h"
#include "SendBroadcast.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")

int main()
{
	char b[] = "1";
	Action a = (Action)(b[0] - '0');
	printf_s("%d\n", a);
	getchar();
	return 0;

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf_s("Error");
		exit(1);
	}


	std::thread send_broadcast_thread = std::thread(sendBroadcastUDP);
	send_broadcast_thread.detach();
	
	int res;

	struct addrinfo *result = NULL, *ptr = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	res = getaddrinfo(NULL, DEFAULT_PORT_STR, &hints, &result);
	if (res != 0) {
		printf("getaddrinfo failed: %d\n", res);
		WSACleanup();
		return 1;
	}

	SOCKET listen_sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listen_sock == INVALID_SOCKET)
	{
		printf_s("Unable to accept incoming connections");
		getchar();
		return 1;
	}

	res = bind(listen_sock, result->ai_addr, (int)result->ai_addrlen);
	if (res == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(listen_sock);
		WSACleanup();
		return 1;
	}

	if (listen(listen_sock, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(listen_sock);
		WSACleanup();
		return 1;
	}
	
	std::list<std::thread> thread_pool;
	std::list<SOCKET> client_sock_list;
	while (true)
	{
		client_sock_list.push_back(accept(listen_sock, NULL, NULL));
	
		if (client_sock_list.back() == INVALID_SOCKET)
		{
			printf("accept failed: %d\n", WSAGetLastError());
			client_sock_list.pop_back();
			continue;
		}

		thread_pool.push_back(std::thread(workWithClient, client_sock_list.back()));
	}
	
	return 0;
}