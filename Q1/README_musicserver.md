MUSIC STREAMING SERVER:-

	This program is a simple music streaming server implemented in C. It allows multiple clients to connect simultaneously and stream audio files from a specified directory. The server handles client connections, disconnections, and audio streaming requests.
	
Getting Started:-

	To compile the server, use a C compiler such as GCC:
	gcc -o server server.c -pthread
	
	To run the server, execute the compiled binary:
	./server <port_number> <root_directory> <max_clients>
	
	<port_number>: Port number for the server to listen on.
	<root_directory>: Path to the directory containing audio files.
	<max_clients>: Maximum number of concurrent client connections allowed.
	Once the server is running, clients can connect to it using appropriate client software.


		
Features:-

	Streaming Audio: Allows clients to connect and stream audio files from the specified directory.
	Concurrency: Handles multiple client connections concurrently using pthreads.
	Dynamic Directory Listing: Dynamically retrieves the list of audio files from the specified directory.
	Error Handling: Provides error handling for socket creation, binding, and client connection.
	
Usage:-

	Run the server and enter the required information.
	Clients can connect to the server using a TCP client.
	The server streams audio files from the specified directory to the connected clients.
	
Implementation Details:-

	Socket Programming: Utilizes socket programming to establish connections between the server and clients.
	Multithreading: Employs pthreads to handle multiple client connections concurrently.
	File I/O: Reads audio files from the specified directory and streams them to clients.
	Control Commands: Parses control commands received from clients to control audio playback.
	Dynamic Memory Allocation: Dynamically allocates memory for storing client information and directory entries.
	
Dependencies:-

	gcc: The GNU Compiler Collection for compiling C code.
	pthread: POSIX Threads library for multithreading support.
Compatibility:-

	This server has been tested on Unix-like systems and may require modifications for compatibility with other platforms.
