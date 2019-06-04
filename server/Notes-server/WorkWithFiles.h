#pragma once
#include <WinSock2.h>

#define USERS_FILE_NAME "users.txt"
#define PATH_FOR_USERS_DATA "user/"
#define USER_DATA_FILE_EXT ".txt"
#define NOTES_NOT_FOUND "У вас ещё нет заметок"

bool registeredUser(char *new_user_name, char *password);
int addNewUser(char *new_user_name, char *password);
void addNewNotes(char *notes_buff, char *user_name);
int deleteNotes(int number_of_notes, char *user_name);
int changeNotes(int number_of_notes, char *user_name, char *notes);
int sendNotes(SOCKET sock, bool in_system, char *user_name);