#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "WorkWithFiles.h"
#include "Actions.h"

int registerUser(SOCKET sock, bool &in_system)
{
	int recv_res, res, ret_val = 0;
	char *user_name = (char*)calloc(USER_NAME_MAX_LEN, sizeof(char));
	char *password = (char*)calloc(PASSWORD_MAX_LEN, sizeof(char));

	recv_res = recv(sock, user_name, USER_NAME_MAX_LEN - 1, 0);
	if (recv_res != SOCKET_ERROR && recv_res != 0)
	{
		user_name = (char*)realloc(user_name, recv_res + 1);

		recv_res = recv(sock, password, PASSWORD_MAX_LEN - 1, 0);
		if (recv_res != SOCKET_ERROR && recv_res != 0)
		{
			password = (char*)realloc(password, recv_res + 1);
			if (!registeredUser(user_name, password))
			{
				res = addNewUser(user_name, password);
				if (!res)
				{
					in_system = true;
				}
				else
				{
					in_system = false;
				}
			}
			else
			{
				in_system = false;
			}
			/*send answer*/
			res = send(sock, (char*)in_system, sizeof(in_system), 0);
			if (res == SOCKET_ERROR)
			{
				ret_val = 1;
			}
			ret_val = 0;
		}
		else
		{
			ret_val = 1;
		}
	}
	else
	{
		ret_val = 1;
	}

	free(user_name);
	free(password);
	return ret_val;
}

int loginUser(SOCKET sock, bool &in_system)
{
	int recv_res, res, ret_val = 0;
	char *user_name = (char*)calloc(USER_NAME_MAX_LEN, sizeof(char));
	char *password = (char*)calloc(PASSWORD_MAX_LEN, sizeof(char));

	recv_res = recv(sock, user_name, USER_NAME_MAX_LEN - 1, 0);
	if (recv_res != SOCKET_ERROR && recv_res != 0)
	{
		user_name = (char*)realloc(user_name, recv_res + 1);

		recv_res = recv(sock, password, PASSWORD_MAX_LEN - 1, 0);
		if (recv_res != SOCKET_ERROR && recv_res != 0)
		{
			password = (char*)realloc(password, recv_res + 1);
			if (registeredUser(user_name, password))
			{
				in_system = true;
			}
			else
			{
				in_system = false;
			}
			/*send answer*/
			res = send(sock, (char*)in_system, sizeof(in_system), 0);
			if (res == SOCKET_ERROR)
			{
				ret_val = 1;
			}
			ret_val = 0;
		}
		else
		{
			ret_val = 1;
		}
	}
	else
	{
		ret_val = 1;
	}

	free(user_name);
	free(password);
	return ret_val;
}