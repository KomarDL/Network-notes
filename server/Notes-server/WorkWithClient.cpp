#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <list>
#include "WorkWithClient.h"
#include "Actions.h"

Action defineAction(char *action)
{
	return (Action)(action[0] - '0');
}

int performAction(SOCKET sock, char *action, bool &in_system)
{
	Action action_type = defineAction(action);
	switch (action_type)
	{
	case Registrate:

		break;
	case Login:

		break;
	case AddNotes:

		break;
	case RemoveNotes:

		break;
	case ModifyNotes:

		break;
	}
	return 0;
}

void workWithClient(SOCKET client_sock)
{
	int recv_res;
	char *recv_buff = (char*)calloc(RECV_BUFF_LEN, sizeof(char));
	bool in_system = false;
	do
	{
		recv_res = recv(client_sock, recv_buff, RECV_BUFF_LEN - 1, 0);
		if (recv_res == SOCKET_ERROR)
		{
			shutdown(client_sock, SD_BOTH);
			closesocket(client_sock);
		}
		else
		{
			performAction(client_sock, recv_buff, in_system);
		}
	} while (recv_res != 0 && recv_res != SOCKET_ERROR);
}