#include "Freenode.hpp"

void Broadcast(std::string message, int sd)
{
	message += "\r\n";
	if (send(sd, message.c_str(), message.length(), 0) < 0)
	{
        throw std::runtime_error("Error sending message.");
    }
}

void send_everyone_in_channel(std::string message, Channel *chan)
{
    std::map<int, User *>::iterator     it;


    it = chan->get_users().begin();
    while (it != chan->get_users().end())    
    {   
        Broadcast(message, it->first);
        ++it;
    }


    it = chan->get_chanops().begin();
    while (it != chan->get_chanops().end())
    {
        Broadcast(message, it->first);
        ++it;
    }


    it = chan->get_voices().begin();
    while (it != chan->get_voices().end())
    {
        Broadcast(message, it->first);
        ++it;
    }
}

void send_everyone_in_channel_except_user(std::string message, Channel *chan, int sd)
{
    std::map<int, User *>::iterator     it;



    it = chan->get_users().begin();
    while (it != chan->get_users().end())    
    {
        if (sd != it->first)
            Broadcast(message, it->first);
        ++it;
    }

    
    it = chan->get_chanops().begin();
    while (it != chan->get_chanops().end())
    {
        if (sd != it->first)
            Broadcast(message, it->first);
        ++it;
    }


    it = chan->get_voices().begin();
    while (it != chan->get_voices().end())
    {
        if (sd != it->first)
            Broadcast(message, it->first);
        ++it;
    }
}

void send_everyone(std::string message, std::map<int, User *> users)
{

    std::map<int, User *>::iterator     it = users.begin();
    
    while (it != users.end())
    {
        Broadcast(message, it->first);
        ++it;
    }
}

std::string user_output(User *user)
{
    return (":" + user->get_nickname() + "!" + user->get_username() + "@" + user->get_hostname() + " ");
}

std::string anonymous_output()
{
    return (":anonymous!anonymous@anonymous ");
}

time_t	get_time()
{
	return time(NULL);
}