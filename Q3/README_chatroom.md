CHAT ROOM SERVER:-

	This program is a simple chat room server implemented in C. It allows multiple clients to connect simultaneously and communicate with each other in a chat room environment. The server handles client connections, disconnections, and message exchanges.

Getting Started:-

To compile the server, use a C compiler such as GCC:

	gcc -o server server.c
	
To run the server, execute the compiled binary with the following command:
	
	./server <port> <max_clients> <timeout>
	

	<port>: The port number on which the server will listen for incoming connections.
	<max_clients>: The maximum number of clients allowed to connect to the server simultaneously.
	<timeout>: The timeout duration in seconds for receiving messages from clients.
	
Features:-
	
	Multiple Clients: Supports multiple clients connecting to the server concurrently.
	Timeout Handling: Sets a timeout for receiving messages from clients, disconnecting clients that have been inactive for too long.
	Command Handling: Allows clients to use commands such as \list to view the list of online users and \bye to disconnect from the chat room.
	Threaded Architecture: Handles each client connection in a separate thread to ensure concurrent operation.
	
Usage:-

	Upon connecting to the server, clients are prompted to enter their username.
	Once authenticated, clients receive a welcome message and a list of online users.
	Clients can send messages to the entire chat room by typing into the terminal.
	Clients can use the \list command to view the list of online users.
	Clients can use the \bye command to disconnect from the chat room.
	
Implementation Details:-

	Socket Programming: Utilizes socket programming to establish connections between the server and clients.
	Multithreading: Employs pthreads to handle multiple client connections concurrently.
	Timeout Handling: Sets a receive timeout for client sockets to detect inactive clients and disconnect them after a certain period of inactivity.
	Dynamic Memory Allocation: Dynamically allocates memory for storing client information.
	
Dependencies:-

	gcc: The GNU Compiler Collection for compiling C code.
	pthread: POSIX Threads library for multithreading support.

Compatibility:-

	This server has been tested on Unix-like systems and may require modifications for compatibility with other platforms.
