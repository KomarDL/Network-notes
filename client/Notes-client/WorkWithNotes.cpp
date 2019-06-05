
#include "Dialog.h"
#include "Action.h"
#include "WorkWithNotes.h"

int sendReqToAddNotes(SOCKET sock, Action action_type)
{
	int ret_val = 0;
	int send_res = send(sock, (char*)&action_type, sizeof(action_type), 0);
	if (send_res == SOCKET_ERROR)
	{
		ret_val = 1;
	}
	else
	{
		char *new_notes = (char*)calloc(NOTES_MAX_LEN, sizeof(char));
		userInput("Текст новой заметки(все символы после 255 игнорируются): ", new_notes, NOTES_MAX_LEN);
		new_notes = (char*)realloc(new_notes, strlen(new_notes) + 1);
		send_res = send(sock, new_notes, strlen(new_notes) + 1, 0);
		if (send_res == SOCKET_ERROR)
		{
			ret_val = 1;
		}
		free(new_notes);
	}
	return ret_val;
}

int sendReqToRemoveNotes(SOCKET sock, Action action_type, char **notes, int len, bool &correct_input)
{
	int ret_val = 0;
	int notes_number = -1;
	correct_input = getValidNotesNumber(notes, len, notes_number);
	if (correct_input)
	{
		int send_res = send(sock, (char*)&action_type, sizeof(action_type), 0);
		if (send_res == SOCKET_ERROR)
		{
			ret_val = 1;
		}
		else
		{
			send_res = send(sock, (char*)&notes_number, sizeof(notes_number), 0);
			if (send_res == SOCKET_ERROR)
			{
				ret_val = 1;
			}
		}
	}
	return ret_val;
}

int sendReqToModifyNotes(SOCKET sock, Action action_type, char **notes, int len, bool &correct_input)
{
	int ret_val = 0;
	int notes_number = -1;
	correct_input = getValidNotesNumber(notes, len, notes_number);
	if (correct_input)
	{
		int send_res = send(sock, (char*)&action_type, sizeof(action_type), 0);
		if (send_res == SOCKET_ERROR)
		{
			ret_val = 1;
		}
		else
		{
			send_res = send(sock, (char*)&notes_number, sizeof(notes_number), 0);
			if (send_res == SOCKET_ERROR)
			{
				ret_val = 1;
			}
			else
			{
				char *new_notes = (char*)calloc(NOTES_MAX_LEN + 1, sizeof(char));
				userInput("Введите новый текст заметки: ", new_notes, NOTES_MAX_LEN + 1);
				send_res = send(sock, new_notes, strlen(new_notes), 0);
				if (send_res == SOCKET_ERROR)
				{
					ret_val = 1;
				}
				free(new_notes);
			}
		}
	}
	return ret_val;
}