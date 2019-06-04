#pragma once

#define USERS_FILE_NAME "users.txt"
#define PATH_FOR_USERS_DATA "user/"
#define USER_DATA_FILE_EXT ".txt"

bool registeredUser(char *new_user_name, char *password);
int addNewUser(char *new_user_name, char *password);
void addNewNotes(char *notes_buff, char *user_name);
int deleteNotes(int number_of_notes, char *user_name);
int changeNotes(int number_of_notes, char *user_name, char *notes);