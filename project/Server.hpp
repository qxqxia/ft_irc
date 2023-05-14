#pragma once
#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include "Freenode.hpp"

class	Server
{
public:
	typedef void (* command) (Server *, std::string, int);

private:
	int	m_server_socket;
	int	m_socket_incoming;

	Bot		*m_bot; // Added
	// Bot		*is_bot_in_channel() const ; // Added // should be public

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
	Server(const std::string &, const std::string &);
	~Server();

	void	connect_to_server();

	std::map<std::string, Channel*>	& get_channels();
	std::map<int, User*> & get_users() ;

	std::string	get_input_from_client_sfd(int socket_fd) const;
	std::string	get_server_name() const;
	std::string	get_port() const;

	struct sockaddr_in	get_server();

	void	get_channels(std::string channel_name, Channel *);
	void	set_users(int sd, User *);
	void	toggle_is_restarting();

	Bot		*is_bot_in_channel() const ; /// Added

	int		search_user_by_nickname(std::string);

	// from what i make out, these cannot be const 
	// bc fn array dictates this->*

	void	Forward_MOTD(int);
	void	Forward_MOTD_Slant_Relief(int);
	void	Forward_MOTD_Dot_Matrix(int);
	void	Forward_MOTD_Mahjong(int);
	void	Forward_MOTD_Doh(int);
	void	Forward_MOTD_la_nuit_mes_yeux(int);
	void	Forward_MOTD_voir_un_univers(int);

	void	clear_all();

};

std::ostream & operator << (std::ostream &, std::map<std::string, Channel*> &);
std::ostream & operator << (std::ostream &, std::map<int, User*> &);
std::ostream & operator << (std::ostream &, User &);

#endif
