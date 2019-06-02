#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <iphlpapi.h>
#include "SendBroadcast.h"

ULONG getBroadcastAddress(PULONG local_machine_IP)
{
	ULONG broadcast_addr = 0;
	ULONG flags = GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER | GAA_FLAG_INCLUDE_GATEWAYS;
	ULONG buffer_size = IP_ADAPTER_BUFFER_SIZE;
	PIP_ADAPTER_ADDRESSES adr = (PIP_ADAPTER_ADDRESSES)malloc(buffer_size), curr_adr;

	ULONG ret_val = GetAdaptersAddresses(AF_INET, flags, NULL, adr, &buffer_size);
	curr_adr = adr;
	if (ret_val == NO_ERROR)
	{
		BOOL gateway_IP_found = FALSE;
		while ((curr_adr != NULL) || (gateway_IP_found))
		{
			/*wprintf_s(L"Description: %s\n", curr_adr->Description);
			wprintf_s(L"Friendly name: %s\n", curr_adr->FriendlyName);*/
			gateway_IP_found = (curr_adr->FirstGatewayAddress != NULL);
			if (gateway_IP_found)
			{
				ULONG subnet_mask = 0;
				ConvertLengthToIpv4Mask(curr_adr->FirstUnicastAddress->OnLinkPrefixLength, &subnet_mask);
				/*char str[16];
				inet_ntop(AF_INET, &subnet_mask, str, 16);
				printf_s("Subnet mask: %s\n", str);*/

				sockaddr_in *IPInAddr = (sockaddr_in*)curr_adr->FirstUnicastAddress->Address.lpSockaddr;
				(*local_machine_IP) = IPInAddr->sin_addr.s_addr;
				/*inet_ntop(AF_INET, local_machine_IP, str, 16);
				printf_s("My IP address: %s\n", str);*/

				broadcast_addr = (*local_machine_IP) | (~subnet_mask);
				/*inet_ntop(AF_INET, &broadcast_addr, str, 16);
				printf_s("Broadcast address: %s\n\n\n", str);*/
			}
			curr_adr = curr_adr->Next;
		}
	}
	else
	{
		LPVOID msg_buff = NULL;
		printf("Call to GetAdaptersAddresses failed with error: %d\n", ret_val);
		if (ret_val == ERROR_NO_DATA)
			printf("\tNo addresses were found for the requested parameters\n");
		else
		{

			if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, ret_val, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),/* Default language*/(LPTSTR)&msg_buff, 0, NULL))
			{
				printf("\tError: %s", (char*)msg_buff);
			}
		}
	}

	free(adr);
	return broadcast_addr;
}

void sendBroadcastUDP()
{
	ULONG server_IP;
	ULONG broadcast_addr = getBroadcastAddress(&server_IP);
	int res = 0;
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET)
	{
		printf_s("Socket failed with error %d\n", WSAGetLastError());
		return;
	}

	sockaddr_in adr;
	adr.sin_family = AF_INET;
	adr.sin_port = htons((USHORT)DEFAULT_PORT);
	adr.sin_addr.s_addr = broadcast_addr;
	while (true)
	{
		//implicit bind function call
		res = sendto(sock, (char *)&server_IP, sizeof(server_IP), 0, (SOCKADDR *)&adr, sizeof(adr));
		if (res == SOCKET_ERROR) {
			printf("sendto failed with error: %d\n", WSAGetLastError());
			closesocket(sock);
			WSACleanup();
			return;
		}
		Sleep(5000);
	}
	return;
}