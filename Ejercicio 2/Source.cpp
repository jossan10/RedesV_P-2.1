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

	memset(&hints_, 0, sizeof(struct addrinfo));

	hints_.ai_family = AF_INET;
	hints_.ai_socktype = SOCK_DGRAM;

	int rc = getaddrinfo(argv[1], argv[2], &hints_, &res_);

	if (rc != 0)
	{
		std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
		return -1;
	}

	// res contenedor sockaddr de dir + puerto

	int sd = socket(res_->ai_family, res_->ai_socktype, 0);

	bind(sd, res_->ai_addr, res_->ai_addrlen);
	
	freeaddrinfo(res_);

	bool running = true;


	while (running)
	{
		char buffer[80];
		char host[NI_MAXHOST];
		char service[NI_MAXSERV];

		struct sockaddr client_addr;
		socklen_t client_len = sizeof(struct sockaddr);

		ssize_t bytes = recvfrom(sd, buffer, 79 * sizeof(char), 0, &client_addr,
		&client_len);

		getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service,
		NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

		time_t t = time(0);

		struct tm* tiempo;

		char bufferT [9];

		if(buffer[0]=='t')
		{
			tiempo=localtime(&t);
			strftime(bufferT,sizeof(bufferT),"%T",tiempo);
		//Respuesta
			sendto(sd, buffer, bytes, 0, &client_addr, client_len);
		}
		else if (buffer[0]=='d')
		{
			tiempo=localtime(&t);
			strftime(bufferT,sizeof(bufferT),"%D",tiempo);
		//Respuesta
			sendto(sd, buffer, bytes, 0, &client_addr, client_len);
		}
		else if(buffer[0]=='q')
		{
			running=false;	
		}

		std::cout << "IP: " << host << " PUERTO: " << service
		<< "MENSAJE: " << buffer << std::endl;
	}
	

	return 0;
}