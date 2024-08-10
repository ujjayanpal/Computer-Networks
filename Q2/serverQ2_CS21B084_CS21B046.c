#include <stdio.h>
#include <locale.h> // for setlocale
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <dirent.h>
#include <strings.h>
#include <sys/time.h>
#include <sys/types.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t post = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t get = PTHREAD_MUTEX_INITIALIZER;


// Structure to hold the request data
struct Request {
    char Directory[4096];
    int client_Socket;
    char FileType[4096];

};


// Function to send the requested file

void* sending_data(void* args) {
    pthread_mutex_lock(&get);
    struct Request* request = (struct Request*)args;
    int clientSocket = request->client_Socket;
    char FilePath[1024];
    strcpy(FilePath, request->Directory);
    printf("Requested file: %s\n",FilePath);
     
    FILE* file = fopen(FilePath, "rb"); 
    printf(FilePath);
    if (file == NULL) {
        // File not found, send 404 Not Found response
        printf("File not found\n");
        char* response = "HTTP/1.1 404 Not Found\r\n\r\n";
        send(clientSocket, response, strlen(response), 0);
        
    } else {


        char response[4096];
        memset(response,0,sizeof(response));
        snprintf(response, sizeof(response), "HTTP/1.1 200 OK\r\nContent-Type: ");
        strncat(response, request->FileType, strlen(request->FileType));
        strncat(response, "\r\n\r\n", strlen(response)+4);
        send(clientSocket, response, strlen(response), 0);
// Read file content and send in chunks
char data[1024];
size_t bytes_read;
while ((bytes_read = fread(data, 1, sizeof(data), file)) > 0) {
    send(clientSocket, data, bytes_read, 0);
}
fclose(file);
printf("%s FILE sending is completed\n",request->FileType);



        
    }
    pthread_mutex_unlock(&get);

    return NULL;
}

