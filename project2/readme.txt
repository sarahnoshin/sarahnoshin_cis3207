This is a program to work as a shell with limited functionality in it. Following is detailed description of program and instructions how to run this shell program.
Open terminal in linux then change directory using ‘cd’ command to make that directory as current directory of terminal which has program files i.e. makefile, myshell.h, myshell.c and utility.c
Then type command ‘make’ and press enter. It will compiler source code files into runnable program file. It will create runnable file name ‘myshell’
There are two ways to run myshell program.
1. Type ‘./myshell’ and press enter. It will run the program that will start prompting user to enter commands to execute in shell. 
2. Type ‘./myshell batchfile’ and press enter. Here 2nd argument is batchfile that contains the commands to run in shell. Program read that batch file and process commands and exit the shell.
In this shell program you can execute following commands.
a. cd <directory> - Change the current default directory to
<directory>. If the <directory> argument is not present,
report the current directory. If the directory does not
exist an appropriate error should be reported. This command
should also change the PWD environment variable.
b. clr - Clear the screen.
c. dir <directory> - List the contents of directory
<directory>.
d. environ - List all the environment strings.
e. echo <comment> - Display <comment> on the display followed
by a new line (multiple spaces/tabs may be reduced to a
single space).
f. help - Display the user manual using the more filter.
g. pause - Pause operation of the shell until 'Enter' is
pressed.
h. quit - Quit the shell.
i. I/O redirections. Examples: 
Output Redirection: dir > out , this command list all the files in current working directory into output file named out. dir >> out , this command will create a file out if it does not exist and write result of dir command, if file exists then it will append results to end of file.
Input Redirectoin: sort < names , this command read input from names file, sort them and display on terminal.
j. command containing | symbol called pipes, for example ‘ls -lt | head -n 2’ list top 2 recently modified files. technique is that ‘ls -lt’ output is used as input for ‘head -n 2’
k. background task using & symbol with commands. Using & symbol make the process run in background. You are free to keep working in same terminal or even if you exit process will continue until it has been completed.

