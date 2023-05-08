#include "Freenode.hpp"

// /mode #channel|nickname [[+|-]modechars [parameters]]
//  This is a powerful command that gives channel 
//  operators control of a channel and the users on it

/*

Channel modes
------------------------
ModeChar        Effects on channels
~~~~~~~~        ~~~~~~~~~~~~~~~~~~~
b <person>        ban somebody, <person> in "nick!user@host" form
i                channel is invite-only
l <number>        channel is limited, <number> users allowed max
m                channel is moderated, (only chanops can talk)
n                external /MSGs to channel are not allowed
o <nickname>        makes <nickname> a channel operator
p                channel is private
s                channel is secret
t                topic limited, only chanops may change it
k <key>                set secret key for a channel

*/


/*

User modes
-------------------
ModeChar        Effects on nicknames
~~~~~~~~        ~~~~~~~~~~~~~~~~~~~~
i                makes you invisible to anybody that does
                not know the exact spelling of your nickname
o                IRC-operator status, can only be set
                by IRC-ops with OPER
s                receive server notices
v                gives a user a voice on a moderated channel

*/
 

void mode_o(Server *serv, Channel *channel, std::string mode, std::string buffer, int sd)
{
    int i = -1, j = 0;

    while (buffer[++i] && j < 3)
    {
        if (buffer[i] == ' ' || buffer[i] == '\t')
        {
            while (buffer[i] == ' ' || buffer[i] == '\t')
            {
                i++;
            }
            j++;
            i--;
        }
    }
    std::string name = buffer.substr(i, (buffer.find_first_of(SEP_CHARSET, i) - i));
    int user_socket_fd = channel->search_user_by_nickname(name);
    if (user_socket_fd == -1)
    {
        Broadcast(get_RPL_ERR(441, serv, FIND_USER(sd), name, channel->get_channelname()), sd);
        return ;
    }

    channel->left_user_of_what_use(user_socket_fd);

    if (mode[0] == '-')
    {
        channel->add_user(user_socket_fd, FIND_USER(user_socket_fd));
    }
    else
    {
        channel->add_chanop(user_socket_fd, FIND_USER(user_socket_fd));
    }

    std::string user_answer = user_output(FIND_USER(sd));

    if (channel->get_mode().find('a') != std::string::npos)
    {
        user_answer = anonymous_output();
    }
    user_answer += "MODE " + channel->get_channelname() + " " + mode + " " + name;
    Broadcast(user_answer, user_socket_fd);
}

void mode_v(Server *serv, Channel *channel, std::string mode, std::string buffer, int sd)
{

    int i = -1, j = 0;
    while (buffer[++i] && j < 3)
    {
        if (buffer[i] == ' ' || buffer[i] == '\t')
        {
            while (buffer[i] == ' ' || buffer[i] == '\t')
                i++;
            j++;
            i--;
        }
    }

    std::string name = buffer.substr(i, (buffer.find_first_of(SEP_CHARSET, i) - i));
    int user_socket_fd = channel->search_user_by_nickname(name);

    if (user_socket_fd == -1)
    {
        Broadcast(get_RPL_ERR(441, serv, FIND_USER(sd), name, channel->get_channelname()), sd);
        return ;
    }
    if (channel->is_chanop(user_socket_fd) == true)
    {
        return ;
    }

    channel->left_user_of_what_use(user_socket_fd);

    if (mode[0] == '-')
    {
        channel->add_user(user_socket_fd, FIND_USER(user_socket_fd));
    }
    else
    {
        channel->add_voice(user_socket_fd, FIND_USER(user_socket_fd));
    }

    std::string user_answer = user_output(FIND_USER(sd));

    if (channel->get_mode().find('a') != std::string::npos)
    {
        user_answer = anonymous_output();
    }

    user_answer += "MODE " + channel->get_channelname() + " " + mode + " " + name;
    Broadcast(user_answer, sd);

}