// Function to handle the client request
void* HandleRequest(void* args) {
    int clientSocket =(*((struct Request*)args)).client_Socket;
    char directory[100];
    char tempdirectory[100];
    int received = 1;
    int start_timer =clock(),current_timer;
    {
    char *buffer = (char*)malloc(1024*sizeof(char)) ;

        current_timer = clock();
        if((current_timer-start_timer)/CLOCKS_PER_SEC>10)
        {
            printf("Timeout\n");
            // break;
        }
       received = recv(clientSocket, buffer, 1024, 0);
        printf("%d Recieving------------------------------------------------\n",received);

    if (received == -1) {
        perror("Error receiving data");
        // close(clientSocket);
        // pthread_exit(NULL);
    }


    printf("------------------------------------------------\n");

    // Print the entire received data (might be garbled)
    printf("Raw data (may contain encoding issues):\n%.*s\n", received, buffer);
    char *buffer_copy = (char*)malloc(1024*sizeof(char));
    strcpy(buffer_copy,buffer);

    // Parse the request (method and path)
    char* method = strtok(buffer, " ");  // Get the first token (method)

    if (method == NULL) {
        // Handle invalid request format
        printf("Method Invalid request format\n");
        close(clientSocket);
        // pthread_exit(NULL);
    }
    else{
    if(strcmp(method,"GET")==0)
    {
        start_timer = clock();
    
    char* path = strtok(NULL, " ");  // Get the second token (file path)
    if (path == NULL) {
        // Handle invalid request format
        printf("Path Invalid request format\n");
        // close(clientSocket);
        // pthread_exit(NULL);
    }
    else {
    // Open the requested file
    if(strcmp(path,"/")==0)
    {
        strcpy(path,"/index.html");
    }
    strcpy(directory,(*((struct Request*)args)).Directory);
    strcpy(tempdirectory,(*((struct Request*)args)).Directory);

    strncat(directory, path, strlen(path));
     char *newpath = strtok(path," / .\"");
     
    struct Request request;
     while(newpath!=NULL)
    {
        if(strcmp(newpath,"html")==0 || strcmp(path,"htm")==0)
        {
            strcpy(request.FileType ,"text/html");break;
        }
        else if(strcmp(newpath,"txt")==0)
        {
            strcpy(request.FileType ,"text/plain");break;
        }
        else if(strcmp(newpath,"css")==0)
        {
            strcpy(request.FileType ,"text/css");break;
        }
        else if(strcmp(newpath,"js")==0)
        {
            strcpy(request.FileType ,"text/javascript");break;
        }
        else if(strcmp(newpath,"jpg")==0)
        {
            strcpy(request.FileType ,"image/jpeg");break;
        }
        else if(strcmp(newpath,"jpeg")==0)
        {
            strcpy(request.FileType ,"image/jpeg");break;
        }
        else if(strcmp(newpath,"png")==0)
        {
            strcpy(request.FileType ,"image/png");break;
        }
        else if(strcmp(newpath,"gif")==0)
        {
            strcpy(request.FileType ,"image/gif");break;
        }
        else if(strcmp(newpath,"mp3")==0)
        {
            strcpy(request.FileType ,"audio/mpeg");break;
        }
        else if(strcmp(newpath,"mp4")==0)
        {
            strcpy(request.FileType ,"video/mp4");break;
        }
        else if(strcmp(newpath,"pdf")==0)
        {
            strcpy(request.FileType ,"application/pdf");break;
        }
        else if(strcmp(newpath,"doc")==0)
        {
            strcpy(request.FileType ,"application/msword");break;
        }
        else if(strcmp(newpath,"docx")==0)
        {
            strcpy(request.FileType ,"application/vnd.openxmlformats-officedocument.wordprocessingml.document");break;
        }
        else if(strcmp(newpath,"ppt")==0)
        {
            strcpy(request.FileType ,"application/vnd.ms-powerpoint");break;
        }
        else if(strcmp(newpath,"pptx")==0)
        {
            strcpy(request.FileType ,"application/vnd.openxmlformats-officedocument.presentationml.presentation");break;
        }
        else if(strcmp(newpath,"xls")==0)
        {
            strcpy(request.FileType ,"application/vnd.ms-excel");break;
        }
        else if(strcmp(newpath,"xlsx")==0)
        {
            strcpy(request.FileType ,"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");break;
        }
        else if(strcmp(newpath,"zip")==0)
        {
            strcpy(request.FileType ,"application/zip");break;
        }
        else if(strcmp(newpath,"rar")==0)
        {
            strcpy(request.FileType ,"application/vnd.rar");break;
        }
        else if(strcmp(newpath,"tar")==0)
        {
            strcpy(request.FileType ,"application/x-tar");break;
        }
        else if(strcmp(newpath,"gz")==0)
        {
            strcpy(request.FileType ,"application/gzip");break;
            

        }
        else if(strcmp(newpath,"7z")==0)
        {
            strcpy(request.FileType ,"application/x- 7z-compressed");break;
            
        }
         newpath = strtok(NULL," / .\"");


    }
    printf("\nDataset completely sent by the browser: %s\n ",newpath);

    strcpy(request.Directory,directory);
    request.client_Socket = clientSocket;
    FILE* fcheck = fopen(directory, "rb");
    if(fcheck==NULL) //file not found sending the 404.html
    {
        printf("File not found\n");
        // char* response = "HTTP/1.1 404 Not Found\r\n\r\n";
        strncat(tempdirectory , "/404.html", 9);
        strcpy(request.Directory,tempdirectory);
        sending_data((void*)&request);
        // send(clientSocket, response, strlen(response), 0);
    }
    else
    {
    pthread_t thread;
    // sending_data((void*)&request);
    pthread_create(&thread, NULL, sending_data, (void*)&request);
    pthread_join(thread, NULL);
    printf("File path: %s\n", directory);
    // Send an empty line to terminate the response
        // send(clientSocket, "\r\n", 2, 0);
    }
    }
    }
    else if(strcmp(method,"POST")==0)  //POST request
    {
        pthread_mutex_lock(&post);
        char * str = NULL;
        printf("POST request\n");
        str = strstr(buffer_copy ,"Content-Disposition: form-data;");
        char* ptr =strtok(str,"%**%");
        ptr = strtok(NULL,"%**%");
        printf("String :%s",ptr);
        char response[4096];
        int char_count = strlen(ptr);

        int word_count = 0;
        
        for(int i=0;i<strlen(ptr);i++)
        {

            if(ptr[i]==' '||ptr[i]=='\n')
            {
                word_count++;
            }
        
        }
        if(ptr!=NULL && strlen(ptr)>0)
        {

            word_count++;
        }
        int sentence_count = 0;
        for(int i=0;i<strlen(ptr);i++)
        {

            if(ptr[i]=='.' || ptr[i]=='?' || ptr[i]=='!' || ptr[i]=='\n')
            {
                sentence_count++;
            }
        }
       if(ptr!=NULL && strstr(ptr,"\n")!=NULL)
       {
        sentence_count++;
       }
       memset(response,0,sizeof(response));

        snprintf(response, sizeof(response), "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
                                          "<html><body>"
                                          "<h2>Character Count: %d</h2>"
                                          "<h2>Word Count: %d</h2>"
                                          "<h2>Sentence Count: %d</h2>"
                                          "</body></html>",
             char_count, word_count, sentence_count);
    send(clientSocket, response, strlen(response), 0);
        pthread_mutex_unlock(&post);

    }
    else
    {
        printf("Invalid request\n");
    }
    
    free(buffer);
}

  printf("Client disconnected\n");
    close(clientSocket);
    return NULL;
}
}


int main(int argc,char* argv[])
{
    int sockfd;
    int port;
     struct sockaddr_in clientAddr;
    sockfd = socket(AF_INET,SOCK_STREAM,0);  //socket creation having 3 arguments 1. domain 2. transport type 3. protocol
    int clientSocket;
    int clientAddrLen = sizeof(clientAddr);
    struct Request request;
    port = atoi(argv[2]);
    strcpy( request.Directory,argv[1]);
    printf("Port: %d\n",port);
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
        perror("Error in binding");
        exit(1);
    }
    printf("Server is binded\n");
    //server is binded now we will listen to the client

    listen(sockfd,5); //here 5 is the maximum number of clients that can be connected to the server at a time.

    //accepting the client
  int connected = 1;
    while(connected>0)
    {
        clientSocket = accept(sockfd, (struct sockaddr*)&clientAddr,(socklen_t*) &clientAddrLen);
        if(clientSocket == -1)
        {
            perror("Error in accepting the client");
            exit(1);
        }
        printf("Client is connected\n");
        request.client_Socket = clientSocket;
        //now we will create a thread for each client
        pthread_t thread;
        pthread_create(&thread, NULL, HandleRequest, (void*)&request); //create the thread for each clients 
        pthread_join(thread, NULL);
        printf("Client handled\n");
    }
}
