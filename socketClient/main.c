#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

   int socketFD = socket(AF_INET, SOCK_STREAM, 0);

   
// socket address struc imported in <netinet/in.h> for IPV4 address and propperties of the address
   struct sockaddr_in address;
   address.sin_family = AF_INET;

//    port number is a short, an unisgned short integer, and when we write bytes in paper we go grande indiano, but I am using
//    a mac so it's a little indian, that's why I used htons to order the bytes in the right order.
   address.sin_port = htons(80);
   
//    this was an ass to find out but sin_add is an unsigned 4 byte (32 bits) integer and IP addresses
//    have 4 numbers inside it, for example (192.0.11.1) -> there are 4 numbers delimited by the dots between them
//    here I would have to extract these numbers, make them an unsigned byte and put them inside an unsigned 
//    integer and ONLY THEN assing it to this propperty. Because that's bullshit, I got this function:
    char* ip = "142.250.188.46";
    inet_pton(AF_INET, ip, &address.sin_addr.s_addr);


// connect function receives a File descriptor, a pointer to the socket address and the size of the address
    int result = connect(socketFD, &address , sizeof address);

    if(result == 0) 
        printf("connection succesfull\n");

   return 0;
}