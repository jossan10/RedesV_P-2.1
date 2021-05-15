#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h>

int main(int argc, char **argv)
{
	
	struct addrinfo hints_;
	struct addrinfo * res_;

	char buffer[80];
	
	memset(&hints_, 0, sizeof(struct addrinfo));

	hints_.ai_family = AF_INET;
	hints_.ai_socktype = SOCK_DGRAM;

	int rc = getaddrinfo(argv[1], argv[2], &hints_, &res_);

	if (rc != 0)
	{
		std::cerr << "getaddrInfo: " << gai_strerror(rc) << std::endl;
		return -1;
	}


	struct sockaddr client_addr;
	socklen_t client_len = sizeof(struct sockaddr);

	int sd = socket(res_->ai_family, res_->ai_socktype, 0);

	sendto(sd, argv[3], sizeof(argv[3]), 0, res_->ai_addr, res_->ai_addrlen);

	ssize_t bytes = recvfrom(sd,(void*)buffer, 79 * sizeof(char), 0, res_->ai_addr,
		&res_->ai_addrlen);

	buffer[bytes]='\0';
	
	std::cout<<buffer<<std::endl;


	freeaddrinfo(res_);
	

	return 0;
}