#pragma once
#ifndef __FREENODE_HPP__
# define __FREENODE_HPP__

# include "iostream"
# include "Fmt.hpp"

# include "map"
# include "set"
# include "sstream"
# include "fstream"
# include "algorithm"

# include "Fmt.hpp"

# include "ctime"
# include "cstdio"
# include "cstdlib"
# include "cstring"
# include "csignal"
# include "cstdlib"

# include "unistd.h"
# include "fcntl.h"
# include "errno.h"

# include "sys/socket.h"
# include "netinet/in.h"
# include "arpa/inet.h"

# include "User.hpp"
# include "Command.hpp"
# include "Channel.hpp"
# include "Server.hpp"
# include "RPL_ERR.hpp"

// using	namespace std;

# define ver "5.0"
# define OPER_PW "1008"

# define SERVER_NAME "localhost"
# define USER_MODE "ior"
# define CHANNEL_MODE "ovamntlkb"

# define FIND_CHANNEL(channel_name) serv->get_channels().find(channel_name)->second
# define FIND_USER(sd)	serv->get_users().find(sd)->second

# define BUFFER_ENDS std::string("\r\n")
# define SEP_CHARSET std::string(" \t\r\n")
# define MAX_CLIENTS 4
// # define MAX_CLIENTS 10

class	Server;
class	User;
class	Channel;

std::string	user_output(User *user);
std::string	anonymous_output();

void	disconnect_user(Server *serv, int sd);
void	forward_message(std::string message, int sd);
void	send_everyone_in_channel(std::string message, Channel *chan);
void	send_everyone_in_channel_except_user(std::string message, Channel *chan, int sd);
void	send_everyone(std::string message, std::map<int, User *> users);
bool	nickname_is_in_use(Server *serv, std::string nick);
bool	nickname_is_valid(std::string nick);

time_t	get_time(void);

#endif