void mode_b(Server *serv, Channel *channel, std::string mode, std::string buffer, int sd)
{
    (void)  mode;


    int i = -1, j = 0;
    while (buffer[++i] && j < 3)
    {
        if (buffer[i] == ' ' || buffer[i] == '\t')
        {
            while (buffer[i] == ' ' || buffer[i] == '\t')
            {
                i++;
            }
            j++;
            i--;
        }
    }

    std::string name = buffer.substr(i, (buffer.find_first_of(SEP_CHARSET, i) - i));

    if (name.empty())
    {
        std::string banlist = channel->get_list_of_users_banned();
        if ( !banlist.empty())
        {
            Broadcast(get_RPL_ERR(367, serv, FIND_USER(sd), channel->get_channelname(), banlist), sd);
        }
        Broadcast(get_RPL_ERR(368, serv, FIND_USER(sd), channel->get_channelname(), ""), sd);
    }
	else
    {
        std::string nick = name.substr(0, name.find('!'));
        if (nick.length() > 1 || (nick.length() == 1 && nick[0] != '*'))
        {
            if (nick[0] == '*')
                name = & nick[1];
            else
                name = nick;
        }
        else
            return ;
        if (mode[0] == '-')
            channel->get_banlist().erase(name);
        else
            channel->get_banlist().insert(std::make_pair(name, nick));
	}
}

void mode_k(Server *serv, Channel *channel, std::string mode, std::string buffer, int sd)
{
    (void)  mode;
    (void)  serv;

    if (mode[0] == '-')
    {
        channel->set_key("");
	    return ;
    }

    int i = -1, j = 0;
    while (buffer[++i] && j < 3)
    {
        if (buffer[i] == ' ' || buffer[i] == '\t')
        {
            while (buffer[i] == ' ' || buffer[i] == '\t')
            {
                i++;
            }
            j++;
            i--;
        }
    }
    std::string key = buffer.substr(i, (buffer.find_first_of(SEP_CHARSET, i) - i));

    if (key == "x")
    {
        Broadcast(get_RPL_ERR(467, serv, FIND_USER(sd), channel->get_key(), ""), sd);
    }
    else
    {
        if (!key.empty())
            channel->set_key(key);
        else
        {
            std::string truc = user_output(FIND_USER(sd));
            Broadcast(truc + ": Wrong key!", sd);
        }
    }
}

void mode_l(Server *serv, Channel *channel, std::string mode, std::string buffer, int sd)
{
    (void)  mode;
    (void)  serv;

    if (mode[0] == '-')
    {
	    channel->set_maximum_users(-1);
	    return ;
    }
    int i = 0;
    for (int j = 0; buffer[i] && j < 3; i++)
    {
        if (buffer[i] == ' ' || buffer[i] == '\t')
        {
            while (buffer[i] == ' ' || buffer[i] == '\t')
                i++;
            j++;
            i--;
        }
    }
    std::string name = buffer.substr(i, buffer.find('\r') != std::string::npos ? buffer.length() - 2 - i : buffer.length() - 1 - i);
    int maxUser = std::strtoul(name.c_str(), NULL, 0);
    if (maxUser < 0)
    {
	    Broadcast("Max user key must be superior to 0.", sd);
	    return ;
    }
    channel->set_maximum_users(maxUser);
}

bool available_modes(char c, std::string available_modes)
{
    return (!(available_modes.find(c) == std::string::npos));
}

