#include "iostream"
#include "cstdio"
#include "cstring" // strlen
#include "cstdlib"
#include "errno.h"
#include "unistd.h" //close 

#include "arpa/inet.h" //close 
#include "sys/types.h"
#include "sys/socket.h" 
#include "netinet/in.h" 
#include "sys/time.h" // FD_SET fn series

#define Backlog_Len 3
#define msg "ECHO Daemon v1.0 \r\n"

#define	PORT 6669


// footnote ...... XXX
//  
//  FD_ISSET() 
//   is a macro fn that says whether a given socket descriptor
//   (4, for example) is set in the bit array, ie. fd_set
//  
//  FD_SET()
//   allows you to set a bit yourself
//
//  FD_CLR()
//   lets you clear a bit.
//


int	main()
{
	// Stages
	//
	//  1. Create socket
	//  2. setsockopt
	//  3. bind
	//  4. listen
	//  5. accept

	struct sockaddr_in	address;
	fd_set	readfds; //set of socket descriptors

	int	client_socket[30];
	int	max_clients = 30;

	// char	buffer[1025]; // data buffer of 1K
	char	buffer[1024] = { 0 }; // both styles

	int	addrlen = sizeof(address);
	int	opt = true;

	int	Server_fd;
	int	new_socket;
	int	activity;
	int	valread;
	int	max_sd;
	int	sd;
	int	i;

	int	ret;
	long	lret;


	// initialise all client_socket[] to 0  

	i = -1;
	while (++i < max_clients)
	{
		client_socket[i] = 0;
	}


	// SOCKET Creation

	Server_fd = socket(
		AF_INET,
		SOCK_STREAM,
		0
	);
	if (Server_fd < 0)
	{
		return (perror("socket failed"), 1);
		// exit(EXIT_FAILURE);
	}


	// set master socket to allow multiple connections
	// * this is just a good pratice, it will work without this

	ret = setsockopt(
		Server_fd,
		SOL_SOCKET, /* level */
	SO_REUSEADDR | SO_REUSEPORT,
		(char *) & opt, /* const void *optVal */
		sizeof(opt)
	);
	if (ret < 0)
	{
		return (perror("setsockopt failed"), 1);
		// exit(EXIT_FAILURE);
	}


	// setting type of socket created 

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY specifies the IP address
	address.sin_port = htons( PORT );


	// bind the socket to localhost port
	//  def. :
	//   bind() binds socket to the 
	//   address & port specified in a struct sockaddr

	ret = bind(
		Server_fd,
		(struct sockaddr *) & address,
		sizeof(address)
	);
	if (ret < 0)
	{
		return (perror("bind failed"), 1);
		// exit(EXIT_FAILURE);
	}

	std::cout << "Listener on port " << PORT << std::endl;


	// specify max length of pending queue
	// set to MAX_Backlog_Len pending connections max for master socket

	// 
	// ** original notes **
	// 
	// LISTEN
	//  listen(sockfd, backlog_len) puts Socket in passive/listening mode
	//  	ie. waiting for client to approach
	//
	//  backlog_len : the max length of the pending connections queue
	//  	ECONNREFUSED beyond backlog_len
	//

	ret = listen(
		Server_fd,
		Backlog_Len /* 3 */
	);
	if (ret < 0)
	{
		return (perror("listen"), 1);
		// exit(EXIT_FAILURE);
	}


	// accept incoming connection(s)

	// 
	// ** original notes **
	//
	// ACCEPT
	//  extracts the first request on the queue of pending
	//  creates a new socket connection
	//  returns a fd to that socket
	//

	addrlen = sizeof(address);
	std::cout << "Waiting for connections ..." << std::endl;


	// enter the "listener while loop"

	while (1)
	{
		// clear the socket set 

		FD_ZERO(&readfds);


		// add master socket to set 

		FD_SET(Server_fd, &readfds);
		max_sd = Server_fd;


		// add child sockets to set 

		i = -1;
		while (++i < max_clients)
		{
			// socket descriptor 
			sd = client_socket[i];

			// validate socket descriptor and add it to red list 
			if (sd > 0)
				FD_SET( sd , &readfds);

			// update the highest file descriptor 
			// which we need for select()
			if (sd > max_sd)
				max_sd = sd;
		}


		// Select()
		// wait for an activity on one of the sockets
		// set timeout to null to wait indefinitely 

		activity = select(
			max_sd + 1,
			& readfds,
			NULL,
			NULL,NULL
		);
		if ( (activity < 0) && (errno != EINTR) )
		{
			std::cout << "select error \n" << std::endl;
		}


		// we monitor master socket
		//  if something happened on the master socket
		//  it could be either one of 2 possibilities

		// (1/2) :: incoming connection
		//  in this case we check fd with fd_is_set

		ret = FD_ISSET(
			Server_fd,
			& readfds
		);
		if (ret)
		{
			
			new_socket = accept(
				Server_fd,
				(struct sockaddr *) & address,
				(socklen_t *) & addrlen
			);
			if ( new_socket < 0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}


			// inform user of socket number
			// forsending / receiving commands 

			std::cout << "New connection , socket fd is "
			<< new_socket << " , ip : "
			<< inet_ntoa(address.sin_addr) << " , port : "
			<< ntohs(address.sin_port) << std:: endl;


			// send new connection greeting msg

			lret = send(
				new_socket,
				msg,
				strlen(msg),
				0
			);
			if( lret != (long) strlen(msg) )
			{
				return (perror("send failed"), 1);
			}
			std::cout << "Welcome message sent successfully \n";


			// add new socket to array of sockets
			
			i = -1;
			while (++i < max_clients)
			{
				// use first empty slot found
				if ( client_socket[i] == 0 )
				{
					client_socket[i] = new_socket;
					std::cout << "Added to list of sockets as "
					<< i;
					break ;
				}
			}
		}


		// (2/2) :: if not incoming connection
		//  then it is I/O operation on some other socket (?)

		i = -1;
		while (++i < max_clients)
		{
			sd = client_socket[i];
			ret = FD_ISSET( sd, & readfds );
			if (ret)
			{
				// check if it was for closing
				// read incoming msg

				valread = read( sd , buffer, 1024 );


				// 2 possibilities
				//  (1/2) :: either someone is disconnected
				//  (2/2) :: or we got a msg we can just echo back

				if (!valread) // someone is disconnected
				{
					// get details and print info
					getpeername(
						sd,
						(struct sockaddr *) & address,
						(socklen_t * ) & addrlen
					);
					std::cout << "Host disconnected , ip "
					<< inet_ntoa(address.sin_addr) << ", port "
					<< ntohs(address.sin_port);

					// close socket
					close( sd );

					// mark as 0 in list for reuse
					client_socket[ i ] = 0;
				}
				else // a message, we echo back
				{
					// term the buffer to null byte
					buffer[valread] = '\0';
					send( sd, buffer, strlen(buffer), 0 );
				}
			}
		}
	}
}
