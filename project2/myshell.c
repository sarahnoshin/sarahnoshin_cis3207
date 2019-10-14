#include "myshell.h"

int get_nargs(char* cmdline, char* args[])
{
	int i = 0;

	/* if no args */
	if ((args[0] = strtok(cmdline, "\n\t ")) == NULL)
		return 0;

	while ((args[++i] = strtok(NULL, "\n\t ")) != NULL) {
		if (i >= ARGS) {
			printf("Too many arguments!\n");
			exit(1);
		}
	}
	/* the last one is always NULL */
	return i;
}

int get_nargs_pipe(char* cmdline, char* args[])
{
	int i = 0;

	/* if no args */
	if ((args[0] = strtok(cmdline, "|")) == NULL)
	{
		return 0;
	}

	while ((args[++i] = strtok(NULL, "|")) != NULL) {
		if (i >= ARGS) {
			printf("Too many arguments!\n");
			exit(1);
		}
	}
	/* the last one is always NULL */
	return i;
}

// returns index if has '>' symbol, -1 otherwise
int has_output(char* args[], int nargs)
{
	int i = 0;
	for (i = 0; i < nargs; i++)
	{
		if (strcmp(args[i], ">") == 0)
			return i;
	}
	return -1;
}

// returns index if has '>>' symbol, -1 otherwise
int has_output_append(char* args[], int nargs)
{
	int i = 0;
	for (i = 0; i < nargs; i++)
	{
		if (strcmp(args[i], ">>") == 0)
			return i;
	}
	return -1;
}

// returns index if has '<' symbol, -1 otherwise
int has_input(char* args[], int nargs)
{
	int i = 0;
	for (i = 0; i < nargs; i++)
	{
		if (strcmp(args[i], "<") == 0)
			return i;
	}
	return -1;
}

