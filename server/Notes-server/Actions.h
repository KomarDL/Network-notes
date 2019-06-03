#pragma once
#include <WinSock2.h>

typedef enum _Action
{
	Registrate,
	Login,
	AddNotes,
	RemoveNotes,
	ModifyNotes
} Action;

#define USER_NAME_MAX_LEN 31
#define PASSWORD_MAX_LEN USER_NAME_MAX_LEN

int registerUser(SOCKET sock, bool &in_system);
int loginUser(SOCKET sock, bool &in_system);
