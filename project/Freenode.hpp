#pragma once
#ifndef __FREENODE_HPP__
# define __FREENODE_HPP__

# include "iostream"

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

# include "Bot.hpp" // Added

# include "User.hpp"
# include "Command.hpp"
# include "Channel.hpp"
# include "Server.hpp"
# include "RPL_ERR.hpp"



# define SERVER_NAME "localhost"
# define ver "7.0"
# define OPER_PASS "1007"

# define USER_MODE \
				"roi"
# define CHANNEL_MODE \
				"maltvonkb" // "ovamntlkb"

# define FIND_CHANNEL(channel_name) \
		serv->get_channels().find(channel_name)->second
# define FIND_USER(sd) \
		serv->get_users().find(sd)->second

/*
# define CHECKUSERMODE(sd, mode) \
	FIND_USER(sd)->get_mode().find(mode) == std::string::npos ? true : false
*/
/*
# define CHECK_CHANNEL_MODE(channel_name, mode) \
	FIND_CHANNEL(channel_name)->get_mode().find(mode) == std::string::npos ? true : false
*/

# define BUFFER_ENDS std::string("\r\n")
# define SEP_CHARSET std::string(" \t\r\n")

# define MAX_CLIENTS 4
// # define MAX_CLIENTS 10

class	Server;

class	Channel;
class	User;

class	Bot; // Added


std::string		anonymous_output();
std::string		user_output(User *Us);
std::string		bot_output(std::string name); /// Added


void	Broadcast(std::string msg, int sd);

void	disconnect_user(Server *, int sd);

void	send_everyone_in_channel(std::string msg, Channel *chan);
void	send_everyone_in_channel_except_user(std::string msg, Channel *chan, int sd);
void	send_everyone(std::string msg, std::map<int, User *> Umap);

bool	nickname_is_in_use(Server *, std::string nick);
bool	nickname_is_validated(std::string nick);

time_t	get_time(void); // Added

#endif
