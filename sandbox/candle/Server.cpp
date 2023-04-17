#include "Freenode.hpp"

int		g_Socket_Queue[ MAX_CLIENTS ];
bool	g_Server_Alive = true;


Server::Server(const std::string &port, const std::string &password) : m_port(port), m_pass(password), m_server_name(), m_is_restarting(false){
	this->m_commands["NICK"] = & nick;
	this->m_commands["JOIN"] = & join;
	this->m_commands["PRIVMSG"] = & privmsg;
	this->m_commands["NOTICE"] = & privmsg; // ??? why
	// this->m_commands["PING"] = & ping;
	// this->m_commands["PONG"] = & pong;
	this->m_commands["PART"] = & part;
	this->m_commands["TOPIC"] = & topic;
	this->m_commands["KICK"] = & kick;
	this->m_commands["MODE"] = & mode;
	this->m_commands["OPER"] = & oper;
	this->m_commands["QUIT"] = & quit;

	this->m_commands["kill"] = & kill;
	this->m_commands["KILL"] = & kill;

	this->m_commands["RESTART"] = & restart;
	this->m_commands["restart"] = & restart;

}


Server::~Server()
{
	this->m_commands.clear();
	
	// /*IMPORTANT*/
	// delete this->_bot;
}


void	Server::XCQ(int socket_fd, std::string buf)
{
	std::string		cmd;
	size_t			ptr;

	if (buf.empty())
	{
		return ;
	}
	cmd = buf;
	std::cout << CYAN "COMMAND RECEIVED :" RESET << buf;	
	ptr = buf.find_first_not_of(SEP_CHARSET, 0);
	buf = cmd.substr(ptr, buf.length() - ptr);
	std::cout << YELLOW "COMMAND PARSED 1 :" RESET << buf; // still contains ending lbreak
	cmd = buf.substr(0, buf.find_first_of(SEP_CHARSET, 0));
	std::cout << GREEN "COMMAND PARSED 2 :" RESET << cmd << nl2reset; // added 1 more lbreak for better view

	if (m_commands.find(cmd) == m_commands.end())
	{
		return ;
	}
	(m_commands[cmd])(this, buf, socket_fd) ;;
}



int Server::new_socket()
{
	// Stages for server
	//  1. Create socket
	//  2. setsockopt
	//  3. bind
	//  4. listen
	//  5. accept

	int	Server_fd;
	int	opt;
	// int	ret;

	Server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (Server_fd < 0)
	{
		throw std::runtime_error("Error creating server socket.\n");
	}
	opt = 1;
	// ret = setsockopt(Server_fd, SOL_SOCKET, SO_REUSEADDR, & opt, sizeof(opt));
	// if (ret)
	if (setsockopt(Server_fd, SOL_SOCKET, SO_REUSEADDR, & opt, sizeof(opt)))
	{
		throw std::runtime_error("Error while setting up socket.\n");
		// return (perror("setsockopt error"), -1);
	}
	if (fcntl(Server_fd, F_SETFL, O_NONBLOCK) == -1)
	{
		throw std::runtime_error("Error while setting socket NON-BLOCKING mode.\n");
	}

	// BIND (1/2)

	m_server.sin_addr.s_addr = INADDR_ANY;
	m_server.sin_port = htons(
			static_cast<uint16_t>(strtoul(this->m_port.c_str(), NULL, 0)));
			// static_cast<unsigned short>(strtoul(this->m_port.c_str(), NULL, 0)));
	m_server.sin_family = AF_INET;


	// BIND (2/2)

	// ret = bind(Server_fd, (const struct sockaddr *) & m_server, sizeof(m_server));
	// if (ret < 0)
	if ( bind(Server_fd, (const struct sockaddr *) & m_server, sizeof(m_server)) < 0 )
		throw std::runtime_error("Error binding socket.\n");
		// return (perror("bind error"), -1);


	// listen() --- Prepare the server for incoming client requests
	// 	* Requests from clients keep queueing up as we listen, accept() not needed here

	// ret = listen(Server_fd, 10); // 2nd param is `backlog`, hereby set to MAX_CLIENTS
	// ret = listen(Server_fd, MAX_CLIENTS); // MAX_CLIENTS
	// if (ret < 0)

	// if ( listen(Server_fd, 10) < 0 )
	if ( listen(Server_fd, MAX_CLIENTS) < 0 )
		throw std::runtime_error("Error listening on socket.\n");
		// return (perror("listen failed"), -1);
	
	return (Server_fd);
}