// returns 1 if args have '<' and '>' or symbols, 0 otherwise
int has_input_output(char* args[], int nargs)
{
	if (has_output(args, nargs) != -1 && has_input(args, nargs) != -1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void process_output(char* args[], int nargs, char* filename)
{
	int pid, async;
	// if its & symbol then async call
	if (!strcmp(args[nargs - 1], "&")) { async = 1; args[--nargs] = 0; }
	else async = 0;

	// open file to write output
	int fd1 = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

	pid = fork();
	// child process
	if (pid == 0)
	{
		if (dup2(fd1, 1) != 1)
		{
			perror("Error: dup2(fd1, 1)\n");
			exit(1);
		}
		close(fd1);

		execvp(args[0], args);
		// return if exec failed
		perror("exec failed");
		exit(-1);
	}
	// parent process
	else if (pid > 0)
	{
		if (!async)
			waitpid(pid, NULL, 0);
	}
	else
	{
		perror("fork failed");
		exit(1);
	}
}

void process_output_append(char* args[], int nargs, char* filename)
{
	int pid, async;
	// if its & symbol then async call
	if (!strcmp(args[nargs - 1], "&")) { async = 1; args[--nargs] = 0; }
	else async = 0;

	// open file to write output
	int fd1 = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);

	pid = fork();
	// child process
	if (pid == 0)
	{
		if (dup2(fd1, 1) != 1)
		{
			perror("Error: dup2(fd1, 1)\n");
			exit(1);
		}
		close(fd1);

		execvp(args[0], args);
		// return if exec failed
		perror("exec failed");
		exit(-1);
	}
	// parent process
	else if (pid > 0)
	{
		if (!async)
			waitpid(pid, NULL, 0);

	}
	else
	{
		perror("fork failed");
		exit(1);
	}
}

void process_input(char* args[], int nargs, char* filename)
{
	int pid, async;
	// if its & symbol then async call
	if (!strcmp(args[nargs - 1], "&")) { async = 1; args[--nargs] = 0; }
	else async = 0;

	// open file to read output
	int fd1 = open(filename, O_RDONLY, 0644);

	pid = fork();
	// child process
	if (pid == 0)
	{
		if (dup2(fd1, 0) != 0)
		{
			perror("Error: can't read from file\n");
			exit(1);
		}
		close(fd1);

		execvp(args[0], args);
		// return if exec failed
		perror("exec failed");
		exit(-1);
	}
	// parent process
	else if (pid > 0)
	{
		if (!async)
			waitpid(pid, NULL, 0);

	}
	else {
		perror("fork failed");
		exit(1);
	}
}

void process_input_output(char* args[], int nargs, char* inFilename, char* outFilename)
{
	int pid, async;
	// if its & symbol then async call
	if (!strcmp(args[nargs - 1], "&")) { async = 1; args[--nargs] = 0; }
	else async = 0;

	// open file to read output
	int fd1 = open(inFilename, O_RDONLY, 0644);
	// open file to write output
	int fd2 = open(outFilename, O_WRONLY | O_CREAT | O_APPEND, 0644);

	pid = fork();
	// child process
	if (pid == 0)
	{
		// replace stdin with input file
		if (dup2(fd1, 0) != 0)
		{
			perror("Error: Can't read from file\n");
			exit(1);
		}
		close(fd1);

		// replace stdout with output file
		if (dup2(fd2, 1) != 1)
		{
			perror("Error: Can't write to file\n");
			exit(1);
		}
		close(fd2);

		execvp(args[0], args);
		// return if exec failed
		perror("exec failed");
		exit(-1);
	}
	// parent process
	else if (pid > 0)
	{
		if (!async)
			waitpid(pid, NULL, 0);

	}
	else {
		perror("fork failed");
		exit(1);
	}
}

void process_pipe(char* cmdline, char** args[], int nargs[], int npipes)
{
	if (npipes == 1)
	{
		int pipes[2];
		int pid;

		pipe(pipes);

		pid = fork();
		// child process
		if (pid == 0)
		{
			dup2(pipes[0], 0);
			close(pipes[1]);

			execvp(args[0][0], args[0]);
		}
		// parent process
		else
		{
			dup2(pipes[1], 1);
			close(pipes[0]);
			execvp(args[1][0], args[1]);
		}
	}
	else
	{
		system(cmdline);
	}
}

char *strstrip1(char *s)
{
	size_t size;
	char *end;

	size = strlen(s);

	if (!size)
		return s;

	end = s + size - 1;
	while (end >= s && isspace(*end))
		end--;
	*(end + 1) = '\0';

	while (*s && isspace(*s))
		s++;

	return s;
}


// delay function 
void delay_time(int second)
{
	sleep(1);
}

// function to exit from shell
void exit_shell()
{
	printf("\n[INFO] Closing shell..\n");
	delay_time(1);
	printf("[INFO] Doing final Checks..\n");
	delay_time(1);
	printf("[OK] All components checked.\n[INFO] EXITING ..\n");
	delay_time(1);
	exit(1);
}

//list of files and folder in current directory
void show_dirs(char* dir) {

	struct dirent *de;  // Pointer for directory entry 

	// opendir() returns a pointer of DIR type.  
	DIR *dr = opendir(dir);

	if (dr == NULL)  // opendir returns NULL if couldn't open directory 
	{
		printf("Could not open given directory\n");
		return;
	}

	// for readdir() 
	while ((de = readdir(dr)) != NULL)
		printf("\t%s\n", de->d_name);

	closedir(dr);
	return;
}

//function to get substring according to position, with space as delimiter 
char* return_substring(char given_string[], int pos) {
	int i, j, ctr;
	j = 0; ctr = 1;
	char temp[BUFFERSIZE];
	for (i = 0; i <= (strlen(given_string)); i++)
	{
		// if space or NULL found, assign NULL into newString[ctr]

		if (given_string[i] == '\0' && ctr < pos) {
			// printf("\n\n[ERROR] Wrong INPUT\n\n");
			// exit_shell();
			char *str = (char *)malloc(sizeof(char) * 2);
			str[0] = ' ';
			str[1] = '\0';
			return str;
		}
		if (given_string[i] == ' ' || given_string[i] == '\0')
		{
			temp[j] = '\0';
			if (ctr == pos) {
				// puts(temp);
				char *str = (char *)malloc(sizeof(char) * (j + 1));
				for (i = 0; i <= j; i++)
					str[i] = temp[i];
				str = strstrip1(str);
				// puts(str);
				return str;
			}

			ctr++;  //for next word
			j = 0;    //for next word, init index to 0
		}
		else
		{
			temp[j] = given_string[i];
			if (given_string[i] == ' ' && given_string[i + 1] == ' ')
				i++;
			j++;
		}
	}

	return "";
}

// function to change directory
void change_dir(char* dir) {
	char s[BUFFERSIZE];


	// using the command 
	chdir(dir);

	// printing current working directory 
	printf("Current working directory %s\n", getcwd(s, BUFFERSIZE));

	// after chdir is executed 
	return;
}

void show_help(char* str) {

	if (strcmp(str, "all") == 0) {
		puts("\t -- quit --no_argument\tused to exit from the shell");
		puts("\t -- cd --argument\tIused to change the current working directory to the given folder \n\tif no arguemnt is passed current folder is changed to home folder");
		puts("\t -- clr --no_argument\tused to clear the screen");
		puts("\t -- dir --argument\tused to get contents of a given folder\n\tif no arguemnt is passed then content of current folder is shown");
		puts("\t -- environ --no_argument\tused to get list of enviroment strings");
		puts("\t -- echo --argument\tused to prit the arguemnt to the shell");
		puts("\t -- pause --no_argument\tused to pause the shell temporarily");
		puts("\t -- cwd --no_argument\tused to get the path of the current working directory");
	}
	else if (strcmp(str, "quit") == 0)
	{
		puts("\t -- quit --no_argument\tused to exit from the shell");
	}
	else if (strcmp(str, "cd") == 0)
	{
		puts("\t -- cd --argument\tIused to change the current working directory to the given folder \n\tif no arguemnt is passed current folder is changed to home folder");
	}
	else if (strcmp(str, "clr") == 0)
	{
		puts("\t -- clr --no_argument\tused to clear the screen");
	}
	else if (strcmp(str, "dir") == 0)
	{
		puts("\t -- dir --argument\tused to get contents of a given folder\n\tif no arguemnt is passed then content of current folder is shown");
	}
	else if (strcmp(str, "environ") == 0)
	{
		puts("\t -- environ --no_argument\tused to get list of enviroment strings");
	}
	else if (strcmp(str, "echo") == 0)
	{
		puts("\t -- echo --argument\tused to print the arguemnt within \" \" to the shell. Any thing outside of \" \" is ignored. More than one space or tabs are ignored.");
	}
	else if (strcmp(str, "pause") == 0)
	{
		puts("\t -- pause --no_argument\tused to pause the shell temporarily");
	}
	else if (strcmp(str, "cwd") == 0)
	{
		puts("\t -- cwd --no_argument\tused to get the path of the current working directory");
	}
	else
	{
		puts("\tCould not find");
	}
}

//implementation of echo function
void echo_function(char* str) {
	int i = 0, j = 0, flag = 0;
	char temp[200];

	while (str[i] != '\0') {
		if (str[i] == '"' && flag == 0) {
			flag = 1;
			i++;
		}
		if (flag == 1) {
			// printf("\n Heheeree\n");
			if (str[i] == '"') {
				temp[j] = '\0';
				puts(temp);
				return;
			}
			temp[j] = str[i];

			j++;
		}
		i++;
	}
}

// main decsion making function that calls each and every function avalable in the shell
// it can be said the main interpreter of the input commands from user 
void routing_function(char* str, char * envp[]) {

	char s[BUFFERSIZE];
	str = strstrip1(str);

	char* args[ARGS];
	char* copy = str;
	//flag to check if args have pipe symbol
	int nargs = 0, flag = 0;

	if (strstr(str, "|"))
	{
		nargs = get_nargs_pipe(str, args);
		flag = 1;
	}

	if (!flag)
	{
		nargs = get_nargs(str, args);
	}



	if (strcmp(return_substring(str, 1), "quit") == 0) {
		exit_shell();
	}
	int flag_output = has_output(args, nargs);
	int flag_input = has_input(args, nargs);
	int flag_output_append = has_output_append(args, nargs);
	int flag_input_output = has_input_output(args, nargs);

	if (flag)
	{
		int i;
		// split args of pipe to further args (if possible)
		char** args2[ARGS];
		int nargs2[ARGS];

		for (i = 0; i < nargs; i++)
		{
			args2[i] = malloc(sizeof(char*) * BUFFERSIZE);
			nargs2[i] = get_nargs(args[i], args2[i]);
		}

		process_pipe(copy, args2, nargs2, nargs);

	}
	// check if args have '<' and '>' symbole
	else if (flag_input_output == 1)
	{
		// get index of input symbol
		int input_index = has_input(args, nargs);

		char inFilename[BUFFERSIZE];
		char outFilename[BUFFERSIZE];
		// get input file name
		strcpy(inFilename, args[input_index + 1]);

		// get index of output symbol
		int output_index = has_output(args, nargs);

		// get output file name
		strcpy(outFilename, args[output_index + 1]);

		// put empty string "" on input_index and next to input_index
		strcpy(args[input_index], "");
		strcpy(args[input_index + 1], "");
		// put empty string "" on output_index and next to output_index
		strcpy(args[output_index], "");
		strcpy(args[output_index + 1], "");


		char* args2[ARGS];
		// copy remianing arguments into new args array
		int i, index = 0;
		for (i = 0; i < nargs; i++)
		{
			if (strcmp(args[i], "") != 0)
			{
				args2[index] = malloc(sizeof(char*) * BUFFERSIZE);
				strcpy(args2[index++], args[i]);
			}
		}
		args2[index] = malloc(sizeof(char*) * BUFFERSIZE);
		args2[index] = NULL;
		process_input_output(args2, index, inFilename, outFilename);
	}
	// check if args has output to file symbol >
	else if (flag_output != -1)
	{
		// get file name after > symbol
		char* filename = args[flag_output + 1];
		// put null char at location of >
		args[flag_output] = NULL;

		process_output(args, nargs, filename);
	}
	else if (flag_output_append != -1)
	{
		// get file name after >> symbol
		char* filename = args[flag_output_append + 1];
		// put null char at location of >>
		args[flag_output_append] = NULL;

		process_output_append(args, nargs, filename);
	}
	else if (flag_input != -1)
	{
		// get file name after < symbol
		char* filename = args[flag_input + 1];
		// put null char at location of <
		args[flag_input] = NULL;

		process_input(args, nargs, filename);
	}
	else if (strcmp(return_substring(str, 1), "cd") == 0) {
		change_dir(return_substring(str, 2));
	}
	else if (strcmp(return_substring(str, 1), "clr") == 0) {
		if (strcmp(os, "Linux") == 0) {
			//for linux
			system("clear");
		}
		else if (strcmp(os, "Windows") == 0) {
			// for windows
			system("cls");
		}
		else {
			printf("Functionality not available");
		}

	}
	else if (strcmp(return_substring(str, 1), "dir") == 0) {
		if (strcmp(return_substring(str, 2), " ") == 0) {
			show_dirs(".");
		}
		else
		{
			show_dirs(return_substring(str, 2));
		}


	}
	else if (strcmp(return_substring(str, 1), "environ") == 0) {
		printf("\n");
		int i = 0;
		for (i = 0; envp[i] != NULL; i++)
			printf("\n%s", envp[i]);
		printf("\nshell=");
		puts(strcat(getcwd(s, BUFFERSIZE), "/myshell"));
		printf("\n\n");
	}
	else if (strcmp(return_substring(str, 1), "echo") == 0) {
		echo_function(str);
	}
	else if (strcmp(return_substring(str, 1), "help") == 0) {
		if (strcmp(return_substring(str, 2), " ") == 0) {
			show_help("all");
		}
		else
		{
			show_help(return_substring(str, 2));
		}
	}
	else if (strcmp(return_substring(str, 1), "pause") == 0) {
		char c;
		do {
			c = getchar();
		} while (c != '\n');
	}
	else if (strcmp(return_substring(str, 1), "cwd") == 0) {

		printf("%s\n", getcwd(s, BUFFERSIZE));
	}
	else
	{
		printf("Wrong INPUT given\n");
	}
}
