#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <locale.h>
#include "RecvBroadcast.h"
#include "Action.h"
#include "Dialog.h"
#include "WorkWithNotes.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")

int main()
{
	setlocale(LC_ALL, "Rus");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf_s("Error");
		exit(1);
	}

	int res;
	SOCKET sock = INVALID_SOCKET;
	res = recvBroadcastUDP(sock);
	if (res)
	{
		WSACleanup();
		return 1;
	}

	res = registrateAndLogin(sock);
	if (res)
	{
		closesocket(sock);
		WSACleanup();
		getchar();
		return 1;
	}

	char **notes = (char**)calloc(1, sizeof(char*));
	int len = 1, tmp;
	getNotes(sock, &notes, len);

	char *action = (char*)calloc(ACTION_STR_LEN, sizeof(char));
	Action action_type;
	bool correct_input = false;
	system("cls");

	displayNotes(notes, len);
	displayMenu();

	do
	{
		do
		{
			userInput("Ваш выбор(учитывается только первый введённый символ): ", action, ACTION_STR_LEN);
			tmp = atoi(action);
			res = defineAction(tmp, action_type);
		} while (res || (action_type == Registrate && action_type == Login));
		system("cls");
		displayNotes(notes, len);

		switch (action_type)
		{
		case AddNotes:
			res = sendReqToAddNotes(sock, action_type);
			getNotes(sock, &notes, len);
			break;
		case RemoveNotes:
			res = sendReqToRemoveNotes(sock, action_type, notes, len, correct_input);
			if (correct_input && !res)
			{
				getNotes(sock, &notes, len);
			}
			break;
		case ModifyNotes:
			res = sendReqToModifyNotes(sock, action_type, notes, len, correct_input);
			if (correct_input && !res)
			{
				getNotes(sock, &notes, len);
			}
			break;
		}
		system("cls");
		displayNotes(notes, len);
		displayMenu();
	} while (!res);

	system("cls");
	printf_s("Работа с заметками не может быть продолжена тк сервер закрыл соединение.\nПопробуйте позже");
	getchar();

	shutdown(sock, SD_BOTH);
	closesocket(sock);
	WSACleanup();

	return 0;
}