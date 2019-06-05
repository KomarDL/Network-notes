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
#define ACTION_STR_LEN 2
#define NOTES_DELIMITER "\n"

int defineAction(int action_numb, Action &action_type);
int registrateAndLogin(SOCKET &sock);
void getNotes(SOCKET &sock, char ***notes, int &len);
void displayNotes(char **notes, int len);