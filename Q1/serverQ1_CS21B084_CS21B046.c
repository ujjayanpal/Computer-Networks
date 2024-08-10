#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <dirent.h>
#include <strings.h>
#include <errno.h>
 int isPlaying = 0;
 int currentSong = 0;
  pthread_mutex_t incr_lock = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t decr_lock = PTHREAD_MUTEX_INITIALIZER;

int client_no = 0;
 struct Sc{
    int socket;
    int connected;
    char root_dir[100];
 };

void* input(void* arg) {
  int sockfd = ((struct Sc*)arg)->socket;
  int connected = ((struct Sc*)arg)->connected;
  char dir[100];

  FILE* fp;
  if (connected == -1) {
    printf("Error in accepting the data\n");
    // Exit could be handled differently here (e.g., restart server)
    return NULL;
  } else {
    printf("Server is ready to accept the data\n");
   int tr= send(connected, "Hello from server", 1024, 0); // TCP, sendto is for UDP
    if(tr!=-1)
    {char buffer[1024];

    if (recv(connected, buffer, 1024, 0) == -1 && errno == ECONNRESET) {
      printf("Client disconnected\n");
      close(connected);
      return NULL;
    } else {
      printf("Message from client: %s\n", buffer);

      {
        char song[20] = "audio";
        strcpy(dir, ((struct Sc*)arg)->root_dir);
        strncat(dir, "/", sizeof(dir) + sizeof(song));
        char message[20];
        printf("%s\n", dir);
        for (int i = 0; i < strlen(buffer); i++) {
          if (buffer[i] == '\n') {
            buffer[i] = '\0';
          }
        }
        strncat(song, buffer, sizeof(song) + sizeof(buffer));
        strncat(song, ".mp3", sizeof(song) + 3);
        strncat(dir, song, sizeof(dir) + sizeof(song));
        currentSong = atoi(buffer);
        printf("%s\n", dir);

        fp = fopen(dir, "rb");
        if (fp == NULL) {
          printf("File open failed\n");
          send(connected, "Choose another song please", 1024, 0);
        } else {
          size_t bytes_read;
          while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
            if(send(connected, buffer, bytes_read, 0)<=0)
            {
              close(connected);
            }
            usleep(1000);
            
          }
          
          fclose(fp);
      //    pthread_mutex_lock(&incr_lock);
          client_no--;
        //  pthread_mutex_unlock(&incr_lock);
        }
      }
    }
    }
  }

  close(connected);
  return NULL;
}



int main(int argc,char**argv)
{
    // pthread_t io;
    // pthread_create(&io,NULL,input,NULL);

    struct Sc link;
   long int port_number = 0;
    int MAX_client = 0;
    int port = atoi(argv[1]);
     MAX_client =atoi (argv[3]);
    char root_dir[100];
    strcpy(root_dir,argv[2]);
    strcpy(link.root_dir,root_dir);
    int sockfd;
     struct sockaddr_in clientAddr;
        char client[1031];
        int isthere = 0;
  
    sockfd = socket(AF_INET,SOCK_STREAM,0);  //socket creation having 3 arguments 1. domain 2. transport type 3. protocol
   
    //server binding now here we go
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr("10.22.21.252");
   int result = bind(sockfd, (struct sockaddr*)&server, sizeof(server)); //here we are binding the server to the socket created above.
                   //where sockfd is the socket descriptor, server is the address of the server and sizeof(server) is the size of struct;
                   //we did typecasting here because bind doesn't consider protocol which we are using IPV4 or IPV6it is, it just consider the address of the server.

 if(result == -1)
   {
       printf("Error in binding\n");
       exit(1);
   }
   else
   {
       printf("Server is binded\n");
   }

   if(listen(sockfd,5) == -1)
   {
       printf("Error in listening\n");
       exit(1);
   }
   else
   {
       printf("Server is ready to listen\n");
   }
   printf("MAx_Client:%d",MAX_client);
int client_Addrlen = sizeof(clientAddr);
char* arg[2];
arg[0] = (char*)sockfd;
   int connected = 2;
   link.socket=sockfd;
   int timer = 0;
  while(1)
  {   
 
     connected = accept(sockfd, (struct sockaddr*)&clientAddr, (socklen_t*)&client_Addrlen);//have to pass pointer to the length of the clients address
    printf("Server is connected to the client:%d,MAx_clients:%d",client_no,MAX_client);
 if(client_no < MAX_client)
   {
    if(connected==-1)
    {
        printf("Server can't connect to it");
    }
    else{
    
    char buffer[1024];
    printf("IP of client%d %s:%d\n",client_no,inet_ntoa(clientAddr.sin_addr),ntohs(clientAddr.sin_port)); //"Internet Address to ASCII" 
  link.connected=connected; 
         
    pthread_mutex_lock(&incr_lock);  
      
        printf("Song is playing with client_no = %d\n",client_no);
        client_no++;
        printf("Song is playing with client_no = %d\n",client_no);
       pthread_mutex_unlock(&incr_lock);
      pthread_t thread;
       int tp = pthread_create(&thread,NULL,input,(void*)&link);
        if(tp==-1)
        {printf("sgdfgfdhf");}
    

       
         
    }} 
  



  }
 


          
    return 0;
   
}
