#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <string.h>

int main(int argc, char** argv){

    struct addrinfo hints;
    struct addrinfo *res;
    
    memset((void*)&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_UNSPEC;
   

    int rc_ = getaddrinfo(argv[1], argv[2], &hints, &res);

    if(rc_ != 0){
        std::cout << "Error GtaddrInfo: " << gai_strerror(rc_) << std::endl;
        return -1;
    }

    for(struct addrinfo * resaux=res; resaux!=0;resaux = resaux->ai_next){
        char host[NI_MAXHOST];
        char serv[NI_MAXSERV];

        getnameinfo(resaux->ai_addr, resaux->ai_addrlen, host, NI_MAXHOST, serv, NI_MAXSERV, 
        NI_NUMERICHOST | NI_NUMERICSERV);

        std::cout << "HOST: " << host << "\tPUERTO: " << serv <<"\n";
    }
    freeaddrinfo(res);

    return 0;
}