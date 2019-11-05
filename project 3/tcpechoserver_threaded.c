#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>


#define NUM_WORKERS 10  //how many pending connections queue  will hold
#define DEFAULT_DICTIONARY "/usr/share/dict/words"
#define DEFAULT_PORT 64154
#define WORD_SIZE 50

struct Node{
    char* word;
    struct Node* next;
};

struct Node *head = NULL;
sem_t *mutex;

void push(char *data)
{
   struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
   new_node->word = (char*)malloc(sizeof(char)*WORD_SIZE);
   strcpy(new_node->word,data);
   new_node->next = NULL;

   if(head == NULL)
   {
      head = new_node;
   }
   else if(head->next == NULL)
   {
      head->next = new_node;
   }
   else
   {
      struct Node* temp_node = head->next;
      while(temp_node->next != NULL)
      {
         temp_node = temp_node->next;
      }
      temp_node->next = new_node;
   } 
}

int search(char *data)
{
   if(head == NULL)
   {
      fprintf(stderr,"here");
      return -1;
   }
   else
   {
      struct Node* temp_node = head;
      while(temp_node != NULL)
      {
         //fprintf(stderr,"%s\n",temp_node->word);
         if(strcmp(data,temp_node->word) == 0)
         {
            return 1;
         }
         temp_node = temp_node->next;
      }
   } 
   return -1;
}

void print()
{
   struct Node* temp_node = head;
      while(temp_node != NULL)
      {
         fprintf(stderr,"%s %ld\n",temp_node->word,strlen(temp_node->word));
         temp_node = temp_node->next;
      }
}

void* tcpechod(void *fd);

int main(int argc, char** argv)
{
   FILE* fp = NULL;
   mutex = sem_open("/sem1", O_CREAT, 0666, 1);  //initialize the semaphore/mutex
   char *word = (char*)malloc(sizeof(char)*WORD_SIZE);
   if(argc == 2) //open the dictionary
   {
      fp = fopen(argv[1],"r");
   }
   else
   {
      fp = fopen(DEFAULT_DICTIONARY,"r");
   }
   while( fscanf(fp, "%s", word) != EOF) // read the dictionary and save to linked list
   {
      push(word);
   }
   
   int server_sockfd;//socket on which server process will listen for incoming con
   int client_sockfd; //socket on which the server will be comm with the client
   struct sockaddr_in server_addr;
   struct sockaddr_in client_addr;

//*** Create passive socket for the server
   server_sockfd = socket (AF_INET, SOCK_STREAM, 0);

//*** Create a address structure containing server IP addr
//*** and port, then bind the server_sockfd with this address
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(DEFAULT_PORT);
   inet_aton("127.0.0.1", &server_addr.sin_addr);
   memset(&(server_addr.sin_zero), '\0', sizeof server_addr.sin_zero);
   bind(server_sockfd, (struct sockaddr*)&server_addr, sizeof server_addr);
//*** Create a connection queue and wait for clients
   listen(server_sockfd, NUM_WORKERS);
   fprintf(stderr,"Listening...\n"); 
   while(1)
   {
//*** Accept a connection, blocks until connection from client is established
//****will return a brand new descriptor for comm with this single connection
      int client_len = sizeof client_addr;
      client_sockfd=accept(server_sockfd,(struct sockaddr*)&client_addr,&(client_len));
      pthread_t tid;
      pthread_attr_t ta;
      pthread_attr_init(&ta);
      pthread_attr_setdetachstate(&ta, PTHREAD_CREATE_DETACHED);
      pthread_create(&tid, &ta, &tcpechod, (void*)&client_sockfd);  
   }
   return 0;
}
void* tcpechod(void * fd)
{
   int client_sockfd = *((int*)fd);
   char *buf = (char*)malloc(sizeof(char)*WORD_SIZE);
   memset(buf,'\0',WORD_SIZE);
   int rv;
   char *response = (char*)malloc(sizeof(char)*WORD_SIZE);
   fprintf(stderr, "\n********* CLIENT CONNECTION ESTABLISHED ********");
   while (rv = read(client_sockfd, buf, WORD_SIZE)) // read the client data
   {
      buf[rv] = '\0';
      if(search(buf) == -1) //search the word
      {
         memset(response,'\0',WORD_SIZE); //if word not found in dictionary
         strcat(response,buf);
         strcat(response," ");
         strcat(response,"MISPELLED\0");
      }
      else  ////if word found in dictionary
      {
         memset(response,'\0',WORD_SIZE);
         strcat(response,buf);
         strcat(response," ");
         strcat(response,"OK\0");      
      }
      write(client_sockfd, response, strlen(response)); //send response back to client socket
      memset(buf,'\0',WORD_SIZE);
      
      //synchronization part
      sem_wait(mutex); //lock the mutex
      FILE* file = file = fopen("log.txt","a"); //open the log file
      fputs(response,file); // write response
      fputc('\n',file);
      fclose(file); //close the file
      sem_post(mutex); //unlock the mutex
   }
   close(client_sockfd); //close the client socket
   fprintf(stderr, "\n********* CLIENT CONNECTION TERMINATED ********");
   pthread_exit(NULL); //exit the thread
}
