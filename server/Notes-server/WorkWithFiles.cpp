#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <share.h>
#include <io.h>
#include "Actions.h"
#include "WorkWithFiles.h"

char *formFileName(char *user_name)
{
	int len = strlen(user_name) + strlen(PATH_FOR_USERS_DATA) + strlen(USER_DATA_FILE_EXT);
	char *result = (char*)calloc(len + 1, sizeof(char));
	memcpy_s(result, len + 1, PATH_FOR_USERS_DATA, strlen(PATH_FOR_USERS_DATA));
	strcat_s(result, len + 1, user_name);
	strcat_s(result, len + 1, USER_DATA_FILE_EXT);
	return result;
}

/*get usefull content and truncate the file to the specified note inclusive*/
char **getNotesToWriteToTheEnd(int number_of_notes, char *user_name, int *result_len)
{
	int len = 10;
	char **result = (char**)calloc(len, sizeof(char*));
	for (int i = 0; i < len; i++)
	{
		result[i] = (char*)calloc(NOTES_MAX_LEN, sizeof(char));
	}

	int result_curr_pos = 0;
	char *tmp;
	char *file_name = formFileName(user_name);
	char *notes = (char*)calloc(NOTES_MAX_LEN + 1, sizeof(char));
	int read_lines_count = 0;
	long int pos_of_notes_for_del = -1;
	FILE *in;
	fopen_s(&in, file_name, "r+");
	while (!feof(in))
	{
		if (read_lines_count == number_of_notes)
		{
			pos_of_notes_for_del = ftell(in);
		}
		tmp = fgets(notes, NOTES_MAX_LEN + 1, in);
		if (tmp != NULL)
		{
			read_lines_count++;
			if (read_lines_count > number_of_notes + 1)
			{
				if (result_curr_pos == len)
				{
					len = 2 * len + 1;
					result = (char**)realloc(result, len * sizeof(char*));
					for (int i = result_curr_pos; i < len; i++)
					{
						result[i] = (char*)calloc(NOTES_MAX_LEN, sizeof(char));
					}
				}
				strcpy_s(result[result_curr_pos], NOTES_MAX_LEN, notes);
				result[result_curr_pos] = (char*)realloc(result[result_curr_pos], (strlen(notes) + 1) * sizeof(char));
				result_curr_pos++;
			}

		}
	}
	result = (char**)realloc(result, result_curr_pos * sizeof(char*));
	*result_len = result_curr_pos;
	fclose(in);
	/*truncate file*/
	int hndl;
	_sopen_s(&hndl, file_name, _O_RDWR, _SH_DENYNO, _S_IREAD | _S_IWRITE);
	_chsize_s(hndl, pos_of_notes_for_del);
	_close(hndl);

	free(notes);
	return result;
}

bool registeredUser(char *new_user_name, char *password)
{
	bool ret_val = false;
	int read_res;
	char *existing_user_name = (char*)calloc(USER_NAME_MAX_LEN, sizeof(char));
	char *existing_password = (char*)calloc(PASSWORD_MAX_LEN, sizeof(char));
	
	FILE *in;
	fopen_s(&in, USERS_FILE_NAME, "r");
	while (!feof(in) && !ret_val)
	{
		read_res = fscanf_s(in, "%s %s\n", existing_user_name, USER_NAME_MAX_LEN, existing_password, PASSWORD_MAX_LEN);
		if (read_res != EOF)
		{
			ret_val = (strcmp(existing_user_name, new_user_name) == 0);
		}
	}

	fclose(in);
	free(existing_user_name);
	free(existing_password);
	return ret_val;
}

int addNewUser(char *new_user_name, char *password)
{
	int ret_val = 0;
	FILE *in;
	ret_val = fopen_s(&in, USERS_FILE_NAME, "a");
	if (ret_val == 0)
	{
		fprintf_s(in, "%s %s\n", new_user_name, password);
		fclose(in);
		/*creat file for new user*/
		char *file_name = formFileName(new_user_name);
		fopen_s(&in, file_name, "w");
		fclose(in);

		free(file_name);
	}
	else
	{
		ret_val = 1;
	}
	return ret_val;
}

void addNewNotes(char *notes_buff, char *user_name)
{
	char *user_file_name = formFileName(user_name);
	FILE *out;
	fopen_s(&out, user_file_name, "a");
	fprintf_s(out, "%s\n", notes_buff);
	fclose(out);
	free(user_file_name);
}

int deleteNotes(int number_of_notes, char *user_name)
{
	int len, ret_val = 0;
	char **usefullNotes = getNotesToWriteToTheEnd(number_of_notes, user_name, &len);
	char *file_name = formFileName(user_name);
	FILE *out;
	fopen_s(&out, file_name, "a");
	for (int i = 0; i < len; i++)
	{
		fprintf_s(out, "%s", usefullNotes[i]);
		free(usefullNotes[i]);
	}
	free(usefullNotes);
	fclose(out);
	return ret_val;
}

int changeNotes(int number_of_notes, char *user_name, char *notes)
{
	int len, ret_val = 0;
	char **usefullNotes = getNotesToWriteToTheEnd(number_of_notes, user_name, &len);
	char *file_name = formFileName(user_name);
	FILE *out;
	fopen_s(&out, file_name, "a");
	fprintf_s(out, "%s\n", notes);
	for (int i = 0; i < len; i++)
	{
		fprintf_s(out, "%s", usefullNotes[i]);
		free(usefullNotes[i]);
	}
	free(usefullNotes);
	fclose(out);
	return ret_val;
}

int sendNotes(SOCKET sock, bool in_system, char *user_name)
{
	int ret_val = 0;
	int send_res = 1;
	if (in_system)
	{
		char *file_name = formFileName(user_name);
		char *notes = (char*)calloc(NOTES_MAX_LEN + 1, sizeof(char));
		int res;
		FILE *in;
		int notes_sended_count = 0;
		res = fopen_s(&in, file_name, "r");
		char *tmp;
		while (!feof(in) && send_res != SOCKET_ERROR)
		{
			tmp = fgets(notes, NOTES_MAX_LEN + 1, in);
			if (tmp != NULL)
			{
				notes_sended_count++;
				send_res = send(sock, notes, strlen(notes), 0);
				if (send_res == SOCKET_ERROR)
				{
					ret_val = 1;
				}
			}
		}
		fclose(in);
		if (!ret_val && notes_sended_count == 0)
		{
			send_res = send(sock, NOTES_NOT_FOUND, strlen(NOTES_NOT_FOUND), 0);
			if (send_res == SOCKET_ERROR)
			{
				ret_val = 1;
			}
		}
	}
	return ret_val;
}