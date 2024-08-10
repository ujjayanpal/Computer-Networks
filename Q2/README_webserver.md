HTTP SERVER:-

	This is a C program for a simple HTTP server that serves static files and handles POST requests. The server listens for incoming client connections, processes HTTP requests, and responds accordingly.

Prerequisites:-

	This program requires a C compiler to build and run.
Ensure that the necessary libraries are available on your system: stdio.h, locale.h, stdlib.h, unistd.h, string.h, arpa/inet.h, sys/socket.h, netinet/in.h, pthread.h, dirent.h, strings.h, sys/time.h, sys/types.h.

Usage:-

	Compile the program using the following command:
	gcc http_server.c -o http_server -lpthread
	Run the compiled executable:
	./http_server <root_directory> <port_number>
	<root_directory>: Path to the directory containing files to be served.
	<port_number>: Port number for the server to listen on.
Features:-

	Serves static files (HTML, CSS, JavaScript, images, audio, video, PDF, etc.) from the specified directory.
	Handles GET requests for file retrieval and POST requests for processing form data.
	Supports multiple concurrent client connections using pthreads.
	Provides basic error handling for file not found and invalid requests.
	Calculates and sends character, word, and sentence counts for POST request data.
	
File Structure:-

	http_server.c: Main program source code.
	404.html: Custom HTML page for 404 Not Found responses.
Example:-

	./http_server /path/to/files 8080
	
Notes:-
	Ensure that the specified root directory contains the necessary files to be served.
	Adjust the port number according to your requirements and firewall settings.
	Handle concurrent requests carefully to avoid race conditions and deadlocks.
