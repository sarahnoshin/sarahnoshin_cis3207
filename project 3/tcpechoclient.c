#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv)
{
   if(argc!=3)
   {
      printf("Must enter IP and port\n");
      exit(1);
   }
//*** STEP-I:  Create a socket
   int sockfd = socket(PF_INET, SOCK_STREAM, 0);
//*** Populate Socket's DS for remote IP and Port, and
//*** let the local IP and Port be selected by the OS itself
   struct sockaddr_in dest_addr;
   dest_addr.sin_family = AF_INET;    
   dest_addr.sin_port = htons(atoi(argv[2]));
   inet_aton(argv[1], &dest_addr.sin_addr);
   memset(&(dest_addr.sin_zero), '\0', sizeof dest_addr.sin_zero); 
//*** Connect this socket to the server's socket
   connect(sockfd, (struct sockaddr*)& dest_addr, sizeof dest_addr);
//*** Client reads string from stdin and send it to server
//*** then read string returned by server and display on stdout
   char buff1[128]={'\0'},buff2[128] = {'\0'};
   while(1)
   {
      write(1,"\nEnter word to check spelling: ",32); //input the word from user
      int n = read(0, buff1, 128);   
      write(sockfd, buff1, n-1); //send it to server
      n = read(sockfd, buff2, 128); //read response of client
      write(1, buff2, n); //write the response on screen
   }
//*** Close socket
   close(sockfd);
   exit(0);
}
