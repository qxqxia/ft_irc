#include "Freenode.hpp"

User::User(
	std::string nickname,
	std::string username,
	std::string hostname,
	std::string realname
) :
	m_nickname(nickname),
	m_username(username),
	m_hostname(hostname),
	m_realname(realname) {
		std::cout
		<< GREEN "(dbg)(Create user):" RESET nl
		<< "nck :: " << get_nickname() << nl
		<< "usr :: " << get_username() << nl
		<< "hst :: " << get_hostname() << nl
		<< "rln :: " << get_realname() << nl2
		;
}

User::~User()
{
	this->m_channels.clear();
}

std::string	User::get_nickname() const
{
	return (this->m_nickname);
}

std::string	User::get_username() const
{ 
	return (this->m_username);
}

std::string	User::get_hostname() const
{
	return (this->m_hostname);
}

std::string	User::get_realname() const
{
	return (this->m_realname);
}

std::set<std::string> & User::get_channels()
{
	return (this->m_channels);
}

size_t	User::get_channels_size() const
{
	return (this->m_channels.size());
}

std::string	User::get_mode() const
{
	return (this->m_mode);
}

void	User::set_mode(std::string mode)
{
	this->m_mode = mode;
}

void	User::add_channel(std::string channel_name)
{
	if (this->m_channels.find(channel_name) == this->m_channels.end())
		this->m_channels.insert(channel_name);
}

void	User::set_nick(std::string new_nick)
{
	std::cout
		<< GREEN "(dbg)(Modify nick):" RESET nl
		<< "(from): " << this->m_nickname << nl
		<< "( to ): " << new_nick << nl2;
		;
	this->m_nickname = new_nick;
}

