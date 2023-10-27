#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h> 


int createTCPIpv4Socket();


// socket address struc imported in <netinet/in.h> for IPV4 address and propperties of the address

//    port number is a short, an unisgned short integer, and when we write bytes in paper we go grande indiano, but I am using
//    a mac so it's a little indian, that's why I used htons to order the bytes in the right order.
   
//    this was an ass to find out but sin_add is an unsigned 4 byte (32 bits) integer and IP addresses
//    have 4 numbers inside it, for example (192.0.11.1) -> there are 4 numbers delimited by the dots between them
//    here I would have to extract these numbers, make them an unsigned byte and put them inside an unsigned 
//    integer and ONLY THEN assing it to this propperty. Because that's bullshit, I got this function:

struct sockaddr_in* createIPv4Address(char *ip, int port);

struct sockaddr_in* createIPv4Address(char *ip, int port) {

   // allocate 16 bytes and return the pointer inside
    struct sockaddr_in  *address =(struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
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

int main() {
   
// socket function accepts domain, type and protocol
// domain will be AF_INET because I will be using IPV4 instead of IPV6 because there is no need for overkill link:(https://kinsta.com/blog/ipv4-vs-ipv6/)
// type will be SOCK_STREAM, basically can create a TCP socket or UDP socket, initially use TCP socket that's why Socket stream

// protocol will be 0 determining the layer beneath the transport layer I am using,
// TCP and UDP are in the transportation layer and there are other layers under it, that
// the socket layer will call after being done with packaging and specifying ports
// and it will pass it to the layer underneath it, and then it will do all it has to do
// and then put it in the IP package. Network is a layered architecture and every layer will have
// a sort of "box" with it's own information. link: (https://www.educative.io/answers/what-is-layered-architecture-in-networking)

// socket function returns an integer, and if it's a non-negative, it means the socket was created successfully 
// and a socket File Descriptor will be returned. Sockets were implemented in a way that they are a file in our system.

   int socketFD = createTCPIpv4Socket();

   struct sockaddr_in *address = createIPv4Address("127.0.0.1", 2000);


// connect function receives a File descriptor, a pointer to the socket address and the size of the address
    int result = connect(socketFD, address , sizeof (*address));

    if(result == 0) 
        printf("connection succesfull\n");


    
    char *line = NULL;
    size_t lineSize = 0;
    printf("type a message: (or type exit to exit) \n");
    

    while(true) {
        // after this function returns, the line buffer will be filled with the message and the
        // charCount will be set to the characters of that message
        ssize_t charCount = getline(&line, &lineSize, stdin);

        if(charCount > 0) {
            if(strcmp(line, "exit\n") == 0)
                break;
            
            ssize_t amountSent = send(socketFD, line, charCount, 0);  
        }

    }

    close(socketFD);

   return 0;
}

int createTCPIpv4Socket() { return socket(AF_INET, SOCK_STREAM, 0); }