void    handle_sigint(int signum)
{
	std::cout << signum << std::endl;
	(void) signum;

	// DBG
	// std::cout << "is alive: " << g_Server_Alive << std::endl;
	
	g_Server_Alive = false;
	
	// DBG
	// std::cout << "is alive: " << g_Server_Alive << std::endl;
}

void Server::connect_to_server()
{
	this->m_server_socket = /*Server::*/new_socket();

	fd_set		readfds;
	int		socket_fd, activity;
	int		maximum_socket_fd /* not sure about the naming of this one */;
	int		i;

	// this->m_server_socket = Server::new_socket();

	i = -1;
	while (++i < MAX_CLIENTS)
        g_Socket_Queue[i] = 0;
	std::cout << "listening..." << std::endl;
	
	// enter the great "listener while loop"
	//	--- cf. cc5/[..]/socket_server_clients/_.cc
	
	// while ( g_Server_Alive == true && this->m_is_restarting == false)
	while (this->m_is_restarting == false && g_Server_Alive == true)
	{
		//  ~~~now the Handler is a member function ::~~~
		// 	~~~you need a pointer to member in order to invoke it~~~
		// 
		// (?) it can't be done. 
		// 	Signal handlers are too primitive a mechanism to support calling
		// 	of a member function on a particular instance of a class.

		// DBG
		// std::cout << "is alive: " << g_Server_Alive << std::endl;

		/*std::*/signal(SIGINT, handle_sigint);

		

		// socket set cleanup
        FD_ZERO( & readfds);

        // add master socket to set
        FD_SET(this->m_server_socket, & readfds);
        maximum_socket_fd = this->m_server_socket; // retrieve master socket

        // add child sockets to set
		i = -1;
		while (++i < MAX_CLIENTS)
        {
            // socket descriptor

            socket_fd = g_Socket_Queue[i];

			// * (at start -> 
			//		i at : 3;
			//		socket_fd at: 0)

			//	DBG
			// std::cout <<  GREEN "i at : " << i << "; socket_fd at: " YELLOW << socket_fd << nlreset;

            // if valid socket descriptor then add to read list
			//	return of 0 means socket() succeeded
			//	return of non-negio
            if (socket_fd > 0)
                FD_SET(socket_fd , & readfds);

			// highest file descriptor number, need it for the select function
			//	* FD_SETSIZE has a limit of 1024
			//	* People have tried to increase the limit but experienced crashing
			//	* If that many connections are need, use poll()
            
            if (socket_fd > maximum_socket_fd)
                maximum_socket_fd = socket_fd;
        }

        // wait for an activity on one of the sockets , timeout is NULL ,
        // wait indefinitely

        activity = select(maximum_socket_fd + 1 , & readfds , NULL , NULL , NULL);
        if ((activity < 0) && (errno != EINTR))
            std::cerr << ("select error") << std::endl;
			// perror("select error");  

		// connect new user

		if (FD_ISSET(this->m_server_socket, & readfds) && g_Server_Alive)
            new_connection();
		else if (g_Server_Alive)
		{
			i = -1;
			while (++i < MAX_CLIENTS)
			{
				socket_fd = g_Socket_Queue[i];

				// if (FD_ISSET(socket_fd , &readfds))
				// {
				// 	std::string buf;

				// 	// Check if it was for closing , and also read the
				// 	// incoming message

				// 	buf = receive_msg(socket_fd);
				// 	if (!buf.empty())
				// 	{
				// 		XCQ(socket_fd, buf);

				// 		/*std::cout << CYAN "COMMAND RECEIVED :" RESET << buf;
				// 		std::string command(buf);
				// 		int occurrence = buf.find_first_not_of(SEP_CHARSET, 0);
				// 		buf = command.substr(occurrence, buf.length() - occurrence);
				// 		command = buf.substr(0, buf.find_first_of(SEP_CHARSET, 0));
				// 		if (m_commands.find(command) != m_commands.end())
				// 			(m_commands[command])(this, buf, socket_fd);
				// 		*/
				// 		break;
				// 	}
				// }

				if ( ! FD_ISSET(socket_fd , &readfds))
					continue ;
				std::string buf;

				// Check if it was for closing , and also read the
				// incoming message

				buf = receive_msg(socket_fd);
				if (!buf.empty())
				{
					XCQ(socket_fd, buf);

					/*std::cout << CYAN "COMMAND RECEIVED :" RESET << buf;
					std::string command(buf);
					int occurrence = buf.find_first_not_of(SEP_CHARSET, 0);
					buf = command.substr(occurrence, buf.length() - occurrence);
					command = buf.substr(0, buf.find_first_of(SEP_CHARSET, 0));
					if (m_commands.find(command) != m_commands.end())
						(m_commands[command])(this, buf, socket_fd);
					*/
					break;
				}

			}
		}
	}

	clear_all();

	i = -1;
	while (++i < MAX_CLIENTS)
	{
		if (g_Socket_Queue[i] != 0)
		{
			close(g_Socket_Queue[i]);
			g_Socket_Queue[i] = 0;
		}
	}
	close(this->m_server_socket);
	if (this->m_is_restarting && g_Server_Alive)
	{
		this->m_is_restarting = false;
		std::cout << "SERVER RESTARTING..." << std::endl;
		connect_to_server();
	}
}


