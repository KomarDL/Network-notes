#pragma once
#include <WinSock2.h>

typedef enum _Action
{
	Registrate = 1,
	Login,
	AddNotes,
	RemoveNotes,
	ModifyNotes
} Action;

#define USER_NAME_MAX_LEN 31
#define PASSWORD_MAX_LEN USER_NAME_MAX_LEN
#define NOTES_MAX_LEN 256

int registerUser(SOCKET sock, bool &in_system, char **user_name);
int loginUser(SOCKET sock, bool &in_system, char **user_name);
int addNotes(SOCKET sock, bool in_system, char *user_name);
int removeNotes(SOCKET sock, bool in_system, char *user_name);
int modifyNotes(SOCKET sock, bool in_system, char *user_name);