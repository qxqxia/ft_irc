#include "Freenode.hpp"

User::User(
	string	nickname,
	string	username,
	string	hostname,
	string	realname
)
	: m_nickname( nickname ),
	m_username( username ),
	m_hostname( hostname ),
	m_realname( realname ) {
}


User::~User() { this->m_channels.clear(); }


// Getters of names

string	User::get_nickname() const
{
	return (this->m_nickname);
}


string	User::get_username() const
{ 
	return (this->m_username);
}


string	User::get_hostname() const
{
	return (this->m_hostname);
}


string	User::get_realname() const
{
	return (this->m_realname);
}


// Channel

set<string> & User::get_channels()
{
	return (this->m_channels);
}



size_t	User::get_channels_size() const
{
	return (this->m_channels.size());
}


void	User::add_channel(string channel_name)
{
	if (this->m_channels.find(channel_name) != this->m_channels.end())
		return ;
	this->m_channels.insert(channel_name);
}


// Mode

string	User::get_mode() const
{
	return (this->m_mode);
}


void	User::set_mode(string mode)
{
	this->m_mode = mode;
}



void	User::set_nick(string new_nick)
{
	this->m_nickname = new_nick;
}