void channel_mode(Server *serv, Channel *channel, std::string mode, int sd, std::string buffer)
{
    std::string channel_mode = channel->get_mode();
    int i;
    typedef void (*modes)(Server *, Channel *, std::string, std::string, int);

    std::map<char, modes> handle_mode;

	handle_mode.insert(std::make_pair('o', &mode_o));
	handle_mode.insert(std::make_pair('v', &mode_v));
	handle_mode.insert(std::make_pair('b', &mode_b));
    handle_mode.insert(std::make_pair('k', &mode_k));
    handle_mode.insert(std::make_pair('l', &mode_l));

    if (mode[0] == '-')
    {
        std::string deleted_mode;


        i = 0;
        while (mode[++i])
        {
            if (available_modes(mode[i], CHANNEL_MODE) == false)
	        {
		        std::string stringMode(1, mode[i]);
                Broadcast(get_RPL_ERR(472, serv, FIND_USER(sd), stringMode, channel->get_channelname()), sd);
	        }
            else if (available_modes(mode[i], "ovbkl") == true)
	        {
		        handle_mode[mode[i]](serv, channel, mode, buffer, sd);
	    	    if (mode[i] == 'k' || mode[i] == 'l')
			        deleted_mode += mode[i];
	        }
	        else if (channel_mode.find(mode[i]) != std::string::npos)
            {
                deleted_mode += mode[i];
                channel_mode.erase(channel_mode.find(mode[i]), 1);
            }
        }
        channel->set_mode(channel_mode);
        std::string user_answer = user_output(FIND_USER(sd));
        if (channel->get_mode().find("a") != std::string::npos)
            user_answer = anonymous_output();
        if (!deleted_mode.empty())
            user_answer += "MODE " + channel->get_channelname() + " -" + deleted_mode;
        if (user_answer.find("MODE") != std::string::npos)
            send_everyone_in_channel(user_answer, channel);
    }
    else
    {
        std::string added_mode;


        i = mode[0] != '+' ? -1 : 0;
        while (mode[++i])
        {
            if (available_modes(mode[i], CHANNEL_MODE) == false)
            {
                std::string stringMode(1, mode[i]);
                Broadcast(get_RPL_ERR(472, serv, FIND_USER(sd), stringMode, channel->get_channelname()), sd);
            }
            else if (available_modes(mode[i], "ovbkl") == true)
            {
                handle_mode[mode[i]](serv, channel, mode, buffer, sd);
                if ((mode[i] == 'k' && channel->get_key() != "") || mode[i] == 'l')
                    added_mode += mode[i];
            }
            else if (channel_mode.find(mode[i]) == std::string::npos)
                added_mode += mode[i];
        }
        channel->set_mode(channel_mode + added_mode);
        std::string user_answer = user_output(FIND_USER(sd));
        if (channel->get_mode().find("a") != std::string::npos)
            user_answer = anonymous_output();
        if (!added_mode.empty())
            user_answer += "MODE " + channel->get_channelname() + " +" + added_mode;
        if (user_answer.find("MODE") != std::string::npos)
            send_everyone_in_channel(user_answer, channel);
    }
}

void userMode(Server *serv, User *user, std::string mode, int sd)
{
    std::string userMode = user->get_mode();
    int i;
    int user_socket_fd = serv->search_user_by_nickname(user->get_nickname());

    if (mode[0] == '-')
    {
        std::string deleted_mode;

        i = 0;
        while (mode[++i])
        {
            if (available_modes(mode[i], USER_MODE) == false)
            {
                Broadcast(get_RPL_ERR(501, serv, FIND_USER(sd), "", ""), sd);
            }
            else if (userMode.find(mode[i]) != std::string::npos)
            {
                if (mode[i] == 'o' && user->get_nickname() != FIND_USER(sd)->get_nickname())
                {
                    Broadcast(get_RPL_ERR(481, serv, FIND_USER(sd), "", ""), sd);
                }
                else if (mode[i] == 'r' && ((FIND_USER(sd)->get_mode().find('r') != std::string::npos) || (FIND_USER(sd)->get_mode().find('o') == std::string::npos )))
                {
                    Broadcast(get_RPL_ERR(481, serv, FIND_USER(sd), "", ""), sd);
                }
                else
                {
                    deleted_mode += mode[i];
                    userMode.erase(userMode.find(mode[i]), 1);
                }
            }
        }
        user->set_mode(userMode);
        std::string user_answer = user_output(FIND_USER(user_socket_fd));
        if (!deleted_mode.empty())
            user_answer += "MODE " + user->get_nickname() + " -" + deleted_mode;
        if (user_answer.find("MODE") != std::string::npos)
            Broadcast(user_answer, user_socket_fd);
    }
    else
    {
        std::string added_mode;


        i = mode[0] != '+' ? -1 : 0;        
        while (mode[++i])
        {
            if (available_modes(mode[i], USER_MODE) == false)
            {
                Broadcast(get_RPL_ERR(501, serv, FIND_USER(sd), "", ""), sd);
            }
            else if (userMode.find(mode[i]) == std::string::npos)
            {
                if (mode[i] == 'o' && FIND_USER(sd)->get_mode().find('o') == std::string::npos)
                {
                    Broadcast(get_RPL_ERR(481, serv, FIND_USER(sd), "", ""), sd);
                }
                else if (mode[i] == 'r' && user->get_nickname() == FIND_USER(sd)->get_nickname())
                {
                    added_mode += mode[i];
                }
                else if (mode[i] == 'r' && ((FIND_USER(sd)->get_mode().find('r') != std::string::npos) || (FIND_USER(sd)->get_mode().find('o') == std::string::npos)))
                {
                    Broadcast(get_RPL_ERR(481, serv, FIND_USER(sd), "", ""), sd);
                }
                else
                {
                    added_mode += mode[i];
                }
            }
        }
        user->set_mode(userMode + added_mode);
        std::string user_answer = user_output(FIND_USER(user_socket_fd));
        if (!added_mode.empty())
        {
            user_answer += "MODE " + user->get_nickname() + " +" + added_mode;
        }
        if (user_answer.find("MODE") != std::string::npos)
        {
            Broadcast(user_answer, user_socket_fd);
        }
    }
}

