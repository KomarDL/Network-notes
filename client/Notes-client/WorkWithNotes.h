#pragma once
#include <WinSock2.h>

int sendReqToAddNotes(SOCKET sock, Action action_type);
int sendReqToRemoveNotes(SOCKET sock, Action action_type, char **notes, int len, bool &correct_input);
int sendReqToModifyNotes(SOCKET sock, Action action_type, char **notes, int len, bool &correct_input);
