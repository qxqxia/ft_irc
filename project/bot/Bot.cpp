#include "Freenode.hpp"

Bot::Bot() : m_name("Chat")
{
	this->m_command_handler.insert(
        std::pair<std::string, command>("wakeup", & connect)
    );

    this->m_command_handler.insert(
        std::pair<std::string, command>("help", & help)
    );

    this->m_command_handler.insert(
        std::pair<std::string, command>("date", & tell_date)
    );

    this->m_command_handler.insert(
        std::pair<std::string, command>("time", & tell_time)
    );

    this->m_command_handler.insert(
        std::pair<std::string, command>("sleep", & quit)
    );
}

Bot::~Bot()
{
    this->m_command_handler.clear();
}

std::string Bot::get_name() const
{
    return this->m_name;
}


void Bot::find_command(Server * serv, Channel *chan, int socket_fd, std::string cmd)
{
    if (m_command_handler.find(cmd) != m_command_handler.end())
    {
        this->m_command_handler[cmd](serv, chan, socket_fd);
    }
    else
    {
        Broadcast(get_RPL_ERR(669, serv, FIND_USER(socket_fd), cmd, ""), socket_fd);
    }
}

void connect(Server *serv, Channel *chan, int socket_fd)
{
    if (chan->is_chanop(socket_fd))
    {
        if (chan && chan->get_bot() == false)
        {
            chan->set_bot();
            send_everyone_in_channel(":" + serv->get_bot()->get_name() + " JOIN " + chan->get_channelname(), chan);
        }
        else
            Broadcast(":" + serv->get_bot()->get_name() + " PRIVMSG " + chan->get_channelname() + " :I am already awake.", socket_fd);
    }
    else
        Broadcast(get_RPL_ERR(482, serv, FIND_USER(socket_fd), chan->get_channelname(), ""), socket_fd);
}

void quit(Server *serv, Channel *chan, int socket_fd)
{
    if (chan->is_chanop(socket_fd))
    {
        if (chan && chan->get_bot() == true)
        {
            chan->set_bot();
            send_everyone_in_channel(":" + serv->get_bot()->get_name() + " PART " + chan->get_channelname(), chan);
        }
        else
            Broadcast(":" + serv->get_bot()->get_name() + " PRIVMSG " + chan->get_channelname() + " :I am sleeping right now, wake me up to play with me.", socket_fd);
    }
    else
        Broadcast(get_RPL_ERR(482, serv, FIND_USER(socket_fd), chan->get_channelname(), ""), socket_fd);
}



void tell_time(Server *serv, Channel *chan, int socket_fd)
{
    if (chan && chan->get_bot() == true)
    {
        std::time_t t = std::time(0);
        std::tm* now = std::localtime(&t);
        std::stringstream sshour;
        sshour << now->tm_hour;
        std::stringstream ssmin;
        ssmin << now->tm_min;
        Broadcast(":" + serv->get_bot()->get_name() + " PRIVMSG " + chan->get_channelname() + " :It is " + sshour.str() + ":" + ssmin.str() + ".", socket_fd);
    }
    else
        Broadcast(":" + serv->get_bot()->get_name() + " PRIVMSG " + chan->get_channelname() + " :I am sleeping right now, wake me up to play with me.", socket_fd);

}

void tell_date(Server *serv, Channel *chan, int socket_fd)
{
    if (chan && chan->get_bot() == true)
    {
        std::string months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
        std::time_t t = std::time(0);
        std::tm* now = std::localtime(&t);
        std::stringstream ssyear;
        ssyear << now->tm_year + 1900;
        std::stringstream ssmday;
        if (now->tm_mday == 1)
            ssmday << now->tm_mday << "st";
        else if (now->tm_mday == 2)
            ssmday << now->tm_mday << "nd";
        else if (now->tm_mday == 3)
            ssmday << now->tm_mday << "rd";
        else
            ssmday << now->tm_mday << "th";

        Broadcast(":" + serv->get_bot()->get_name() + " PRIVMSG " + chan->get_channelname() + " :Today we are on " + months[now->tm_mon] + " " + ssmday.str() + " of " + ssyear.str() + ".", socket_fd);
    }
    else
    {
        Broadcast(":" + serv->get_bot()->get_name() + " PRIVMSG " + chan->get_channelname() + " :I am sleeping right now, wake me up to play with me.", socket_fd);
    }
}

void help(Server *serv, Channel *chan, int socket_fd)
{
    std::string line = "!wakeup  - Connect the bot to the channel.";
    Broadcast(":" + serv->get_bot()->get_name() + " PRIVMSG " + chan->get_channelname() + " :" + line, socket_fd);

    line = "!date    - Give today's date.";
    Broadcast(":" + serv->get_bot()->get_name() + " PRIVMSG " + chan->get_channelname() + " :" + line, socket_fd);

    line = "!time    - Give the current local time.";
    Broadcast(":" + serv->get_bot()->get_name() + " PRIVMSG " + chan->get_channelname() + " :" + line, socket_fd);

    line = "!sleep   - Disconnect the bot.";
    Broadcast(":" + serv->get_bot()->get_name() + " PRIVMSG " + chan->get_channelname() + " :" + line, socket_fd);
}


Channel *userInChanBot(Server *serv, User *user)
{
    for (std::set<std::string>::iterator it = user->get_channels().begin(); it != user->get_channels().end(); it++)
        if (FIND_CHANNEL(*it)->get_bot() == true)
            return FIND_CHANNEL(*it);
    return NULL;
}

