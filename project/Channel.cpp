#include "Freenode.hpp"

Channel::Channel(
	string channel_name
) : 
	m_channelname(channel_name),
	m_topic("") {
}


Channel::~Channel()
{
	this->m_users.clear();
	this->m_chanops.clear();
	this->m_voices.clear();
	this->m_banlist.clear();
}


int Channel::get_user_number() const
{
	return (
		this->m_users.size()
		+ this->m_chanops.size()
		+ this->m_voices.size()
	);
}


string Channel::get_topic() const
{
	return (this->m_topic);
}


string Channel::get_channelname() const
{
	return (this->m_channelname);
}


map<int, User*> & Channel::get_users()
{
	return (this->m_users);
}


map<int, User*> & Channel::get_chanops()
{
	return (this->m_chanops);
}


map<int, User*> & Channel::get_voices()
{
	return (this->m_voices);
}


map<string, string> & Channel::get_banlist()
{
	return (this->m_banlist);
}


string Channel::get_mode() const
{
	return (this->m_mode);
}


string Channel::get_key() const
{
	return (this->m_key);
}


int Channel::get_maximum_users() const
{
	return (this->m_maximum_users);
}


void Channel::set_topic(string topic)
{
	this->m_topic = topic;
}

void Channel::set_mode(string mode)
{
	this->m_mode = mode;
}


void Channel::set_key(string key)
{
	this->m_key = key;
}


void Channel::set_maximum_users(int maxUser)
{
	this->m_maximum_users = maxUser;
}


void Channel::add_user(int sd, User *user)
{
	this->m_users.insert(make_pair(sd, user));
}


void Channel::add_chanop(int sd, User *user)
{
	this->m_chanops.insert(make_pair(sd, user));
}


void Channel::add_voice(int sd, User *user)
{
	this->m_voices.insert(make_pair(sd, user));
}


void Channel::left_user_of_what_use(int sd)
{
	map<int, User*>::iterator   it;

	if ((it = this->m_users.find(sd)) != this->m_users.end())
	{
		this->m_users.erase(it);
	}
	else if ((it = this->m_chanops.find(sd)) != this->m_chanops.end())
	{
		this->m_chanops.erase(it);
	}
	else if ((it = this->m_voices.find(sd)) != this->m_voices.end())
	{
		this->m_voices.erase(it);
	}
}


int Channel::search_user_by_nickname(string nickname)
{
	map<int, User*>::iterator   it;

	it = this->m_users.begin();
	while (it != this->m_users.end())
	{
		if (nickname == it->second->get_nickname())
			return (it->first);
		++it;
	}

	it = this->m_chanops.begin();
	while  (it != this->m_chanops.end())
	{
		if (nickname == it->second->get_nickname())
			return (it->first);
		++it;
	}

	it = this->m_voices.begin();
	while (it != this->m_voices.end())
	{
		if (nickname == it->second->get_nickname())
			return it->first;
		++it;
	}
	return (-1);
}

string Channel::get_list_of_users_in_channel()
{
	map<int, User *>::iterator  it;
	string                      res;


	it = this->m_chanops.begin();
	while (it != this->m_chanops.end())
	{
		if (!res.empty())
			res += " ";
		res += "@";
		res += it->second->get_nickname();
		++it;
	}

	it = this->m_voices.begin();
	while (it != this->m_voices.end())
	{
		if (!res.empty())
			res += " ";
		res += "+";
		res += it->second->get_nickname();
		++it;
	}

	it = this->m_users.begin();
	while (it != this->m_users.end())
	{
		if (!res.empty())
			res += " ";
		res += it->second->get_nickname();
		++it;
	}
	return (res);
}


string Channel::get_list_of_users_banned()
{
	map<string, string>::iterator   it;
	string                          res;

	it = this->m_banlist.begin();
	while (it != this->m_banlist.end())
	{
		if (!res.empty())
			res += ",";
		res += it->second;
		++it;
	}
	return (res);
}


bool    Channel::is_chanop(int sd)
{
	if (this->m_chanops.find(sd) == this->m_chanops.end())
		return (false);
	return (true);
}


bool    Channel::is_voice(int sd)
{
	if (this->m_voices.find(sd) == this->m_voices.end())
		return (false);
	return (true);
}


bool	Channel::is_banned(string nickname)
{
	if (this->m_banlist.find(nickname) == this->m_banlist.end())
		return (false);
	return (true);
}

