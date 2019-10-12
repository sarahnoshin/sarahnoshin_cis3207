#include<unistd.h>  
//#include<conio.h>
#include<stdio.h> 
#include<stdlib.h>
#include <dirent.h> 
#include <time.h> 
#include <string.h>
#include <ctype.h>
#include <setjmp.h>


#if defined(_WIN32) || defined(_WIN64)
        const char* os = "Windows";
#else
#ifdef __linux
        const char* os = "Linux";
#else
        const char* os = "Unknown";
#endif
#endif

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
void show_dirs(char* dir){

	struct dirent *de;  // Pointer for directory entry 
  
    // opendir() returns a pointer of DIR type.  
    DIR *dr = opendir(dir); 
  
    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("Could not open given directory\n" ); 
        return ; 
    } 

    // for readdir() 
    while ((de = readdir(dr)) != NULL) 
            printf("\t%s\n", de->d_name); 
  
    closedir(dr);     
    return; 
}
//function to get substring according to position, with space as delimiter 
char* return_substring(char given_string[], int pos){
	int i,j,ctr;
	j=0; ctr=1;
	char temp[100];
    for(i=0;i<=(strlen(given_string));i++)
    {
        // if space or NULL found, assign NULL into newString[ctr]
        
        if(given_string[i]=='\0' && ctr<pos){
        	// printf("\n\n[ERROR] Wrong INPUT\n\n");
			// exit_shell();
			char *str = (char *) malloc(sizeof(char) * 2);
			str[0]=' ';
			str[1]='\0';
    		return str;
		}
        if(given_string[i]==' '||given_string[i]=='\0')
        {
            temp[j]='\0';
            if(ctr == pos){
				// puts(temp);
				char *str = (char *) malloc(sizeof(char) * (j+1));
				for(i=0;i<=j;i++)
				str[i]=temp[i];
				str = strstrip1(str);
				// puts(str);
    			return str;
			}
            
            ctr++;  //for next word
            j=0;    //for next word, init index to 0
        }
        else
        {
            temp[j]=given_string[i];
			if(given_string[i]==' ' && given_string[i+1]==' ')
			i++;
            j++;
        }
	}
}

// function to change directory

void change_dir(char* dir){
	char s[100]; 
  
  
    // using the command 
    chdir(dir); 
  
    // printing current working directory 
    printf("Current working directory %s\n", getcwd(s, 100)); 
  
    // after chdir is executed 
    return ; 
}
void show_help(char* str){

	if(strcmp(str,"all") == 0){
		puts("\t -- quit --no_argument\tused to exit from the shell");
		puts("\t -- cd --argument\tIused to change the current working directory to the given folder \n\tif no arguemnt is passed current folder is changed to home folder");
		puts("\t -- clr --no_argument\tused to clear the screen");
		puts("\t -- dir --argument\tused to get contents of a given folder\n\tif no arguemnt is passed then content of current folder is shown");
		puts("\t -- environ --no_argument\tused to get list of enviroment strings");
		puts("\t -- echo --argument\tused to prit the arguemnt to the shell");
		puts("\t -- pause --no_argument\tused to pause the shell temporarily");
		puts("\t -- cwd --no_argument\tused to get the path of the current working directory");
	}
	else if (strcmp(str,"quit") == 0)
	{
		puts("\t -- quit --no_argument\tused to exit from the shell");
	}
	else if (strcmp(str,"cd") == 0)
	{
		puts("\t -- cd --argument\tIused to change the current working directory to the given folder \n\tif no arguemnt is passed current folder is changed to home folder");
	}
	else if (strcmp(str,"clr") == 0)
	{
		puts("\t -- clr --no_argument\tused to clear the screen");
	}
	else if (strcmp(str,"dir") == 0)
	{
		puts("\t -- dir --argument\tused to get contents of a given folder\n\tif no arguemnt is passed then content of current folder is shown");
	}
	else if (strcmp(str,"environ") == 0)
	{
		puts("\t -- environ --no_argument\tused to get list of enviroment strings");
	}
	else if (strcmp(str,"echo") == 0)
	{
		puts("\t -- echo --argument\tused to print the arguemnt within \" \" to the shell. Any thing outside of \" \" is ignored. More than one space or tabs are ignored.");
	}
	else if (strcmp(str,"pause") == 0)
	{
		puts("\t -- pause --no_argument\tused to pause the shell temporarily");
	}
	else if (strcmp(str,"cwd") == 0)
	{
		puts("\t -- cwd --no_argument\tused to get the path of the current working directory");
	}
	else	
	{
		puts("\tCould not find");
	}
	
}
//implementation of echo function
void echo_function(char* str){
	int i =0, j=0, flag = 0;
	char temp[200];
	
	while(str[i]!='\0'){
		if(str[i]=='"' && flag==0){
			flag = 1;
			i++;
		}
		if(flag == 1){
			// printf("\n Heheeree\n");
			if(str[i]=='"'){
				temp[j]='\0';
				puts(temp);
				return;
			}
			temp[j]=str[i];
			
			j++;
		}
		i++;
	}
}

