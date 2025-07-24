#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
// errno ? and -1 error ?

// int getaddrinfo(const char *node, // eg. "www.website.com" or IP
	// 	const char *service, // eg "http" or port number
	// 	const struct addrinfo *hints,
	// 	struct addrinfo **res); // why is double pointer used?? res for results, a linked list

// int s;
// struct addrinfo hints, *res;

// do the lookup
// getaddrinfo("www.example.com", "http", &hints, &res);

// call the socket system call and returns a socket descriptor that can be later used in system call
// s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);


// bind() - port number system call

// sockfd is the socket file descriptor returned by socket()
// *my_addr is a pointer that points to a struct sockaddr that contains info about your address
// namely port and ip address
// addrlen is the length in bytes of that address

// int bind(int sockfd, struct sockaddr *my_addr, int addrlen);

// example that binds the socket to the host the program is running on, port 3490

// struct addrinfo hints, *res;
// int sockfd;


// first, load up address structs with getaddrinfo():
// memset(&hints, 0, sizeof hints);

// hints.ai_family = AF_UNSPEC; // can use ipv4 or ipv6 whichever
// hints.ai_socktype = SOCK_STREAM;
// hints.ai_flags = AI_PASSIVE; // fill in my IP for me


// getaddrinfo(NULL, "3490", &hints, &res);


// make a socket:
//sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);


// bind it to the port we passed in to getaddrinfo();
// bind(sockfd, res->ai_addr, res->ai_addrlen);


// connect() - remote connection to ex. "10.12.110.57" om port 23
// int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);

// Example where we make a socket connection to "www.example.com" on port 3490
// struct addrinfo hints, *res;
// int sockfd;

// first load up address structs with getaddrinfo():
// memset(&hints, 0, sizeof hints);
// hints.ai_family = AF_UNSPEC;
// hints.ai_socktype = SOCK_STREAM;

// getaddrinfo("www.example.com", "3490", &hints, &res);

// make a socket
// sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

// connect!
// connect(sockfd, res->ai_addr, res->ai_addrlen);
// will return -1 on error and set on variable errno




// listen() - if you dont want to connect to a remote host, 
// just wait for incoming connections and handle them in some way
// the process contains two steps listen() and then accept() the client machine

// int listen(int sockfd ,int backlog);

// socketfd is the usual file descriptor.
// and backlog is the number of connections allowed on the incoming queue.
// backlog is the number of systems waiting in the queue until you accept() them.
// this is the limit of how many systems can queue up. mostly systems silently limit his number to 20
//  we can set it to b/w 5 or 10
// and as per usual it returns 0 on success and -1 on failure and sets errno on error.



// so the step by step system call process to listen for incoming connections is - 
// getaddrinfo()
// socket()
// bind() - to tell your buddies which port to connect to!
// listen()
// accept()



// accept() - accept incoming connections

// int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen); 

// sockfd is the listen() file descriptor.
// addr will usually be the pointer to a local struct sockaddr_storage - // this is where the information about the incoming connection will go
// 																			// when you accept() the connection with it you can determine which host is calling you from which port
// addrlen is a local integer varible that should be set to the sizeof(struct sockaddr_storage) before its address is passed to accept()
// new fact : it returns -1 on error and sets errno if error occurs. bet ya didn't know that

// example code for accept()
// #define MYPORT "3490"
// #define BACKLOG 10

// int main(void){

//  	struct sockaddr_storage their_addr;
//  	socklen_t addr_size;
//  	struct addrinfo hints, *res;
//   	int sockfd, new_fd; // new_fd is used for send() and recv() calls.

	// don't forget the error checking for these calls !! very imp
	// first load up the address structs with getaddrinfo();

//  	memset(&hints, 0, sizeof(hints));
//    hints.ai_family = AF_UNSPEC; // use ipv4 or ipv6
//  	hints.ai_socktype = SOCK_STREAM;
//  	hints.ai_flags = AI_PASSIVE; // fill in the IP address

