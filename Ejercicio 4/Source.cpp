#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h>



int main(int argc, char **argv)
{
struct addrinfo hints;
struct addrinfo * res;

memset(&hints, 0, sizeof(struct addrinfo));

hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_STREAM;

int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

if ( rc != 0 )
{
std::cerr << "getaddrInfo: " << gai_strerror(rc) << std::endl;
return -1;
}
int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
bind(sd, res->ai_addr, res->ai_addrlen);
freeaddrinfo(res);
listen(sd, 16);


while (true) {

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    char buffer[80];
    struct sockaddr client_addr;
    socklen_t client_len = sizeof(struct sockaddr);
    int sd_client = accept(sd, &client_addr, &client_len);
    getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service,
    NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

    std::cout << "CONEX. IP: " << host << " PUERTO: " << service << std::endl;

    while (true) {

       ssize_t bytes = recv(sd_client, (void *) buffer, sizeof(char)*79, 0);
       if(buffer[0] == 'q' && bytes <= 1 || bytes <= 0)break;
       buffer[bytes]='\0';
       sendto(sd_client, buffer,bytes, 0,&client_addr,client_len);
    }
    std::cout << "CONEX. FIN " << std::endl;
}
return 0;
}