#include "Freenode.hpp"

Channel::Channel(std::string channel_name) : 
    m_channelname(channel_name),
    m_bot_in_channel(false) /* added for Bot*/
{
    // ... 
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
    return (this->m_users.size() + this->m_chanops.size() + this->m_voices.size());
}


std::string Channel::get_channelname() const
{
    return this->m_channelname;
}

std::map<int, User*> & Channel::get_users()
{
    return this->m_users;
}

std::map<int, User*> & Channel::get_chanops()
{
    return this->m_chanops;
}

std::map<int, User*> & Channel::get_voices()
{
    return this->m_voices;
}

std::map<std::string, std::string> & Channel::get_banlist()
{
    return this->m_banlist;
}

std::string Channel::get_mode() const
{
    return this->m_mode;
}


int Channel::get_maximum_users() const
{
    return (this->m_maximum_users);
}



void Channel::set_mode(std::string mode)
{
    this->m_mode = mode;
}


void Channel::set_maximum_users(int maxUser)
{
    this->m_maximum_users = maxUser;
}

void Channel::add_user(int sd, User *user)
{
    this->m_users.insert(std::make_pair(sd, user));
}

void Channel::add_chanop(int sd, User *user)
{
    this->m_chanops.insert(std::make_pair(sd, user));
}

void Channel::add_voice(int sd, User *user)
{
    this->m_voices.insert(std::make_pair(sd, user));
}

void Channel::left_user_of_what_use(int sd)
{
    std::map<int, User*>::iterator it;

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

int Channel::search_user_by_nickname(std::string nickname)
{
    std::map<int, User*>::iterator  it;

    it = this->m_users.begin();
    while (it != this->m_users.end())
    {
        if (nickname == it->second->get_nickname())
            return (it->first);
        ++it;
    }

    it = this->m_chanops.begin();
    while (it != this->m_chanops.end())
    {
        if (nickname == it->second->get_nickname())
            return (it->first);
        ++it;
    }

    it = this->m_voices.begin();
    while (it != this->m_voices.end())
    {
        if (nickname == it->second->get_nickname())
            return (it->first);
        ++it;
    }
    return (-1);
}

std::string Channel::get_list_of_users_in_channel()
{
    std::map<int, User *>::iterator     it;
    std::string     output;

    it = this->m_chanops.begin();
    while (it != this->m_chanops.end())
    {
        if (!output.empty())
            output += " ";
        output += "@";
        output += it->second->get_nickname();
        ++it;
    }

    it = this->m_voices.begin();
    while (it != this->m_voices.end())
    {
        if (!output.empty())
            output += " ";
        output += "+";
        output += it->second->get_nickname();
        ++it;
    }

    it = this->m_users.begin();
    while (it != this->m_users.end())
    {
        if (!output.empty())
            output += " ";
        output += it->second->get_nickname();
        ++it;
    }
    return (output);
}

std::string Channel::get_list_of_users_banned()
{
    std::map<std::string, std::string>::iterator    it;
    std::string     output;


    it = this->m_banlist.begin();
    while (it != this->m_banlist.end())
    {
        if (!output.empty())
            output += ",";
        output += it->second;
        ++it;
    }
    return (output);
}

bool    Channel::is_chanop(int sd)
{
    if (this->m_chanops.find(sd) != this->m_chanops.end())
        return (true);
    return (false);
}

bool    Channel::is_voice(int sd)
{
    if (this->m_voices.find(sd) != this->m_voices.end())
        return (true);
    return (false);
}

bool Channel::is_banned(std::string nickname)
{
    if (this->m_banlist.find(nickname) != this->m_banlist.end())
        return (true);
    return (false);
}


/// Added

bool    Channel::is_bot_in_channel() const
{
    return (this->m_bot_in_channel);
}

void    Channel::set_bot()
{
    this->m_bot_in_channel = !(this->m_bot_in_channel);
}