// main decsion making function that calls each and every function avalable in the shell
// it can be said the main interpreter of the input commands from user 
void routing_function(char* str, char * envp[]){
	
	char s[100]; 
	str = strstrip1(str);
	if(strcmp(return_substring(str,1),"quit") == 0){
		
		exit_shell();
	}
	else if(strcmp(return_substring(str,1),"cd") == 0){
		change_dir(return_substring(str,2));
	}
	else if(strcmp(return_substring(str,1),"clr") == 0){
		if(strcmp(os,"Linux") == 0){
			//for linux
			system("clear");
		}
		else if(strcmp(os,"Windows") == 0){
			// for windows
			system("cls");
		}
		else{
			  printf("Functionality not available");
		}
		
	}
	else if(strcmp(return_substring(str,1),"dir") == 0){
		if(strcmp(return_substring(str,2)," ") == 0){
			show_dirs(".");
		}
		else
		{
			show_dirs(return_substring(str,2));
		}
		
		
	}
	else if(strcmp(return_substring(str,1),"environ") == 0){
		printf("\n");
		int i = 0;
		for (i = 0; envp[i] != NULL; i++) 
        printf("\n%s", envp[i]); 
        printf("\nshell=");
        puts(strcat(getcwd(s, 100), "/myshell"));
        printf("\n\n"); 
	}
	else if(strcmp(return_substring(str,1),"echo") == 0){
		echo_function(str);
	}
	else if(strcmp(return_substring(str,1),"help") == 0){
		if(strcmp(return_substring(str,2)," ") == 0){
			show_help("all");
		}
		else
		{
			show_help(return_substring(str,2));
		}
	}
	else if(strcmp(return_substring(str,1),"pause") == 0){
		char c;
		do{
			c = getchar();
		}while(c != '\n');
	}
	else if(strcmp(return_substring(str,1),"cwd") == 0){
		
	  	printf("%s\n", getcwd(s, 100));
	}
	else
	{
		printf("Wrong INPUT given\n");
	}
}

// main driver programme
 int main(int argc, char **argv, char * envp[])
{
	//
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
	char command[100]; 
	int i,j,n ;
	// for shell mode
	if (argc == 1)
	{
		while(1){
			printf("myshell> ");
			//gets(command); 
			fgets(command, 100, stdin);
			n = strlen(command);
			i =0;
			while (i < n)
			{
				if(command[i]=='\t')
				{
					command[i] = ' ';
				}					
				i++;
    		}
			i =0;
			while (i < n)
			{
				if(command[i]==' ' && (command[i+1]==' ' || command[i-1]==' '))
				{
					for(j=i;j<n;j++)
					command[j]=command[j+1];
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
	else if (argc  == 2){
		printf("\n\n[INFO] Batch file execution initiated ..\n");
		static jmp_buf s_jumpBuffer;
		delay_time(1);
		printf("\n\n[INFO] Checking the batch file\n");
		delay_time(1);
		//---------------------------------------------------------------------
			if( access( argv[1], F_OK ) != -1 ) {
				printf("\n\n[OK] File is good! Starting execution\n\n");
				delay_time(2);
			} else {
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
				i =0;
				while (i < n)
				{
					if(line[i]=='\t')
					{
						line[i] = ' ';
					}					
					i++;
				}
				i =0;
				while (i < n)
				{
					if(line[i]==' ' && (line[i+1]==' ' || line[i-1]==' '))
					{
						for(j=i;j<n;j++)
						line[j]=line[j+1];
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

/*


printf("TO EXECUTE THE PROGRAM AS A BASIC SHELL");
printf("Execute the command-  ./myshell ");
printf("To check ");

printf("TO EXECUTE THE PROGRAM AS A BATCH FILE");
printf("./myshell batch_filename.txt");
*/

