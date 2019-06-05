#pragma once
#include <WinSock2.h>

#define NUMBER_OF_NOTES_BUFF_SIZE 8

int sendReqToAddNotes(SOCKET sock, Action action_type);