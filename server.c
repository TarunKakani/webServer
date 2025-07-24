// A simple Stream Server

// Everything that innvolves networks works on client-server architecture
// Take telnet for example, when you connect to a remote host on port 23 with telnet(the client)
// with the programn on that host called telnetd(the server) springs to life
// it handles the incoming telnet connection, sets you up with a login prompt etc.

// CLIENT(send()) --request--> SERVER(recv())
// CLIENT(recv()) <--response-- SERVER(send())

// The client/server pair can speak SOCK_STREAM or SOCK_DGRAM or anything else as they are speaking the same thing
// Some Good examples of client/server pairs are:
// - telnet/telnetd
// - ftp/ftpd
// - firefox/apache
// So everytime you use ftp there's a remote program ftpd that serves you.

// often there is only one server on a machine - this server handles multiple clients using fork()
// the basic process is the server will wait for a connection, accept() it and fork() a child process to handle it

// this is what our simple server will do below

//connect it with: telnet remotehostname 3490 - where remotehostname is the name of the machine you are running it on




/* - - - - - - */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h> // for errors
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3490" // port users will be connecting to
#define BACKLOG 10 // how many pending connections queue will hold

// Clears the zombie processes
void sigchld_handler(int s){

    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

// get sockaddr, Ipv4 or Ipv6
void *get_in_addr(struct sockaddr *sa){
    if (sa->sa_family == AF_INET){
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void){
    int sockfd, new_fd; // listen for sockfd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP - ??? 

    // getaddrinfo
    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can --> we can ???
    // is this error handling?? 
    for (p = servinfo; p != NULL; p = p->ai_next){

        // what is this doing + what is perror?
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
            perror("Server: socket");
            continue;
        }
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
            perror("setsockopt");
            exit(1);
        }
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1){
            close(sockfd);
            perror("Server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure - free this structure as it has no use anymore ??

    if (p == NULL){
        fprintf(stderr, "Server: failed to bind\n");
        exit(1);
    }

    // probably used when backlog is more than the limit and it returns -1 on error
    if (listen(sockfd, BACKLOG) == -1){
        perror("listen");
        exit(1);
    }
    
    sa.sa_handler = sigchld_handler; // reap all the dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGCHLD, &sa, NULL) == -1){
        perror("Sigaction");
        exit(1);
    }

    printf("Server: Waiting for connections....\n");

    while(1){  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);

        if (new_fd == -1){
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        printf("Server: got connection from %s\n", s);

        if (!fork()){ // this is the child process
            close(sockfd); // child doesn't need the listener

            if (send(new_fd, "Hello, Client Side", 20, 0) == -1){
                perror("send");
            }
            close(new_fd);
            exit(0);
        }
        close(new_fd);
    }
    return 0;
}

// Feel to break the whole chunk of code in the main() function into functions that give more clean and reusable code
// this whole sigaction() thing might be new to you—that’s OK. The code that’s there is responsible
// for reaping zombie processes that appear as the and don’t reap them, your system administrator will become agitated.)
// You can get the data from this server by using the client listed in the next section.
// fork()ed child processes exit. If you make lots of zombies and dont reap them, your sys admin will become agitated!!
