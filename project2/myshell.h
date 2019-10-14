
#include <unistd.h>  
#include <stdio.h> 
#include <stdlib.h>
#include <dirent.h> 
#include <time.h> 
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define ARGS 20
#define BUFFERSIZE 128

#if defined(linux) || defined(__linux) || defined(__linux__)
        #define os "Linux"
#elif defined(_WIN32) || defined(_WIN64)
        const char* os = "Windows";
#else
        const char* os = "Unknown";
#endif


int get_nargs(char* cmdline, char* args[]);

int get_nargs_pipe(char* cmdline, char* args[]);

int has_output(char* args[], int nargs);

int has_output_append(char* args[], int nargs);

int has_input(char* args[], int nargs);

int has_input_output(char* args[], int nargs);

void process_output(char* args[], int nargs, char* filename);

void process_output_append(char* args[], int nargs, char* filename);

void process_input(char* args[], int nargs, char* filename);

void process_input_output(char* args[], int nargs, char* inFilename, char* outFilename);

void process_pipe(char* cmdline, char** args[], int nargs[], int npipes);

char *strstrip1(char *s);

void delay_time(int second);

void exit_shell();

void show_dirs(char* dir);

char* return_substring(char given_string[], int pos);

void change_dir(char* dir);

void show_help(char* str);

void echo_function(char* str);

void routing_function(char* str, char * envp[]);

