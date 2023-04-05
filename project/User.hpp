#pragma once
#ifndef __USER_HPP__
# define __USER_HPP__

# include ""
using	namespace std;

class	User
{

private:
	string	m_nickname;
	string	m_username;
	string	m_hostname;
	string	m_realname;
	string	m_mode;

	std::set<string>	m_channels;

public:
	User(
		string	nick,
		string	user,
		string	hostname,
		string	realname
	);

	~User();

	string	get_nickname() const;
	string	get_username() const;
	string	get_hostname() const;
	string	get_realname() const;

	std::set<string>	& get_channels();

	string	get_mode() const;
	size_t		get_channels_size() const;

	void	set_nick(string nick );
	void	set_mode(string mode );

	void	add_channel(string channel_name );

};


#endif