void mode(Server *serv, std::string buffer, int sd)
{
    std::string buf(buffer);
    int j = 0;
    size_t i;
    std::string msgtarget = "";
    if ((i = buf.find_first_not_of(SEP_CHARSET, 5)) != std::string::npos)
        msgtarget = buf.substr(i, ((j = buf.find_first_of(SEP_CHARSET, i)) - i));
    if (msgtarget.empty())
    {
        Broadcast(get_RPL_ERR(461, serv, FIND_USER(sd), "MODE", ""), sd);
        return ;
    }
    std::string idOfChannel = "#&+";
    j = buf.find_first_not_of(SEP_CHARSET, j);
    std::string mode = "";
    if (buf.find_first_of(SEP_CHARSET, j) != std::string::npos)
        mode = buf.substr(j, (buf.find_first_of(SEP_CHARSET, j) - j));
    if (!msgtarget.empty() && idOfChannel.find(msgtarget[0]) != std::string::npos)
    {
        if (serv->get_channels().find(msgtarget) == serv->get_channels().end())
            Broadcast(get_RPL_ERR(403, serv, FIND_USER(sd), msgtarget, ""), sd);
        else if (FIND_USER(sd)->get_mode().find('r') != std::string::npos)
            Broadcast(get_RPL_ERR(484, serv, FIND_USER(sd), "", ""), sd);
        else if (FIND_CHANNEL(msgtarget)->get_chanops().find(sd) == FIND_CHANNEL(msgtarget)->get_chanops().end())
            Broadcast(get_RPL_ERR(482, serv, FIND_USER(sd), msgtarget, ""), sd);
        else
            channel_mode(serv, FIND_CHANNEL(msgtarget), mode, sd, buffer);
    }
    else
    {
        int user_socket_fd;
        if ((user_socket_fd = serv->search_user_by_nickname(msgtarget)) == -1)
            Broadcast(get_RPL_ERR(401, serv, FIND_USER(sd), msgtarget, ""), sd);
        else if (mode.empty() && !FIND_USER(user_socket_fd)->get_mode().empty())
            Broadcast(get_RPL_ERR(221, serv, FIND_USER(user_socket_fd), '+' + FIND_USER(user_socket_fd)->get_mode(), ""), sd);
        else if (mode.empty() && FIND_USER(user_socket_fd)->get_mode().empty())
            Broadcast("This user don't have any modes!", sd);
        else
            userMode(serv, FIND_USER(user_socket_fd), mode, sd);
    }
    
}
