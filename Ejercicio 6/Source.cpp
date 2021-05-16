//////////////////////////////////////////////////////////////////////////////////////////
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <vector>

static bool finish = false;
pthread_mutex_t mtxt = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condt = PTHREAD_COND_INITIALIZER;
class MessageThread
{
private:
	ssize_t bytes;
	int sd;
	char buffer[80];

public:
	MessageThread(int sd_):sd(sd_){
		
	}
	void haz_mensaje(){
		while (true){
		
			char host[NI_MAXHOST];
			char service[NI_MAXSERV];

			struct sockaddr client_addr;
			socklen_t client_len = sizeof(struct sockaddr);

			bytes = recvfrom(sd, buffer, 79 * sizeof(char), 0, &client_addr,
				&client_len);
			buffer[bytes] = '\0';

			if (bytes == -1)
			{
				std::cerr << "recvfrom: " << std::endl;
				return;
			}

			getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service,
				NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

			std::cout << "THREAD: " << pthread_self() << " IP: " << host << " PUERTO: " << service
				<< "MSG: " << buffer << std::endl;

			sendto(sd, buffer, bytes, 0, &client_addr, client_len);

			if (buffer[0] == 'q')
			{
				pthread_mutex_lock(&mtxt);
				finish = true;
				pthread_cond_signal(&condt);
				pthread_mutex_unlock(&mtxt);

				break;
			}

		}
	}
};
extern "C" void* _haz_mensaje(void* mt_)
{
	MessageThread* mt = static_cast<MessageThread*>(mt_);
	mt->haz_mensaje();
	delete mt;
	return 0;
}

int main(int argc, char **argv)
{
	struct addrinfo hints;
	struct addrinfo * res;
	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

	if (rc != 0)
	{
		std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
		return -1;
	}

	int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	bind(sd, res->ai_addr, res->ai_addrlen);
	freeaddrinfo(res);
	
	for (int i = 0; i < 5; ++i) {
		MessageThread* mt = new MessageThread(sd);
		pthread_attr_t attrt;
		pthread_attr_init(&attrt);
		pthread_attr_setdetachstate(&attrt, PTHREAD_CREATE_DETACHED); 
		pthread_t thread_id;                                          
		pthread_create(&thread_id, &attrt, _haz_mensaje, mt);
	}

	pthread_mutex_lock(&mtxt);
	while (!finish) { pthread_cond_wait(&condt, &mtxt); }
	pthread_mutex_unlock(&mtxt);


	return 0;
}
