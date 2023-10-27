#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h> 

// same logic as client-side, but instead of connecting to a server, the socket API is going to LISTEN
// to incoming connections.

int createTCPIpv4Socket();

struct sockaddr_in* createIPv4Address(char *ip, int port);


int main() {
    
    // creating serverside socket
    int serverSocketFD = createTCPIpv4Socket();

    struct sockaddr_in *serverAddress = createIPv4Address("", 2000);

    // bind function receives a File descriptor, a pointer to the socket address and the size of the address
    // and it tries to bind and occupy the open-port number 2000 from the OS. so basically, this process wants
    // the number 2000 to listen for incoming connections
    int result = bind(serverSocketFD, serverAddress, sizeof(*serverAddress));
    if(result == 0) 
        printf("connection succesfull\n");
    
    // listens for incoming connections, and it can accept up to 10 connections in queue
    int listenResult = listen(serverSocketFD, 10);

    // function accept receives a File descriptor, a pointer to the socket address and the size of the address
    // and it accepts the incoming connection. The response is the file descriptor of the clientside;
    struct sockaddr_in clientAddress;
    int clientAddressSize = sizeof(struct sockaddr_in);
    int clientSocketFD = accept(serverSocketFD, &clientAddress, &clientAddressSize);

    char buffer[1024];

    while(true) {
       ssize_t amountReceived = recv(clientSocketFD, buffer, 1024, 0);

       if(amountReceived > 0) {
        // this is just to reset the response, to print until the LAST message
           buffer[amountReceived] = 0;
           printf("Response was: %s\n", buffer);
        }

       if(amountReceived == 0){
            break;
       }
    }

  
    close(clientSocketFD);
    shutdown(serverSocketFD, SHUT_RDWR);


    return 0;
}


int createTCPIpv4Socket() { return socket(AF_INET, SOCK_STREAM, 0); }

struct sockaddr_in* createIPv4Address(char *ip, int port) {

   // allocate 16 bytes and return the pointer inside
    struct sockaddr_in  *address = malloc(sizeof(struct sockaddr_in));
   //  arrow operator is used to point to the address, and not use the .
    address->sin_family = AF_INET;
    address->sin_port = htons(port);


    // if no IP provided, set to any
    if(strlen(ip) == 0)
        address->sin_addr.s_addr = INADDR_ANY;
    else
        inet_pton(AF_INET, ip, &address->sin_addr.s_addr);

    return address;
}

