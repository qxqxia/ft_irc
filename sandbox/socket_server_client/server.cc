#include "iostream"
#include "netinet/in.h"
#include "sys/socket.h"
#include "unistd.h" // read close
#include "cstring" // strlen
#include "cstdio" // perror
#include "Fmt.hpp"

#define PORT 8080

#define msg "this is server to client"
#define Backlog_Len 3

int	main()
{
	// Stages for server
	//  1. Create socket
	//  2. setsockopt
	//  3. bind
	//  4. listen
	//  5. accept

	struct sockaddr_in	address;
	int		Server_fd;
	int		new_socket;
	int		valread;
	int		ret;
	int		opt = 1;
	int		addrlen = sizeof(address);
	char		buffer[1024] = { 0 };


	// SOCKET Creation
	//  generate a socker fd

	Server_fd = socket(
		AF_INET, /* domain */
		// AF_INET or AF_INET6 - between procs on different hosts (IPV6)
		// AF_LOCAL - between procs on the same host
		SOCK_STREAM, /* communication type */
		// SOCK_STREAM - TCP
		// SOCK_DGRAM - UDP
		0 /* protocol - Internet Protocol uses 0 */
	);

	if ( Server_fd < 0 )
		return (perror("failed to create socket"), -1);



	// ATTACH - optional but prevents error "address in use"
	//  attach socket to port 8080

	ret = setsockopt(
		Server_fd, /* sockfd */
		SOL_SOCKET, /* level */
		SO_REUSEADDR | SO_REUSEPORT, /* optname, Here: reuse ADDR & port */
		& opt, /* const void *optVal */
		sizeof(opt) /* socklen_t option_len */
	);

	if (ret)
		return (perror("setsockopt error"), -1);



	// BIND
	//   binds the socket to the address and port number specified in a struct sockaddr
	//
	//   Here:
	//     - server is bound to Localhost
	//     - INADDR_ANY is used to specify the IP address

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT /* 8080 */);

	// `hton``ntoh`
	//   convert values between host (h) byte order and network byte order (n)
	//    
	//    htonl : uint hostlong to NBO
	//    htons : uint16 hostshort to NBO
	//    
	//    ntohl : uint hostlong to HBO
	//    ntohs : uint16 hostshort to HBO

	ret = bind(
		Server_fd, /* sockfd */
		(struct sockaddr *) & address, /* const struct sockaddr *addr */
		sizeof(address) /* socklen_t Address_len */
	);

	if (ret < 0)
		return (perror("bind error"), -1);



	// LISTEN
	//  listen(sockfd, backlog_len) puts Socket in passive/listening mode 
	//  	ie. waiting for client to approach
	//
	//  backlog_len : the max length of the pending connections queue
	//  	ECONNREFUSED beyond backlog_len

	ret = listen(
		Server_fd, 
		Backlog_Len /* backlog_len */
	);

	if (ret < 0)
		return (perror("listen failed"), -1);



	// ACCEPT
	//  extracts the first request on the queue of pending
	//  creates a new socket connection
	//  returns a fd to that socket

	new_socket = accept(
		Server_fd,
		(struct sockaddr *) & address,
		(socklen_t *) & addrlen
	);
	if (new_socket < 0)
		return (perror("accept failed"), -1);



	// READ

	valread = read(
		new_socket,
		buffer,
		1024
	);
	std::cout << CYAN << buffer << " (received by server)" << nlreset;



	// SEND
	send(new_socket, msg, strlen(msg), 0);
	std::cout << CYAN << "Message sent. (server side)" nlreset;


	// CLOSE Socket (client)
	close(new_socket);
	shutdown(Server_fd, SHUT_RDWR);
}