void Server::new_connection(void)
{
	// socklen_t csize = sizeof(m_server);
	size_t	addrlen = sizeof(m_server);

	this->m_sock_coming = accept(
		this->m_server_socket,
		(struct sockaddr *) & m_server,
		(socklen_t*) & addrlen
	);
	if (this->m_sock_coming < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

	// inform user of socket number - used in send and receive commands

	std::cout << "New connection , socket fd is " << this->m_sock_coming << " , ip is : " << inet_ntoa(m_server.sin_addr) << " , port : " <<  ntohs(m_server.sin_port) << std::endl;
	std::string ret;

	// send new connection greeting message

	size_t	occurrence;
	size_t	first_occurrence;
	bool	password_is_valid;
	bool	nick_is_valid;
	bool	user_is_valid;
	int		i;

	password_is_valid = false;
	nick_is_valid = false;
	user_is_valid = false;
	std::string nick = "", user = "", host = "", serverName = "", realname = "", pass = "";

	ret = this->receive_msg(this->m_sock_coming);
	if (((ret.find("CAP LS") != std::string::npos && ret.find("PASS ") == std::string::npos) || (ret.find("CAP LS") != std::string::npos && ret.find("PASS ") == std::string::npos && ret.find("NICK ") != std::string::npos)) && ret.find("USER ") == std::string::npos)
	{
		ret = this->receive_msg(this->m_sock_coming);
	}
	if ((occurrence = ret.find("PASS ")) != std::string::npos)
	{
		if ((first_occurrence = ret.find_first_not_of(SEP_CHARSET, occurrence + 5)) == std::string::npos)
		{
			forward_message(forward_RPL(461, this, NULL, "PASS", ""), this->m_sock_coming);
			close(this->m_sock_coming);
		}
		else
		{
			i = -1;
			while (ret[++i + first_occurrence] && SEP_CHARSET.find(ret[first_occurrence + i]) == std::string::npos)
			{
				pass += ret[first_occurrence + i];
			}
			if (pass.empty())
			{
				forward_message(forward_RPL(461, this, NULL, "PASS", ""), this->m_sock_coming);
				close(this->m_sock_coming);
			}
			else if (pass != this->m_pass)
			{
				forward_message("WRONG PASSWORD", this->m_sock_coming);
				close(this->m_sock_coming);
			}
			else
			{
				password_is_valid = true;
			}
		}
	}
	else
	{
		forward_message("You need to enter a pass!", this->m_sock_coming);
		close(this->m_sock_coming);
	}
	if (password_is_valid == true)
	{
		if (ret.find("NICK ") == std::string::npos)
		{
			ret = this->receive_msg(this->m_sock_coming);
		}
		if ((occurrence = ret.find("NICK ")) != std::string::npos)
		{
			if ((first_occurrence = ret.find_first_not_of(SEP_CHARSET, occurrence + 5)) == std::string::npos)
			{
				forward_message(forward_RPL(432, this, NULL, nick, ""), this->m_sock_coming);
				close(this->m_sock_coming);
			}
			else
			{
				nick = ret.substr(first_occurrence, ret.find_first_of(BUFFER_ENDS, first_occurrence) - first_occurrence);
				nick = nick.substr(0, nick.find_last_not_of(SEP_CHARSET, nick.size()) + 1);
				if (!nickname_is_valid(nick))
				{
					forward_message(forward_RPL(432, this, NULL, nick, ""), this->m_sock_coming);
					close(this->m_sock_coming);	
				}
				else if (nickname_is_in_use(this, nick))
				{
					forward_message(forward_RPL(433, this, NULL, nick, ""), this->m_sock_coming);
					forward_message("Please try reconnect with an available nickname.", this->m_sock_coming);
					close(this->m_sock_coming);
				}
				else
				{
					nick_is_valid = true;
				}
			}
		}
		else
		{
			forward_message("You have to enter a nickname\nUsage: NICK [nickname]", this->m_sock_coming);
			close(this->m_sock_coming);
		}
		if (user_is_valid == false && nick_is_valid == true)
		{
			if (ret.find("USER ") == std::string::npos)
			{
				ret = this->receive_msg(this->m_sock_coming);
			}
			if ((occurrence = ret.find("USER ")) != std::string::npos)
			{
				int i = 0;

				// username

				if ((first_occurrence = ret.find_first_not_of(SEP_CHARSET, occurrence + 5)) == std::string::npos)
					forward_message(forward_RPL(461, this, NULL, "USER", ""), this->m_sock_coming);
				else
				{
					user = ret.substr(first_occurrence, (i = ret.find_first_of(SEP_CHARSET, first_occurrence)) - first_occurrence);

					// hostname

					if ((first_occurrence = ret.find_first_not_of(SEP_CHARSET, i)) == std::string::npos)
						forward_message(forward_RPL(461, this, NULL, "USER", ""), this->m_sock_coming);
					else
					{
						host = ret.substr(first_occurrence, (i = ret.find_first_of(SEP_CHARSET, first_occurrence)) - first_occurrence);

						// serverName

						if ((first_occurrence = ret.find_first_not_of(SEP_CHARSET, i)) == std::string::npos)
							forward_message(forward_RPL(461, this, NULL, "USER", ""), this->m_sock_coming);
						else
						{
							serverName = ret.substr(first_occurrence, (i = ret.find_first_of(SEP_CHARSET, first_occurrence)) - first_occurrence);

							// realname

							if ((first_occurrence = ret.find_first_not_of(SEP_CHARSET, i)) == std::string::npos)
								forward_message(forward_RPL(461, this, NULL, "USER", ""), this->m_sock_coming);
							else
							{
								realname = ret.substr(first_occurrence, (i = ret.find_first_of(SEP_CHARSET, first_occurrence)) - first_occurrence);
								realname = realname.substr(0, realname.find_last_not_of(SEP_CHARSET, realname.size()) + 1);
							}
						}
					}
				}
				if (!(user.empty() || host.empty() || serverName.empty() || realname.empty()))
					user_is_valid = true;
			}
		}
		if (user_is_valid == false && nick_is_valid == true)
		{
			forward_message("Usage: USER [username] [hostname] [serverName] [realname]", this->m_sock_coming);
			close(this->m_sock_coming);
		}
	}
	// if (password_is_valid == true && m_users.size() < 10 && nick_is_valid == true && user_is_valid == true && g_Server_Alive == true)
	if (password_is_valid == true && m_users.size() < 4 && nick_is_valid == true && user_is_valid == true && g_Server_Alive == true)
	{
		this->m_server_name = serverName;
		User *newUser = new User(nick, user, host, realname);
		this->set_users(this->m_sock_coming, newUser);
		std::cout << "Number of user connected on the server: " << this->m_users.size() << std::endl;
		forward_message(forward_RPL(001, this, newUser, "", ""), this->m_sock_coming);
		forward_message(forward_RPL(002, this, newUser, "", ""), this->m_sock_coming);
		forward_message(forward_RPL(003, this, newUser, "", ""), this->m_sock_coming);
		forward_message(forward_RPL(004, this, newUser, "", ""), this->m_sock_coming);
		forward_MOTD(this->m_sock_coming);

		// add new socket to array of sockets

		i = -1;
		while (++i < MAX_CLIENTS)
		{
			// if position is empty

			if (g_Socket_Queue[i] == 0)
			{
				g_Socket_Queue[i] = this->m_sock_coming;
				break;
			}
		}
	}
	else if (password_is_valid == true && nick_is_valid == true && g_Server_Alive == true && user_is_valid == true)
		forward_message(forward_RPL(005, this, NULL, nick, ""), this->m_sock_coming);
}


std::string Server::receive_msg(int socket_fd) const
{
	char buffer[1024];
	std::string buf = "";
	memset(buffer, 0, 1024);
	while ((buf += buffer).find('\n') == std::string::npos && g_Server_Alive == true)
	{
		if (recv(socket_fd, buffer, 1024, 0) < 0)
			throw std::runtime_error("Error receiving message");
	}
	return (buf);
}


std::string Server::get_server_name() const
{
	return (this->m_server_name);
}


std::string Server::get_port() const
{
	return (this->m_port);
}


std::map<std::string, Channel*> & Server::get_channels()
{
	return (this->m_channels);
}


std::map<int, User*> & Server::get_users()
{
	return (this->m_users);
}


struct sockaddr_in Server::get_server()
{
	return (this->m_server);
}


void Server::get_channels(std::string channel_name, Channel *chan)
{
	this->m_channels.insert(std::make_pair(channel_name, chan));
}


void Server::set_users(int socket_fd, User *user)
{
	this->m_users.insert(std::make_pair(socket_fd, user));
}


void Server::set_is_restarting()
{
	this->m_is_restarting = !this->m_is_restarting;
}


int Server::search_user_by_nickname(std::string nickname)
{
	std::map<int, User*>::iterator it;

	it = this->m_users.begin();
    while (it != this->m_users.end())
    {
        if (nickname == it->second->get_nickname())
            return it->first;
		++it;
    }
    return (-1);
}


std::ostream	& operator << (std::ostream &stdout, std::map<std::string, Channel*> &channels)
{
	std::map<std::string, Channel*>::iterator	it;
	int											i;

	i = 0;
	it = channels.begin();
	while (it != channels.end())
	{
		stdout << "Channel " << i << " name is " << it->first << "." << std::endl;
		++it;
		++i;
	}
	return (stdout);
}


std::ostream	& operator << (std::ostream &stdout, std::map<int, User*> &users)
{
	std::map<int, User*>::iterator	it;
    int								i;

	it = users.begin();
	i = 0;
	while (it != users.end())
	{
		stdout << "User " << i << " nick is " << it->second->get_nickname() << "." << std::endl;
		++it;
		++i;
	}
	return (stdout);
}


std::ostream	& operator << (std::ostream &stdout, User &user)
{
	std::set<std::string>			channels;
	std::set<std::string>::iterator	it;
    int								i;

	channels = user.get_channels();
	it = channels.begin();
	i = 0;
    while (it != channels.end())
    {
        stdout << "Channel " << i << " of User " << user.get_nickname() << " is called " << *it << std::endl;
		++it;
		++i;
	}
    return (stdout);
}


void Server::clear_all()
{
	std::map<std::string, Channel*>::iterator	cit;
	std::map<int, User*>::iterator				uit;

	uit = this->m_users.begin();
	while (uit != this->m_users.end()) // Crucial fix
	{
		delete	(uit->second);

		++uit;
	}
	this->m_users.clear();

	cit = this->m_channels.begin();
	while (cit != this->m_channels.end())
	{
		delete	(cit->second);

		++cit;
	}
	this->m_channels.clear();
}


void Server::forward_MOTD(int socket_fd)  // Message of the Day
{
	size_t	choice = rand() % 6;

	void	(Server::*forward_MOTD_modes[6]) (int) =
	{
		& Server::forward_MOTD_Slant_Relief,
		& Server::forward_MOTD_Dot_Matrix,
		& Server::forward_MOTD_Mahjong,
		& Server::forward_MOTD_Doh,
		& Server::forward_MOTD_la_nuit_mes_yeux,
		& Server::forward_MOTD_voir_un_univers
	};

	(this->*forward_MOTD_modes[choice])(socket_fd);

}


void Server::forward_MOTD_voir_un_univers(int socket_fd)
{
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("            Voir un univers dans un grain de sable,", socket_fd);
	forward_message("            et un paradis dans une fleur sauvage.", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
}


void Server::forward_MOTD_la_nuit_mes_yeux(int socket_fd)
{
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("            La nuit mes yeux t'eclairent", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
}


void Server::forward_MOTD_Slant_Relief(int socket_fd)
{
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("__/\\\\\\\\\\\\\\\\\\\\\\____/\\\\\\\\\\\\\\\\\\____________/\\\\\\\\\\\\\\\\\\_        ", socket_fd);
	forward_message(" _\\// // /\\\\\\// /___/\\\\\\// // // /\\\\\\_______/\\\\\\// // // // __       ", socket_fd);
	forward_message("  _____\\/\\\\\\_____\\/\\\\\\_____\\/\\\\\\_____/\\\\\\/___________      ", socket_fd);
	forward_message("   _____\\/\\\\\\_____\\/\\\\\\\\\\\\\\\\\\\\\\/_____/\\\\\\_____________     ", socket_fd);
	forward_message("    _____\\/\\\\\\____\\/\\\\\\// // // \\\\\\____\\/\\\\\\_____________    ", socket_fd);
	forward_message("     _____\\/\\\\\\_____\\/\\\\\\____\\// \\\\\\___\\// \\\\\\____________   ", socket_fd);
	forward_message("      _____\\/\\\\\\_____\\/\\\\\\_____\\// \\\\\\___\\// /\\\\\\__________  ", socket_fd);
	forward_message("       __/\\\\\\\\\\\\\\\\\\\\\\_\\/\\\\\\______\\// \\\\\\____\\// // \\\\\\\\\\\\\\\\\\_ ", socket_fd);
	forward_message("        _\\// // // // // /__\\// /________\\// /________\\// // // // /__", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
}


void Server::forward_MOTD_Dot_Matrix(int socket_fd)
{
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("       _  _  _  _  _  _  _        _  _  _       ", socket_fd);
	forward_message("      (_)(_)(_)(_)(_)(_)(_) _  _ (_)(_)(_) _    ", socket_fd);
	forward_message("         (_)   (_)         (_)(_)         (_)   ", socket_fd);
	forward_message("         (_)   (_) _  _  _ (_)(_)               ", socket_fd);
	forward_message("         (_)   (_)(_)(_)(_)   (_)               ", socket_fd);
	forward_message("         (_)   (_)   (_) _    (_)          _    ", socket_fd);
	forward_message("       _ (_) _ (_)      (_) _ (_) _  _  _ (_)   ", socket_fd);
	forward_message("      (_)(_)(_)(_)         (_)   (_)(_)(_)      ", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
}


void Server::forward_MOTD_Mahjong(int socket_fd)
{
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message(" .----------------.  .----------------.  .----------------. ", socket_fd);
	forward_message("| .--------------. || .--------------. || .--------------. |", socket_fd);
	forward_message("| |     _____    | || |  _______     | || |     ______   | |", socket_fd);
	forward_message("| |    |_   _|   | || | |_   __ \\    | || |   .' ___  |  | |", socket_fd);
	forward_message("| |      | |     | || |   | |__) |   | || |  / .'   \\_|  | |", socket_fd);
	forward_message("| |      | |     | || |   |  __ /    | || |  | |         | |", socket_fd);
	forward_message("| |     _| |_    | || |  _| |  \\ \\_  | || |  \\ `.___.'\\  | |", socket_fd);
	forward_message("| |    |_____|   | || | |____| |___| | || |   `._____.'  | |", socket_fd);
	forward_message("| |              | || |              | || |              | |", socket_fd);
	forward_message("| '--------------' || '--------------' || '--------------' |", socket_fd);
	forward_message(" '----------------'  '----------------'  '----------------' ", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
}


void Server::forward_MOTD_Doh(int socket_fd)
{
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("IIIIIIIIIIRRRRRRRRRRRRRRRRR           CCCCCCCCCCCCC", socket_fd);
	forward_message("I::::::::IR::::::::::::::::R       CCC::::::::::::C", socket_fd);
	forward_message("II::::::IIRR:::::R     R:::::R  C:::::CCCCCCCC::::C", socket_fd);
	forward_message("  I::::I    R::::R     R:::::R C:::::C       CCCCCC", socket_fd);
	forward_message("  I::::I    R::::R     R:::::RC:::::C              ", socket_fd);
	forward_message("  I::::I    R::::RRRRRR:::::R C:::::C              ", socket_fd);
	forward_message("  I::::I    R:::::::::::::RR  C:::::C              ", socket_fd);
	forward_message("  I::::I    R::::RRRRRR:::::R C:::::C              ", socket_fd);
	forward_message("  I::::I    R::::R     R:::::RC:::::C              ", socket_fd);
	forward_message("  I::::I    R::::R     R:::::RC:::::C              ", socket_fd);
	forward_message("  I::::I    R::::R     R:::::R C:::::C       CCCCCC", socket_fd);
	forward_message("II::::::IIRR:::::R     R:::::R  C:::::CCCCCCCC::::C", socket_fd);
	forward_message("I::::::::IR::::::R     R:::::R   CC:::::::::::::::C", socket_fd);
	forward_message("I::::::::IR::::::R     R:::::R     CCC::::::::::::C", socket_fd);
	forward_message("IIIIIIIIIIRRRRRRRR     RRRRRRR        CCCCCCCCCCCCC", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
	forward_message("                                                   ", socket_fd);
}

// end
