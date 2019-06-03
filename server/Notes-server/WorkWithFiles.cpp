#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Actions.h"
#include "WorkWithFiles.h"

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
			ret_val = ((strcmp(existing_user_name, new_user_name) == 0) && (strcmp(existing_password, password) == 0));
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