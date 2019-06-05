#pragma once

#define NUMBER_OF_NOTES_BUFF_SIZE 8
#define NOTES_NOT_FOUND "У вас ещё нет заметок"

void startMsg();
void displayMenu();
void userInput(const char msg[], char *input_str, int str_len);
bool getValidNotesNumber(char **notes, int len);