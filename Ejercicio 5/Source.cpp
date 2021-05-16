#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <iostream>


int main(int argc, char **argv)
{

char buffer [80];	
struct addrinfo hints;
struct addrinfo * res;

memset(&hints, 0, sizeof(struct addrinfo));

hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_STREAM;

int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

if ( rc != 0 )
{
std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
return -1;
}

struct sockaddr client_addr;
socklen_t client_len = sizeof(struct sockaddr);

int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

int server = connect(sd,res->ai_addr,res->ai_addrlen);

sendto(sd, argv[3],sizeof(argv[3]), 0,res->ai_addr,res->ai_addrlen);

ssize_t bytes = recvfrom(sd,(void*)buffer, 79 * sizeof(char), 0, res->ai_addr,
		&res->ai_addrlen);

buffer[bytes]='\0';

std::cout<<buffer<<std::endl;

freeaddrinfo(res);

return 0;
}