#include "Freenode.hpp"

void forward_message(std::string message, int sd)
{
	message += "\r\n";
	if (send(sd, message.c_str(), message.length(), 0) < 0)
		throw std::runtime_error("Error sending message.");
}

void send_everyone_in_channel(std::string message, Channel *chan)
{
    for (std::map<int, User *>::iterator it = chan->get_users().begin(); it != chan->get_users().end(); it++)
        forward_message(message, it->first);
    for (std::map<int, User *>::iterator it = chan->get_chanops().begin(); it != chan->get_chanops().end(); it++)
        forward_message(message, it->first);
    for (std::map<int, User *>::iterator it = chan->get_voices().begin(); it != chan->get_voices().end(); it++)
        forward_message(message, it->first);
}

void send_everyone_in_channel_except_user(std::string message, Channel *chan, int sd)
{
    for (std::map<int, User *>::iterator it = chan->get_users().begin(); it != chan->get_users().end(); it++)
        if (sd != it->first)
            forward_message(message, it->first);
    for (std::map<int, User *>::iterator it = chan->get_chanops().begin(); it != chan->get_chanops().end(); it++)
        if (sd != it->first)
            forward_message(message, it->first);
    for (std::map<int, User *>::iterator it = chan->get_voices().begin(); it != chan->get_voices().end(); it++)
        if (sd != it->first)
            forward_message(message, it->first);
}

void send_everyone(std::string message, std::map<int, User *> users)
{
    for (std::map<int, User *>::iterator it = users.begin(); it != users.end(); it++)
        forward_message(message, it->first);
}

std::string user_output(User *user)
{
    return (":" + user->get_nickname() + "!" + user->get_username() + "@" + user->get_hostname() + " ");
}

std::string anonymous_output()
{
    return (":anonymous!anonymous@anonymous ");
}

time_t	get_time(void)
{
	return time(0);
}