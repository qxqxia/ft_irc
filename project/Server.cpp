#include "Freenode.hpp"

int		g_server_client_socket[ MAX_CLIENTS ];
bool	g_server_is_alive = true;

void    handle_sigint(int signum)
{
	(void)	signum;

	g_server_is_alive = false;
}

Server::Server(const std::string & port, const std::string & password)
 : m_port(port), m_pass(password), m_server_name(), m_is_restarting(false)
 {
	this->m_commands["NICK"] = & nick;
	this->m_commands["JOIN"] = & join;
	this->m_commands["PRIVMSG"] = & privmsg;
	this->m_commands["NOTICE"] = & privmsg;
	this->m_commands["PING"] = & ping;
	this->m_commands["PING"] = & pong;
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

	this->m_bot = new Bot; ///	Added

}

Server::~Server()
{
	this->m_commands.clear();

	delete	this->m_bot; ///	Added
}


void Server::connect_to_server()
{
	this->m_server_socket = Server::new_socket();

	fd_set		rfds;

	/*
	From sys/select.h

	typedef struct	fd_set {
		u_int	fd_count;
		SOCKET	fd_array[FD_SETSIZE];
	} fd_set;
	*/

	int		socket_fd, max_socket_fd, activity;
	int		i = -1;


	while (++i < MAX_CLIENTS)
    {
		g_server_client_socket[i] = 0;
	}

	std::cout << "listening..." << std::endl;

	while (this->m_is_restarting == false && g_server_is_alive == true)
	{
		// Can we make a separate handler function?

		// it can't be done. 
		//	Signal handlers are too primitive a mechanism to support calling
		//	of a member function on a particular instance of a class.

		std::signal(SIGINT, handle_sigint);

		//  now the Handler is a member function ::
		//	you need a pointer to member in order to invoke it

		// clear the socket set

        FD_ZERO(& rfds);

        // add master socket to set

        FD_SET(this->m_server_socket, & rfds);
        max_socket_fd = this->m_server_socket;

        // add child sockets to set


		i = -1;
		while (++i < MAX_CLIENTS)
        {
            //socket descriptor

            socket_fd = g_server_client_socket[i];

            //if valid socket descriptor then add to read list

            if (socket_fd > 0)
            {
				FD_SET(socket_fd , & rfds);
			}

            //highest file descriptor number, need it for the select function
 
            if (socket_fd > max_socket_fd)
			{
				max_socket_fd = socket_fd;
			}
        }

        //wait for an activity on one of the sockets , timeout is NULL ,
        //so wait indefinitely

        activity = select(
			max_socket_fd + 1 , 
			& rfds,
			NULL,
			NULL,
			NULL
		);

        if ((activity < 0) && (errno != EINTR))
        {
			std::cerr << ("select() gives error") << std::endl;
		}

		//connect new user

		if (FD_ISSET(this->m_server_socket, & rfds) && g_server_is_alive)
		// FD_ISSET -- tests to see if an FD is part of the set; this is useful after select() returns
        {
			Server::new_connection();
			// new_connection();
		}
		else if (g_server_is_alive)
		{

			i = -1;
			while (++i < MAX_CLIENTS)
			{
				socket_fd = g_server_client_socket[i];

				if (FD_ISSET(socket_fd , & rfds))
				{
					std::string buf;

					//	+ check command is for stopping the server
					//	+ try to read next incoming message

					buf = get_input_from_client_sfd(socket_fd);
					if ( ! buf.empty())
					{
						std::cout << CYAN "(dgb)(input):\t" RESET << buf;

						std::string		command(buf);

						int	L = buf.find_first_not_of(SEP_CHARSET, 0);

						// std::cout << CYAN "(dbg)(echo 0): " RESET << buf << nl;

						buf = command.substr(L, buf.length() - L);
						command = buf.substr(0, buf.find_first_of(SEP_CHARSET, 0));

						std::cout << CYAN "(dbg)(echo 1):\t" RESET << (
							(buf.empty()) ? "(None)" : buf
						);

						// if (buf.empty())
						// 	std::cout << CYAN "(dbg)(echo 1): " RESET << "(None)";
						// else
						// 	std::cout << CYAN "(dbg)(echo 1): " RESET << buf;

						std::cout << CYAN "(dbg)(echo 2):\t" RESET << (
							(command.empty()) ? "(None)" : command
						) << nl2;

						// if (command.empty())	
						// 	std::cout << CYAN "(dbg)(echo 2): " RESET << "(None)" << nl2;
						// else
						// 	std::cout << CYAN "(dbg)(echo 2): " RESET << command << nl2;

						if (m_commands.find(command) != m_commands.end())
						//{
							(m_commands[command])(this, buf, socket_fd);
							 // break ; // Greatest bug of all time
						//} // Greatest bug of all time
						break ; // BUG FIX : re-added 
					}
				}
			}
		}
	}
	clear_all();
	
	i = -1;
	while (++i < MAX_CLIENTS)
	{
		if (g_server_client_socket[i] != 0)
		{
			close(g_server_client_socket[i]);
			g_server_client_socket[i] = 0;
		}
	}
	close(this->m_server_socket);
	if (this->m_is_restarting == true && g_server_is_alive == true)
	{
		this->m_is_restarting = false;
		std::cout << "SERVER RESTARTING..." << std::endl;

		Server::connect_to_server();
	}
}