//  	getaddrinfo(NULL, MYPORT, &hints, &res);

	// make a socket, bind it, and listen on it.
	// sockfd = socket(res->ai_family, res->ai_socktype, res->ai_flags);
	// bind(sockfd, res->ai_addr, res->ai_addrlen);
	// listen(sockfd, BACKLOG);

	// now accept an incoming connection - understand deepply 
	// addr_size = sizeof their_addr; // their_addr ??
	// new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size); // (struct sockaddr *)&their_addr ?? so sexy syntax

	// ready to communicate on the new_fd socket descriptor
	/* your code here
	...
	*/



	// send() and recv() - talk to me, baby!
	// these are used for communicating b/w two stream sockets or datagram sockets
	// blocking calls - recv() will block until there is some data ready to receive
	// block basically means the programn will stop there until someone sends you something.
	// send() can also block if the stuff you are sending is all jammed up due to some reason - but rare

	int send(int sockfd, const void *msg, int len, int flags);

	// sockfd - as usual is the file descriptor you want to send data to.
	// it could be the one returned by socket() or the one you got with accept()
	// msg - is a pointer to the data you want to send
	// len - is the length of that data in bytes - so if we dont know this we have to allocate memory dynamically ?
	// just set flags to zero 
  
	// sample code 
  char *msg = "Tarun was here!";
  int len, bytes_sent;

  len = strlen(msg);
  bytes_sent = send(sockfd, msg, len, 0);
  
  // if the data is more than the len, then send() will try to send as much data as it can and trust us to send the remaining data later
  
  // recv()
  int recv(int sockfd, void *buf, int len, int flags);
  // same, the sockfd is the file descriptor to read from
  // buf is the buffer to read the information into 
  // len is the maximum length of the buffer 
  // same, flags can be set to 0

  // recv() returns the number of bytes read into the buffer or -1 on error and sets errno
  // recv() can return 0. this means only one thing that remote side has closed the connection on you!

  // YOU CAN NOW PASS DATA BACK AND FORTH WITH STREAM SOCKETS!!!

  // Datagram sockets sendto() and recvfrom() is skipped for now will do it at last - but dont forget this part its imp for multiplayer games




	// close() and shutdown() - shut the fuc down
	int close(int sockfd);
	// close() - will prevent anymore read and writes to the socket, anyone who attempts will get an error

	
	int shutdown(int sockfd, int how);
	// you can use shutdown() if you want more control - it allows you to cut off communication in a certain direction or both ways just like close()
	// sockfd is the file descriptor you want to shutdown and int how can be one of the following:
	// 0 - further recieves are disallowed
	// 1 - further sends are disallowed
	// 2 - further sends and recieves are disallowed(like close())
	// same, it returns 0 on success and -1 on error and sets errno accordingly

	// its important to know that shutdown() doesn't actually close the file descriptor - it just changes the usability.
	// to free the socket descriptor you need to use close() nessecarily.

	// ALSO IMP NOTE: IF YOU ARE USING WINDOWS AND WINSOCK you should call closesocket() instead of close().




	// getpeername() - who are you?
	// this function tells you who is at the other end of the connected stream socket.

	int getpeername(int sockfd, struct sockaddr *addr, int *addrlen);

	// sockfd is the file descriptor of the connected stream socket
	// addr is a pointer to a struct sockaddr or a struct sockaddr_in that will hold the information about the other side of the connection
	// addrlen - is a pointer to int that should be initialized to sizeof *addr or sizeof(struct sockaddr).

	// the function returns -1 on error and sets errno accordingly

	// once you have their address, you can use inet_ntop(), getnameinfo(), gethostbyaddr() to print or get more information.
	// Usually we can't get the login name, BUT listen to me.. BUT we if the computer is running an ident daemon, this is possible
	// Check out RFC(1413) for more info



	
	// gethostname() - who am i?
	// even easier than the getpeername() is gethostname().
	// it returns the name of the computer that your programn is running on.
	// the name then can be used by getaddrinfo() above to determine the ip address of your local machine.

	int gethostname(char *hostname, size_t size);

	// hostname is a pointer to a array of chars that will contain the hostname upon the function's return
	// size is the length in bytes of the hostname array
	// the function returns 0 on success and -1 on error and sets errno as usaual
	// 
}
