#include <stdio.h>
#include <string.h>
#include "Action.h"
#include "Dialog.h"

void startMsg()
{
	printf_s("%d. Зарегистрироваться\n", Registrate);
	printf_s("%d. Войти\n", Login);
}

void displayMenu()
{
	printf_s("\n\n%d. Добавить заметку\n", AddNotes);
	printf_s("%d. Удалить заметку\n", RemoveNotes);
	printf_s("%d. Изменить заметку\n", ModifyNotes);
}

void userInput(const char msg[], char *input_str, int str_len)
{
	printf_s(msg);
	fgets(input_str, str_len, stdin);
	int last_symb_pos = strlen(input_str) - 1;
	if (last_symb_pos != 0 && input_str[last_symb_pos] == '\n')
	{
		input_str[last_symb_pos] = '\0';
	}

	if ((str_len - 2) == last_symb_pos)
	{
		while (getchar() != '\n');
	}
}

bool getValidNotesNumber(char **notes, int len, int &notes_number)
{
	bool ret_val = false;
	if (!(len == 1 && (strcmp(notes[0], NOTES_NOT_FOUND) == 0)))
	{
		char *buff = (char*)calloc(NUMBER_OF_NOTES_BUFF_SIZE, sizeof(char));
		notes_number = -1;
		do
		{
			userInput("Номер заметки(-1 для перехода в главное меню): ", buff, NUMBER_OF_NOTES_BUFF_SIZE);
			notes_number = atoi(buff) - 1;
			if ((notes_number > -1) && (notes_number < len))
			{
				ret_val = true;
			}
		} while ((!ret_val) || (notes_number == -2));
		
	}
	return ret_val;
}