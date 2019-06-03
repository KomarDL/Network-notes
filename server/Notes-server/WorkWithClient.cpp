#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <list>
#include "WorkWithClient.h"
#include "WorkWithFiles.h"
#include "Actions.h"

Action defineAction(char *action)
{
	return (Action)(action[0] - '0');
}

//return 1 if errors occurs
int performAction(SOCKET sock, Action action_type, bool &in_system)
{
	int res;
	switch (action_type)
	{
	case Registrate:
		res = registerUser(sock, in_system);
		break;
	case Login:
		res = loginUser(sock, in_system);
		break;
	case AddNotes:
		res = addNotes(sock);
		break;
	case RemoveNotes:
		res = removeNotes(sock);
		break;
	case ModifyNotes:
		res = modifyNotes(sock);
		break;
	}
	return res;
}

void workWithClient(SOCKET client_sock)
{
	int recv_res, res;
	Action action_type;
	bool in_system = false;
	do
	{
		recv_res = recv(client_sock, (char*)&action_type, sizeof(action_type), 0);
		if (recv_res != 0 && recv_res != SOCKET_ERROR)
		{
			res = performAction(client_sock, action_type, in_system);
			if (res)
			{
				recv_res = 0;
			}
		}
	} while (recv_res != 0 && recv_res != SOCKET_ERROR);
	shutdown(client_sock, SD_BOTH);
	closesocket(client_sock);
}