#include "myshell.h"

// main driver programme
int main(int argc, char **argv, char * envp[])
{
	char s[BUFFERSIZE];
	//	char* test;
	//	test = return_substring("Hello how afe you", 3);
	//	puts(test);
		// puts(os);
	printf("[INFO] Starting shell..\n");
	delay_time(1);
	printf("[INFO] Preparing all the commands \n");
	delay_time(1);
	printf("[OK] All components checked. \n");
	delay_time(1);
	// string variable to store input command
	char command[BUFFERSIZE];
	int i, j, n;
	// for shell mode
	if (argc == 1)
	{
		while (1) {
			printf("%s/myshell> ", getcwd(s, BUFFERSIZE));
			//gets(command); 
			fgets(command, BUFFERSIZE, stdin);
			n = strlen(command);
			i = 0;
			while (i < n)
			{
				if (command[i] == '\t')
				{
					command[i] = ' ';
				}
				i++;
			}
			i = 0;
			while (i < n)
			{
				if (command[i] == ' ' && (command[i + 1] == ' ' || command[i - 1] == ' '))
				{
					for (j = i; j < n; j++)
						command[j] = command[j + 1];
					n--;
				}
				else
				{
					i++;
				}
			}
			routing_function(command, envp);
		}

	}
	// for batch mode
	else if (argc == 2) {
		printf("\n\n[INFO] Batch file execution initiated ..\n");

		delay_time(1);
		printf("\n\n[INFO] Checking the batch file\n");
		delay_time(1);
		//---------------------------------------------------------------------
		if (access(argv[1], F_OK) != -1) {
			printf("\n\n[OK] File is good! Starting execution\n\n");
			delay_time(2);
		}
		else {
			printf("\n\n[ERROR] Could not open file exiting shell!!\n");
			delay_time(1);
			exit_shell();
		}
		char const* const fileName = argv[1]; /* should check that argc > 1 */
		FILE* file = fopen(fileName, "r"); /* should check the result */
		char line[256];

		while (fgets(line, sizeof(line), file)) {

			// puts(line);
			n = strlen(line);
			i = 0;
			while (i < n)
			{
				if (line[i] == '\t')
				{
					line[i] = ' ';
				}
				i++;
			}
			i = 0;
			while (i < n)
			{
				if (line[i] == ' ' && (line[i + 1] == ' ' || line[i - 1] == ' '))
				{
					for (j = i; j < n; j++)
						line[j] = line[j + 1];
					n--;
				}
				else
				{
					i++;
				}
			}
			routing_function(line, envp);
			delay_time(1);
		}

		fclose(file);

		return 0;
		//---------------------------------------------------------------------
	}
	// raising an exception when wring input is given
	else
	{
		printf("\n\n[ERROR] Can execute only one batch file at a time.\n[INFO] Exiting shell ..\n\n");
		exit_shell();
	}
	return 0;
}
