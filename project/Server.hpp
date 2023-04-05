#pragma once
#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include "Freenode.hpp"
using	namespace std;

class	Server
{
public:
	typedef void (* command) (Server *, string, int);

private:
	int	m_server_socket;
	int	m_sock_coming;

	const string	m_port;
	const string	m_pass;

	struct sockaddr_in	m_server;
	string			m_server_name;

	map<string, command>	m_commands;
	map<string, Channel*>	m_channels;
	map<int, User *>			m_users;

	int		new_socket();
	void	new_connection();
	bool	m_is_restarting;
	// void    handle_sigint(int signum);


public:
	Server(const string & port, const string & pass);
	~Server();

	void	connect_to_server();

	map<string, Channel*>	& get_channels();
	map<int, User*> & get_users() ;

	string	receive_msg(int sd) const;
	string	get_server_name() const;
	string	get_port() const;

	struct sockaddr_in	get_server();

	void	get_channels(string channel_name, Channel *chan);
	void	set_users(int sd, User *user);
	void	set_is_restarting();

	int		search_user_by_nickname(string nickname);

	// from what i make out, these cannot be const 
	// bc fn array dictates this->*
	void	forward_MOTD(int socket_fd);
	void	forward_MOTD_Slant_Relief(int socket_fd);
	void	forward_MOTD_Dot_Matrix(int socket_fd);
	void	forward_MOTD_Mahjong(int socket_fd);
	void	forward_MOTD_Doh(int socket_fd);
	void	forward_MOTD_la_nuit_mes_yeux(int socket_fd);
	void	forward_MOTD_voir_un_univers(int socket_fd);

	void	XCQ(int socket_fd, string buf);

	void	clear_all();

};

ostream & operator << (ostream &, map<string, Channel*> & chans);
ostream & operator << (ostream &, map<int, User*> & users);
ostream & operator << (ostream &, User & user);

#endif