void Server::new_connection()
{
	socklen_t csize = sizeof(m_server);
	if ((this->m_sock_coming = accept(this->m_server_socket, (struct sockaddr *)& m_server, & csize)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

	//inform user of socket number - used in send and receive commands

	std::cout
	<< "New connection, socket fd is " << this->m_sock_coming
	<< " , ip is : " << inet_ntoa(m_server.sin_addr)
	<< " , port : " <<  ntohs(m_server.sin_port)
	<< nl /*std::endl*/;

	std::string ret;

	//send new connection greeting message

	size_t occ;
	size_t first_occurrence;

	bool	password_is_valid = false;
	bool	nickname_is_valid = false;
	bool	username_is_valid = false;

	std::string		nick = "";
	std::string		user = "";
	std::string		host = "";
	std::string		server_name = "";
	std::string		real_name = "";
	std::string		pass = "";

	ret = this->get_input_from_client_sfd(this->m_sock_coming);
	if (((ret.find("CAP LS") != std::string::npos && ret.find("PASS ") == std::string::npos) || (ret.find("CAP LS") != std::string::npos && ret.find("PASS ") == std::string::npos && ret.find("NICK ") != std::string::npos)) && ret.find("USER ") == std::string::npos)
		ret = this->get_input_from_client_sfd(this->m_sock_coming);
	if ((occ = ret.find("PASS ")) != std::string::npos)
	{
		if ((first_occurrence = ret.find_first_not_of(SEP_CHARSET, occ + 5)) == std::string::npos)
		{
			Broadcast(get_RPL_ERR(461, this, NULL, "PASS", ""), this->m_sock_coming);
			close(this->m_sock_coming);
		}
		else
		{

			int		i = 0;
			while (ret[first_occurrence + i] && SEP_CHARSET.find(ret[first_occurrence + i]) == std::string::npos)			
			{
				pass += ret[first_occurrence + i];
				++i;
			}
			if (pass.empty())
			{
				Broadcast(get_RPL_ERR(461, this, NULL, "PASS", ""), this->m_sock_coming);
				close(this->m_sock_coming);
			}
			else if (pass != this->m_pass)
			{
				Broadcast("WRONG PASSWORD", this->m_sock_coming);
				close(this->m_sock_coming);
			}
			else
				password_is_valid = true;
		}
	}
	else
	{
		Broadcast("You need to enter a pass!", this->m_sock_coming);
		close(this->m_sock_coming);
	}
	if (password_is_valid == true)
	{
		if (ret.find("NICK ") == std::string::npos)
			ret = this->get_input_from_client_sfd(this->m_sock_coming);
		if ((occ = ret.find("NICK ")) != std::string::npos)
		{
			if ((first_occurrence = ret.find_first_not_of(SEP_CHARSET, occ + 5)) == std::string::npos)
			{
				Broadcast(get_RPL_ERR(432, this, NULL, nick, ""), this->m_sock_coming);
				close(this->m_sock_coming);
			}
			else
			{
				nick = ret.substr(first_occurrence, ret.find_first_of(BUFFER_ENDS, first_occurrence) - first_occurrence);
				nick = nick.substr(0, nick.find_last_not_of(SEP_CHARSET, nick.size()) + 1);
				if (!nickname_is_validated(nick))
				{
					Broadcast(get_RPL_ERR(432, this, NULL, nick, ""), this->m_sock_coming);
					close(this->m_sock_coming);	
				}
				else if (nickname_is_in_use(this, nick))
				{
					Broadcast(get_RPL_ERR(433, this, NULL, nick, ""), this->m_sock_coming);
					Broadcast("Please try reconnect with an available nickname.", this->m_sock_coming);
					close(this->m_sock_coming);
				}
				else
				{
					nickname_is_valid = true;
				}
			}
		}
		else
		{
			Broadcast("You have to enter a nickname\nUsage: NICK [nickname]", this->m_sock_coming);
			close(this->m_sock_coming);
		}
		if (username_is_valid == false && nickname_is_valid == true)
		{
			if (ret.find("USER ") == std::string::npos)
				ret = this->get_input_from_client_sfd(this->m_sock_coming);
			if ((occ = ret.find("USER ")) != std::string::npos)
			{
				int i = 0;

				//username

				if ((first_occurrence = ret.find_first_not_of(SEP_CHARSET, occ + 5)) == std::string::npos)
					Broadcast(get_RPL_ERR(461, this, NULL, "USER", ""), this->m_sock_coming);
				else
				{
					user = ret.substr(first_occurrence, (i = ret.find_first_of(SEP_CHARSET, first_occurrence)) - first_occurrence);

					//	hostname

					if ((first_occurrence = ret.find_first_not_of(SEP_CHARSET, i)) == std::string::npos)
						Broadcast(get_RPL_ERR(461, this, NULL, "USER", ""), this->m_sock_coming);
					else
					{
						host = ret.substr(first_occurrence, (i = ret.find_first_of(SEP_CHARSET, first_occurrence)) - first_occurrence);

						//	server_name

						if ((first_occurrence = ret.find_first_not_of(SEP_CHARSET, i)) == std::string::npos)
							Broadcast(get_RPL_ERR(461, this, NULL, "USER", ""), this->m_sock_coming);
						else
						{
							server_name = ret.substr(first_occurrence, (i = ret.find_first_of(SEP_CHARSET, first_occurrence)) - first_occurrence);

							//	real_name

							if ((first_occurrence = ret.find_first_not_of(SEP_CHARSET, i)) == std::string::npos)
								Broadcast(get_RPL_ERR(461, this, NULL, "USER", ""), this->m_sock_coming);
							else
							{
								real_name = ret.substr(first_occurrence, (i = ret.find_first_of(SEP_CHARSET, first_occurrence)) - first_occurrence);
								real_name = real_name.substr(0, real_name.find_last_not_of(SEP_CHARSET, real_name.size()) + 1);
							}
						}
					}
				}
				if (!(user.empty() || host.empty() || server_name.empty() || real_name.empty()))
					username_is_valid = true;
			}
		}
		if (username_is_valid == false && nickname_is_valid == true)
		{
			Broadcast("Usage: USER [username] [hostname] [server_name] [real_name]", this->m_sock_coming);
			close(this->m_sock_coming);
		}
	}
	// if (password_is_valid == true && m_users.size() < 10 && nickname_is_valid == true && username_is_valid == true && g_server_is_alive == true)
	if (password_is_valid == true && m_users.size() < 4 && nickname_is_valid == true && username_is_valid == true && g_server_is_alive == true)
	{
		this->m_server_name = server_name;
		User *newUser = new User(nick, user, host, real_name);
		this->set_users(this->m_sock_coming, newUser);
		std::cout << "Number of user connected on the server: " << this->m_users.size() << std::endl;
		Broadcast(get_RPL_ERR(001, this, newUser, "", ""), this->m_sock_coming);
		Broadcast(get_RPL_ERR(002, this, newUser, "", ""), this->m_sock_coming);
		Broadcast(get_RPL_ERR(003, this, newUser, "", ""), this->m_sock_coming);
		Broadcast(get_RPL_ERR(004, this, newUser, "", ""), this->m_sock_coming);
		Forward_MOTD(this->m_sock_coming);

		//add new socket to array of sockets


		int		i = -1;
		while (++i < MAX_CLIENTS)
		{
			//if position is empty

			if ( !g_server_client_socket[i])
			{
				g_server_client_socket[i] = this->m_sock_coming;
				break ;
			}
		}
	}
	else if (password_is_valid == true && nickname_is_valid == true && g_server_is_alive == true && username_is_valid == true)
		Broadcast(get_RPL_ERR(005, this, NULL, nick, ""), this->m_sock_coming);
}

int Server::new_socket()
{
	int		sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0)
	{
		throw std::runtime_error("Error creating socket.\n");
	}

	int		tmp = 1;

	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, & tmp, sizeof(tmp)))
	{
		throw std::runtime_error("Error while setting up socket.\n");
	}
	if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1)
	{
		throw std::runtime_error("Error while setting socket NON-BLOCKING mode.\n");
	}

	m_server.sin_addr.s_addr = INADDR_ANY;
	m_server.sin_port = htons(static_cast<unsigned short>(std::strtoul(this->m_port.c_str(), NULL, 0)));
	m_server.sin_family = AF_INET;

	if (bind(sock, (const struct sockaddr*)&m_server, sizeof(m_server)) < 0)
	{
		throw std::runtime_error("Error binding socket.\n");
	}
	if (listen(sock, MAX_CLIENTS /* 10 */) < 0)
	{
		throw std::runtime_error("Error listening on socket.\n");
	}

	return (sock);
}

