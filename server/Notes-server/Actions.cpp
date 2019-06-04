#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "WorkWithFiles.h"
#include "Actions.h"

int registerUser(SOCKET sock, bool &in_system, char **user_name)
{
	int recv_res, res, ret_val = 0;
	char *password = (char*)calloc(PASSWORD_MAX_LEN, sizeof(char));

	recv_res = recv(sock, *user_name, USER_NAME_MAX_LEN - 1, 0);
	if (recv_res != SOCKET_ERROR && recv_res != 0)
	{
		*user_name = (char*)realloc(*user_name, recv_res + 1);

		recv_res = recv(sock, password, PASSWORD_MAX_LEN - 1, 0);
		if (recv_res != SOCKET_ERROR && recv_res != 0)
		{
			password = (char*)realloc(password, recv_res + 1);
			if (!registeredUser(*user_name, password))
			{
				res = addNewUser(*user_name, password);
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
			res = send(sock, (char*)&in_system, sizeof(in_system), 0);
			if (res == SOCKET_ERROR)
			{
				ret_val = 1;
			}
			else
			{
				ret_val = 0;
			}
			
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

	free(password);
	return ret_val;
}

int loginUser(SOCKET sock, bool &in_system, char **user_name)
{
	int recv_res, res, ret_val = 0;
	char *password = (char*)calloc(PASSWORD_MAX_LEN, sizeof(char));

	recv_res = recv(sock, *user_name, USER_NAME_MAX_LEN - 1, 0);
	if (recv_res != SOCKET_ERROR && recv_res != 0)
	{
		*user_name = (char*)realloc(*user_name, recv_res + 1);

		recv_res = recv(sock, password, PASSWORD_MAX_LEN - 1, 0);
		if (recv_res != SOCKET_ERROR && recv_res != 0)
		{
			password = (char*)realloc(password, recv_res + 1);
			if (registeredUser(*user_name, password))
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

	free(password);
	return ret_val;
}

int addNotes(SOCKET sock, bool in_system, char *user_name)
{
	int ret_val = 0;
	if (in_system)
	{
		int recv_res, total_recv_res;
		char *notes_buff = (char*)calloc(NOTES_MAX_LEN, sizeof(char));
		char *tmp = notes_buff;
		int tmp_len = NOTES_MAX_LEN - 1;

		do
		{
			recv_res = recv(sock, tmp, tmp_len, 0);
			if (recv_res != SOCKET_ERROR && recv_res != 0)
			{
				total_recv_res += recv_res;
				tmp += recv_res;
				tmp_len -= recv_res;

			}
			else
			{
				ret_val = 1;
				total_recv_res = NOTES_MAX_LEN;
			}
		} while (total_recv_res < NOTES_MAX_LEN && notes_buff[total_recv_res - 1] != '\0');
		
		if (!ret_val)
		{
			addNewNotes(notes_buff, user_name);
		}
	}
	return ret_val;
}

int removeNotes(SOCKET sock, bool in_system, char *user_name)
{
	int ret_val = 0, res;
	if (in_system)
	{
		int number_of_notes, recv_res;
		recv_res = recv(sock, (char*)&number_of_notes, sizeof(number_of_notes), 0);
		if (recv_res != SOCKET_ERROR && recv_res != 0)
		{
			res = deleteNotes(number_of_notes, user_name);
		}

	}
	return ret_val;
}

int modifyNotes(SOCKET sock, bool in_system, char *user_name)
{
	int ret_val = 0, res;
	if (in_system)
	{
		int number_of_notes, recv_res;
		recv_res = recv(sock, (char*)&number_of_notes, sizeof(number_of_notes), 0);
		if (recv_res != SOCKET_ERROR && recv_res != 0)
		{
			char *notes = (char*)calloc(NOTES_MAX_LEN, sizeof(char));
			recv_res = recv(sock, notes, NOTES_MAX_LEN, 0);
			notes = (char*)realloc(notes, strlen(notes) + 1);
			if (recv_res != SOCKET_ERROR && recv_res != 0)
			{
				res = changeNotes(number_of_notes, user_name, notes);
			}
			else
			{
				ret_val = 1;
			}
			free(notes);
		}
	}
	return ret_val;
}