#pragma once
#ifndef __USER_HPP__
# define __USER_HPP__

# include "Freenode.hpp"
// using	namespace std;

class	User
{

private:
	std::string	m_nickname;
	std::string	m_username;
	std::string	m_hostname;
	std::string	m_realname;
	std::string	m_mode;

	std::set<std::string>	m_channels;

public:
	User(
		std::string	nick,
		std::string	user,
		std::string	hostname,
		std::string	realname
	);

	~User();

	std::string	get_nickname() const;
	std::string	get_username() const;
	std::string	get_hostname() const;
	std::string	get_realname() const;

	std::set<std::string>	& get_channels();

	std::string	get_mode() const;
	size_t		get_channels_size() const;

	void	set_nick(std::string nick);
	void	set_mode(std::string mode);
	void	add_channel(std::string channelname);

};


#endif


