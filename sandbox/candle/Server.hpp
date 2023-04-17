#pragma once
#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include "Freenode.hpp"
// using	namespace std;

class	Server
{
public:
	typedef void (* command) (Server *, std::string, int);

private:
	int	m_server_socket;
	int	m_sock_coming;

	const std::string	m_port;
	const std::string	m_pass;

	struct sockaddr_in	m_server;
	std::string			m_server_name;

	std::map<std::string, command>	m_commands;
	std::map<std::string, Channel*>	m_channels;
	std::map<int, User *>			m_users;

	int		new_socket();
	void	new_connection();
	bool	m_is_restarting;
	// void    handle_sigint(int signum);


public:
	Server(const std::string & port, const std::string & pass);
	~Server();

	void	connect_to_server();

	std::map<std::string, Channel*>	& get_channels();
	std::map<int, User*> & get_users() ;

	std::string	receive_msg(int sd) const;
	std::string	get_server_name() const;
	std::string	get_port() const;

	struct sockaddr_in	get_server();

	void	get_channels(std::string channel_name, Channel *chan);
	void	set_users(int sd, User *user);
	void	set_is_restarting();

	int		search_user_by_nickname(std::string nickname);

	// from what i make out, these cannot be const 
	// bc fn array dictates this->*
	void	forward_MOTD(int socket_fd);
	void	forward_MOTD_Slant_Relief(int socket_fd);
	void	forward_MOTD_Dot_Matrix(int socket_fd);
	void	forward_MOTD_Mahjong(int socket_fd);
	void	forward_MOTD_Doh(int socket_fd);
	void	forward_MOTD_la_nuit_mes_yeux(int socket_fd);
	void	forward_MOTD_voir_un_univers(int socket_fd);

	void	XCQ(int socket_fd, std::string buf);

	void	clear_all();

};

std::ostream & operator << (std::ostream &, std::map<std::string, Channel*> & chans);
std::ostream & operator << (std::ostream &, std::map<int, User*> & users);
std::ostream & operator << (std::ostream &, User & user);

#endif


