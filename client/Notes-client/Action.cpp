#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include "Dialog.h"
#include "Action.h"

int defineAction(int action_numb, Action &action_type)
{
	int ret_val = 0;
	switch (action_numb)
	{
	case Registrate:
		action_type = Registrate;
		break;
	case Login:
		action_type = Login;
		break;
	case AddNotes:
		action_type = AddNotes;
		break;
	case RemoveNotes:
		action_type = RemoveNotes;
		break;
	case ModifyNotes:
		action_type = ModifyNotes;
		break;
	default:
		ret_val = 1;
		break;
	}
	return ret_val;
}

int registrateAndLogin(SOCKET &sock)
{
	char *password = (char*)calloc(PASSWORD_MAX_LEN, sizeof(char));
	char *action = (char*)calloc(ACTION_STR_LEN, sizeof(char));
	char *user_name = (char*)calloc(USER_NAME_MAX_LEN, sizeof(char));
	Action action_type = Registrate;
	int tmp;
	bool in_system = false;
	int recv_res, res;

	do
	{
		do
		{
			startMsg();
			userInput("Ваш выбор(учитывается только первый введённый символ): ", action, ACTION_STR_LEN);
			tmp = atoi(action);
			res = defineAction(tmp, action_type);
			system("cls");
			if (res || (action_type != Registrate && action_type != Login))
			{
				printf_s("Введено некорректное значение\n\n");
			}
		} while (res || (action_type != Registrate && action_type != Login));
		system("cls");

		res = send(sock, (char*)&action_type, sizeof(action_type), 0);
		if (res == SOCKET_ERROR)
		{
			printf("send failed with error: %ld\n", WSAGetLastError());
			free(user_name);
			free(password);
			free(action);
			return 1;
		}

		userInput("Логин и пароль не могут быть больше 30 символов\nИмя учётной записи: ", user_name, USER_NAME_MAX_LEN);
		res = send(sock, user_name, strlen(user_name), 0);
		if (res == SOCKET_ERROR)
		{
			printf("send failed with error: %ld\n", WSAGetLastError());
			free(user_name);
			free(password);
			free(action);
			return 1;
		}

		userInput("Пароль: ", user_name, USER_NAME_MAX_LEN);
		res = send(sock, user_name, strlen(user_name), 0);
		if (res == SOCKET_ERROR)
		{
			printf("send failed with error: %ld\n", WSAGetLastError());
			free(user_name);
			free(password);
			free(action);
			return 1;
		}

		recv_res = recv(sock, (char*)&in_system, sizeof(in_system), 0);
		if (recv_res == SOCKET_ERROR && recv_res == 0)
		{
			if (recv_res == SOCKET_ERROR)
			{
				printf("send failed with error: %ld\n", WSAGetLastError());
			}
			else
			{
				printf("Сервер закрыл соединение\n");
			}
			free(user_name);
			free(password);
			free(action);
			return 1;
		}
		if (!in_system)
		{
			printf_s("Что-то пошло не так. Возможно вы ввели некорректные данные. Попробуйте ещё раз\n");
		}

	} while (!in_system);
	return 0;
}

char **createNotesArr(char *buff, int &len)
{
	len = 10;
	char **Result = (char**)malloc(len * sizeof(char*));
	int i = 0, TokenLen;
	char *NextToken = NULL;
	char *Token = strtok_s(buff, NOTES_DELIMITER, &NextToken);
	while (Token != NULL)
	{
		if (i == len)
		{
			len = len * 2 + 1;
			Result = (char**)realloc(Result, len * sizeof(char*));
		}
		TokenLen = strlen(Token);
		Result[i] = (char*)malloc((TokenLen + 1) * sizeof(char));
		strcpy_s(Result[i++], TokenLen + 1, Token);

		Token = strtok_s(NULL, NOTES_DELIMITER, &NextToken);
	}

	len = i;
	Result = (char**)realloc(Result, len * sizeof(char*));
	return Result;
}

void getNotes(SOCKET &sock, char ***notes, int &len)
{
	for (int i = 0; i < len; i++)
	{
		free((*notes)[i]);
	}
	free(*notes);

	int buff_len = NOTES_MAX_LEN;
	char *buff = (char*)calloc(buff_len, sizeof(char));
	
	int tmp_len = buff_len;
	char *tmp = buff;

	int recv_res, select_res;
	timeval delay = { 1, 0 };
	fd_set read_sock = { 1, sock };
	int curr_pos = 0;
	do
	{
		select_res = select(0, &read_sock, NULL, NULL, &delay);
		if (select_res > 0)
		{
			recv_res = recv(sock, tmp, tmp_len, 0);
			tmp += recv_res;
			tmp_len -= recv_res;
			if (tmp_len == 0)
			{
				buff_len = 2 * buff_len + 1;
				buff = (char*)realloc(buff, buff_len * sizeof(char));
				tmp_len = buff_len - tmp_len;
				ZeroMemory(tmp, tmp_len);
			}
		}
	} while (select_res > 0);

	(*notes) = createNotesArr(buff, len);
	free(buff);
}

void displayNotes(char **notes, int len)
{
	if ((len == 1) && (strcmp(notes[0], NOTES_NOT_FOUND) == 0))
	{
		printf_s("\t&%s\n\n", notes[0]);
		return;
	}

	for (int i = 0; i < len; i++)
	{
		printf_s("%d) %s\n\n", i + 1, notes[i]);
	}
}