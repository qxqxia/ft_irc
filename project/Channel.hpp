#pragma once
#ifndef __CHANNEL_HPP__
# define __CHANNEL_HPP__

# include "Freenode.hpp"

using	namespace std;

class	User;

class	Channel
{

private:

	map<int, User *>	m_users;
	map<int, User *>	m_chanops;
	map<int, User *>	m_voices;
	string			m_channelname;
	string			m_topic;
	string			m_mode;
	string			m_key;
	int			m_maximum_users;

	map<string, string>	m_banlist;


public:

	Channel(string);

	~Channel();


	map<string, string>	& get_banlist();


	map<int, User*>		& get_users();
	map<int, User*>		& get_chanops();
	map<int, User*>		& get_voices();


	string	get_topic() const;
	string	get_channelname() const;
	string	get_mode() const;
	string	get_key() const;


	string	get_list_of_users_in_channel();
	string	get_list_of_users_banned();


	int	get_user_number() const;
	int	get_maximum_users() const;


	bool	is_chanop(int sd);
	bool	is_voice(int sd);
	bool	is_banned(string nick);


	void	set_topic(string top);
	void	set_mode(string mod);
	void	set_key(string key);
	void	set_maximum_users(int max);


	void	add_user(int sd, User *u);
	void	add_chanop(int sd, User *u);
	void	add_voice(int sd, User *u);


	void	left_user_of_what_use(int sock_fd);
	int	search_user_by_nickname(string nick);

};


#endif