std::string Server::get_input_from_client_sfd(int socket_fd) const
{
	char		buffer[1024];
	std::string		buf = "";

	memset(buffer, 0, 1024); // copy 0 to every one of all 1024 slots

	while ((buf += buffer).find('\n') == std::string::npos && g_server_is_alive == true)
	{
		if (recv(socket_fd, buffer, 1024, 0) < 0)
		{
			throw std::runtime_error("Error receiving message");
		}
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
    
	std::map<int, User*>::iterator	it;

	it = this->m_users.begin();
	while (it != this->m_users.end())
	{
        // if (nickname.compare(it->second->get_nickname()) == 0)
		if (it->second->get_nickname() == nickname)
            return it->first;
		++it;
    }
    return (-1);
}

std::ostream	& operator << (std::ostream & stdout, std::map<std::string, Channel*> & channels)
{

	std::map<std::string, Channel*>::iterator	it;
	int	i = 0;

	it = channels.begin();
	while (it != channels.end())
	{
		stdout << "Channel " << i << " name is " << it->first << "." << std::endl;
		++it;
		++i;
	}
	return (stdout);
}

std::ostream	& operator << (std::ostream & stdout, std::map<int, User*> &users)
{

	std::map<int, User*>::iterator	it;
	int	i = 0;

	while (it != users.end())
	{
		stdout << "User " << i << " nick is " << it->second->get_nickname() << "." << std::endl;
		++it;
		++i;
	}
	return (stdout);
}

std::ostream	& operator << (std::ostream & stdout, User &user)
{
    int i = 0;
    std::set<std::string> channels = user.get_channels();


	std::set<std::string>::iterator		it;

	it = channels.begin();
	while (it != channels.end())
	{
        stdout << "Channel " << i << " of User " << user.get_nickname() << " is called " << *it << std::endl;
		++it;
    }
    return (stdout);
}

void Server::clear_all()
{
	std::map<int, User*>::iterator				itU;
	std::map<std::string, Channel*>::iterator	itC;


	itU = this->m_users.begin();
	while (itU != this->m_users.end())
	{
		delete	itU->second;

		++itU;
	}
	this->m_users.clear();


	itC = this->m_channels.begin();
	while (itC != this->m_channels.end())
	{
		delete	itC->second;

		++itC;
	}

	this->m_channels.clear();
}

void Server::Forward_MOTD(int socket_fd)  // Message of the Day
{
	size_t	choice = rand() % 6;

	void	(Server::*Forward_MOTD_modes[6]) (int) =
	{
		& Server::Forward_MOTD_Slant_Relief,
		& Server::Forward_MOTD_Dot_Matrix,
		& Server::Forward_MOTD_Mahjong,
		& Server::Forward_MOTD_Doh,
		& Server::Forward_MOTD_la_nuit_mes_yeux,
		& Server::Forward_MOTD_voir_un_univers
	};

	(this->*Forward_MOTD_modes[choice])(socket_fd);

}


void Server::Forward_MOTD_voir_un_univers(int socket_fd)
{
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("            Voir un univers dans un grain de sable,", socket_fd);
	Broadcast("            et un paradis dans une fleur sauvage.", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
}


void Server::Forward_MOTD_la_nuit_mes_yeux(int socket_fd)
{
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("            La nuit mes yeux t'eclairent", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
}
void Server::Forward_MOTD_Slant_Relief(int socket_fd)
{
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("__/\\\\\\\\\\\\\\\\\\\\\\____/\\\\\\\\\\\\\\\\\\____________/\\\\\\\\\\\\\\\\\\_        ", socket_fd);
	Broadcast(" _\\/////\\\\\\///___/\\\\\\///////\\\\\\_______/\\\\\\////////__       ", socket_fd);
	Broadcast("  _____\\/\\\\\\_____\\/\\\\\\_____\\/\\\\\\_____/\\\\\\/___________      ", socket_fd);
	Broadcast("   _____\\/\\\\\\_____\\/\\\\\\\\\\\\\\\\\\\\\\/_____/\\\\\\_____________     ", socket_fd);
	Broadcast("    _____\\/\\\\\\____\\/\\\\\\//////\\\\\\____\\/\\\\\\_____________    ", socket_fd);
	Broadcast("     _____\\/\\\\\\_____\\/\\\\\\____\\//\\\\\\___\\//\\\\\\____________   ", socket_fd);
	Broadcast("      _____\\/\\\\\\_____\\/\\\\\\_____\\//\\\\\\___\\///\\\\\\__________  ", socket_fd);
	Broadcast("       __/\\\\\\\\\\\\\\\\\\\\\\_\\/\\\\\\______\\//\\\\\\____\\////\\\\\\\\\\\\\\\\\\_ ", socket_fd);
	Broadcast("        _\\///////////__\\///________\\///________\\/////////__", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
}


void Server::Forward_MOTD_Dot_Matrix(int socket_fd)
{
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("       _  _  _  _  _  _  _        _  _  _       ", socket_fd);
	Broadcast("      (_)(_)(_)(_)(_)(_)(_) _  _ (_)(_)(_) _    ", socket_fd);
	Broadcast("         (_)   (_)         (_)(_)         (_)   ", socket_fd);
	Broadcast("         (_)   (_) _  _  _ (_)(_)               ", socket_fd);
	Broadcast("         (_)   (_)(_)(_)(_)   (_)               ", socket_fd);
	Broadcast("         (_)   (_)   (_) _    (_)          _    ", socket_fd);
	Broadcast("       _ (_) _ (_)      (_) _ (_) _  _  _ (_)   ", socket_fd);
	Broadcast("      (_)(_)(_)(_)         (_)   (_)(_)(_)      ", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
}


void Server::Forward_MOTD_Mahjong(int socket_fd)
{
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast(" .----------------.  .----------------.  .----------------. ", socket_fd);
	Broadcast("| .--------------. || .--------------. || .--------------. |", socket_fd);
	Broadcast("| |     _____    | || |  _______     | || |     ______   | |", socket_fd);
	Broadcast("| |    |_   _|   | || | |_   __ \\    | || |   .' ___  |  | |", socket_fd);
	Broadcast("| |      | |     | || |   | |__) |   | || |  / .'   \\_|  | |", socket_fd);
	Broadcast("| |      | |     | || |   |  __ /    | || |  | |         | |", socket_fd);
	Broadcast("| |     _| |_    | || |  _| |  \\ \\_  | || |  \\ `.___.'\\  | |", socket_fd);
	Broadcast("| |    |_____|   | || | |____| |___| | || |   `._____.'  | |", socket_fd);
	Broadcast("| |              | || |              | || |              | |", socket_fd);
	Broadcast("| '--------------' || '--------------' || '--------------' |", socket_fd);
	Broadcast(" '----------------'  '----------------'  '----------------' ", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
}


void Server::Forward_MOTD_Doh(int socket_fd)
{
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("IIIIIIIIIIRRRRRRRRRRRRRRRRR           CCCCCCCCCCCCC", socket_fd);
	Broadcast("I::::::::IR::::::::::::::::R       CCC::::::::::::C", socket_fd);
	Broadcast("II::::::IIRR:::::R     R:::::R  C:::::CCCCCCCC::::C", socket_fd);
	Broadcast("  I::::I    R::::R     R:::::R C:::::C       CCCCCC", socket_fd);
	Broadcast("  I::::I    R::::R     R:::::RC:::::C              ", socket_fd);
	Broadcast("  I::::I    R::::RRRRRR:::::R C:::::C              ", socket_fd);
	Broadcast("  I::::I    R:::::::::::::RR  C:::::C              ", socket_fd);
	Broadcast("  I::::I    R::::RRRRRR:::::R C:::::C              ", socket_fd);
	Broadcast("  I::::I    R::::R     R:::::RC:::::C              ", socket_fd);
	Broadcast("  I::::I    R::::R     R:::::RC:::::C              ", socket_fd);
	Broadcast("  I::::I    R::::R     R:::::R C:::::C       CCCCCC", socket_fd);
	Broadcast("II::::::IIRR:::::R     R:::::R  C:::::CCCCCCCC::::C", socket_fd);
	Broadcast("I::::::::IR::::::R     R:::::R   CC:::::::::::::::C", socket_fd);
	Broadcast("I::::::::IR::::::R     R:::::R     CCC::::::::::::C", socket_fd);
	Broadcast("IIIIIIIIIIRRRRRRRR     RRRRRRR        CCCCCCCCCCCCC", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
	Broadcast("                                                   ", socket_fd);
}

Bot		*Server::get_bot() const
{
	return (this->m_bot);
}