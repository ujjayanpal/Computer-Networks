#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include <sys/select.h>
#include <errno.h>

#define BUFFER_SIZE 1024
int MAX_CLIENTS; 
int tout;
int bytes_received;
// Structure to hold client information
typedef struct {
    int socket;
    char username[50];
} Client;

// Global variables
Client* clients;
int num_clients = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Function prototypes
void *handle_client(void *arg);
void send_to_all(char *message, int sender_socket);
void remove_client(int index);

int server_socket;
struct sockaddr_in server_addr;
int b = 0;
    
int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <port> <max_clients> <timeout>\n", argv[0]);
        return 1;
    }
    MAX_CLIENTS = atoi(argv[2]);
    clients = (Client*)malloc(MAX_CLIENTS*sizeof(Client));
    int port = atoi(argv[1]); 
    tout = atoi(argv[3]);

    

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        return 1;
    }

    // Prepare the sockaddr_in structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        return 1;
    }

    // Listen
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Listen failed");
        return 1;
    }

    printf("Server started on port %d...\n", port);

    while (1) {
        // Accept connection from client
        if (b == 1)
        {
            break;
        }
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("Accept failed");
            continue;
        }

        // Handle client in a separate thread
        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, &client_socket) != 0) {
            perror("Thread creation failed");
            close(client_socket);
        }
    }

    // Close server socket
    close(server_socket);

    return 0;
}

void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    char username[50];
    char buffer[BUFFER_SIZE];
    char bffr[BUFFER_SIZE];
    
    // Set receive timeout
    struct timeval timeout;
    timeout.tv_sec = tout;
    timeout.tv_usec = 0;
    if (setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
        perror("Error setting socket receive timeout");
        close(client_socket);
        pthread_exit(NULL);
    }

    // Receive username
    pthread_mutex_lock(&mutex);
    char name_enter[BUFFER_SIZE];
    sprintf(name_enter, "Please enter your name:\n");
    send(client_socket, name_enter, strlen(name_enter), 0);
    if (recv(client_socket, username, sizeof(username), 0) <= 0) {
        perror("Receive username failed");
        close(client_socket);
        pthread_exit(NULL);
    }
    pthread_mutex_unlock(&mutex);

    // Add client to list
    pthread_mutex_lock(&mutex);
    if (num_clients >= MAX_CLIENTS) {
        b = 1;
        printf("Max clients reached. Connection closed.\n");  
        memset(buffer, '\0', sizeof(buffer));
        sprintf(buffer, "Max clients reached. Connection closed. Any further chat won't be communicated\n");
        send_to_all(buffer, -1);
        send(client_socket, buffer, strlen(buffer), 0);
 	pthread_mutex_unlock(&mutex);
        for (int i = 0; i < num_clients; i++)
        {
        	close(clients[i].socket);
        }
        close(client_socket);
        pthread_exit(NULL);
    }
    strcpy(clients[num_clients].username, username);
    clients[num_clients].socket = client_socket;
    num_clients++;
    pthread_mutex_unlock(&mutex);

    // Send welcome message and list of users
    pthread_mutex_lock(&mutex);
    char welcome_message[BUFFER_SIZE];
    sprintf(welcome_message, "Welcome, %s!\nUsers currently online:\n", username);
    send(client_socket, welcome_message, strlen(welcome_message), 0);
    for (int i = 0; i < num_clients; i++) {
        if (strcmp(clients[i].username, username) != 0) {
            sprintf(buffer, "- %s\n", clients[i].username);
            send(client_socket, buffer, strlen(buffer), 0);
        }
    }
    sprintf(welcome_message, "Client %s joined the chatroom\n", username);
    send_to_all(welcome_message, client_socket);
    pthread_mutex_unlock(&mutex);
    
    while (1) {
        memset(buffer, '\0', sizeof(buffer));
        bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            // Error or timeout occurred
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                // Timeout occurred
                pthread_mutex_lock(&mutex);
                for (int i = 0; i < num_clients; i++) {
                    if (clients[i].socket == client_socket) {
                        sprintf(buffer, "Client %s left the chatroom (Timeout)\n", clients[i].username);
                        send_to_all(buffer, client_socket);
                        remove_client(i);
                        break;
                    }
                }
                pthread_mutex_unlock(&mutex);
            } else {
                // Other error occurred
                perror("Receive failed");
            }
            break;
        }
        
        pthread_mutex_lock(&mutex);
        // Handle client commands
        if (strcmp(buffer, "\\list") == 0) {
            sprintf(buffer, "Users currently online:\n");
            for (int i = 0; i < num_clients; i++) {
                sprintf(buffer + strlen(buffer), "- %s\n", clients[i].username);
            }
            send(client_socket, buffer, strlen(buffer), 0);
        } else if (strcmp(buffer, "\\bye") == 0) {
            for (int i = 0; i < num_clients; i++) {
                if (clients[i].socket == client_socket) {
                    sprintf(buffer, "Client %s left the chatroom\n", clients[i].username);
                    send_to_all(buffer, client_socket);
                    remove_client(i);
                    break;
                }
            }
            pthread_mutex_unlock(&mutex);
            break;
        } else {
            // Send message to all clients
            memset(bffr, '\0', sizeof(bffr));
            sprintf(bffr, "%s: %s", username, buffer);
            send_to_all(bffr, client_socket);
        }
        pthread_mutex_unlock(&mutex);
    }

    // Close client socket
    close(client_socket);
    pthread_exit(NULL);
}

void send_to_all(char *message, int sender_socket) {
    for (int i = 0; i < num_clients; i++) {
        if (clients[i].socket != sender_socket) {
            send(clients[i].socket, message, strlen(message), 0);
        }
    }
}

void remove_client(int index) {
    for (int i = index; i < num_clients - 1; i++) {
        clients[i] = clients[i + 1];
    }
    num_clients--;
}
