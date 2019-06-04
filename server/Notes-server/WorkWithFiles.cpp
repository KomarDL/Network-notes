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
	memcpy_s(result, len + 1, user_name, strlen(user_name));
	strcat_s(result, len + 1, PATH_FOR_USERS_DATA);
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

	int res, result_curr_pos = 0;
	char *file_name = formFileName(user_name);
	char *notes = (char*)calloc(NOTES_MAX_LEN, sizeof(char));
	int read_lines_count = 0;
	long int pos_of_notes_for_del = -1;
	FILE *in;
	fopen_s(&in, file_name, "r+");
	while (!feof(in))
	{
		if (read_lines_count == number_of_notes - 1)
		{
			pos_of_notes_for_del = ftell(in);
		}
		res = fscanf_s(in, "%s\n", notes);
		if (res != EOF)
		{
			read_lines_count += res;
			if ((read_lines_count > number_of_notes) && (res != 0))
			{
				if (result_curr_pos == len)
				{
					len = 2 * len + 1;
					result = (char**)realloc(result, len);
					for (int i = result_curr_pos; i < len; i++)
					{
						result[i] = (char*)calloc(NOTES_MAX_LEN, sizeof(char));
					}
				}
				strcpy_s(result[result_curr_pos], NOTES_MAX_LEN, notes);
				result[result_curr_pos] = (char*)realloc(result[result_curr_pos], strlen(notes) + 1);
				result_curr_pos++;
			}

		}
	}
	result = (char**)realloc(result, result_curr_pos);
	*result_len = result_curr_pos;
	fclose(in);
	/*truncate file*/
	int hndl;
	_sopen_s(&hndl, "1.txt", _O_RDWR, _SH_DENYNO, _S_IREAD | _S_IWRITE);
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
		read_res = fscanf_s(in, "%s %s\n", existing_user_name, existing_password);
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
	if (ret_val != 0)
	{
		fprintf_s(in, "%s %s\n", new_user_name, password);
		fclose(in);
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
		fprintf_s(out, "%s\n", usefullNotes[i]);
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
		fprintf_s(out, "%s\n", usefullNotes[i]);
		free(usefullNotes[i]);
	}
	free(usefullNotes);
	fclose(out);
	return ret_val;
}