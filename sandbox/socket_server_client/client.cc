#include "iostream"
#include "cstdlib"
#include "cstdio"
#include "cstring"
#include "netinet/in.h"
#include "sys/socket.h"
#include "unistd.h" // read close
#include "arpa/inet.h" // inet_pton
#include "Fmt.hpp"

#define PORT 8080

#define msg "this is major tom to ground control"

int	main()
{
	// Stages for clients
	//  1. Create socket
	//  2. Use connect() system call to link sockfd to addr w/ port

	struct sockaddr_in	Server_address;
	int		Client_fd;
	int		valread;
	int		status;
	int		ret;
	char		buffer[1024] = { 0 };

	Client_fd = socket(
		AF_INET, /* domain */
		SOCK_STREAM, /* communication type */
		0 /* opt for IP */
	);
	if ( Client_fd < 0 )
		return (perror("failed to create socket"), -1);

	Server_address.sin_family = AF_INET;
	Server_address.sin_port = htons(PORT);

	ret = inet_pton(
		AF_INET,
		"127.0.0.1",
		& Server_address.sin_addr
	);
	if (ret < 1)
		return (perror("invalid address"), -1);

	status = connect(
		Client_fd, /* sockfd */
		(struct sockaddr *) &Server_address, /* const struct sockaddr * */
		sizeof( Server_address ) /* socklen_t addrlen */
	);
	if (status < 0)
		return (perror("failed to connect"), -1);

	send(Client_fd, msg, strlen(msg), 0);
	std::cout << YELLOW << "Message sent. (client side)" nlreset;

	valread = read( Client_fd, buffer, 1024);
	std::cout << YELLOW << buffer << " (received by client)" << nlreset;
	

}




