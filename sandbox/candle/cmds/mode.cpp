#include "Freenode.hpp"

/*** DEFINITION ***/
// The /mode command changes the mode of the specified Channel or Nick/User

/*** CATEGORY ***/
/*
    Channel modes 
        - can only be changed by a channel operator
    User/Nick modes 
        - can only be changed for your own nick
*/

/*** CATEGORY ***/
/*
    /mode #<channel|nickname> [[+|-]modechars [parameters]]
*/

/*** CHANNEL modes ***/
/*
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
k <key>                std::set secret key for a channel


a               used in mode o & mode v
[?] chanmodes/chanadmin	Channel Admin
    - A level above chanops with only one special extra privileges:
        - people below chanadmin (so +o) cannot KICK +a people
*/


/*** USER Modes ***/
/*
ModeChar        Effects on nicknames
~~~~~~~~        ~~~~~~~~~~~~~~~~~~~~
i                makes you invisible to anybody that does
                not know the exact spelling of your nickname
o                IRC-operator status, can only be std::set
                by IRC-ops with OPER
r
*/
 

void mode_o(Server *serv, Channel *channel, std::string mode, std::string buffer, int sd)
{
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
    std::string name = buffer.substr(i, (buffer.find_first_of(SEP_CHARSET, i) - i));
    int user_socket_fd = channel->search_user_by_nickname(name);
    if (user_socket_fd == -1)
    {
        forward_message(forward_RPL(441, serv, FIND_USER(sd), name, channel->get_channelname()), sd);
        return ;
    }
    channel->left_user_of_what_use(user_socket_fd);
    if (mode[0] == '-')
        channel->add_user(user_socket_fd, FIND_USER(user_socket_fd));
    else
        channel->add_chanop(user_socket_fd, FIND_USER(user_socket_fd));
    std::string user_feedback = user_output(FIND_USER(sd));
    if (channel->get_mode().find('a') != std::string::npos)
        user_feedback = anonymous_output();
    user_feedback += "MODE " + channel->get_channelname() + " " + mode + " " + name;
    forward_message(user_feedback, user_socket_fd);
}

void mode_v(Server *serv, Channel *channel, std::string mode, std::string buffer, int sd)
{
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
    std::string name = buffer.substr(i, (buffer.find_first_of(SEP_CHARSET, i) - i));
    int user_socket_fd = channel->search_user_by_nickname(name);
    if (user_socket_fd == -1)
    {
        forward_message(forward_RPL(441, serv, FIND_USER(sd), name, channel->get_channelname()), sd);
        return ;
    }
    if (channel->is_chanop(user_socket_fd) == true)
        return ;
    channel->left_user_of_what_use(user_socket_fd);
    if (mode[0] == '-')
        channel->add_user(user_socket_fd, FIND_USER(user_socket_fd));
    else
        channel->add_voice(user_socket_fd, FIND_USER(user_socket_fd));
    std::string user_feedback = user_output(FIND_USER(sd));
    if (channel->get_mode().find('a') != std::string::npos)
        user_feedback = anonymous_output();
    user_feedback += "MODE " + channel->get_channelname() + " " + mode + " " + name;
    forward_message(user_feedback, sd);
}

void mode_b(Server *serv, Channel *channel, std::string mode, std::string buffer, int sd)
{
    std::string      name;
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
    name = buffer.substr(i, (buffer.find_first_of(SEP_CHARSET, i) - i));
    if (name.empty())
    {
        std::string banlist = channel->get_list_of_users_banned();
        if (!banlist.empty())
            forward_message(forward_RPL(367, serv, FIND_USER(sd), channel->get_channelname(), banlist), sd);
        forward_message(forward_RPL(368, serv, FIND_USER(sd), channel->get_channelname(), ""), sd);
    }
	else
    {
        std::string nick = name.substr(0, name.find('!'));
        if (nick.length() > 1 || (nick.length() == 1 && nick[0] != '*'))
        {
            if (nick[0] == '*')
                name = &nick[1];
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
    (void) mode;
}

void mode_k(Server *serv, Channel *channel, std::string mode, std::string buffer, int sd)
{
    if (mode[0] == '-')
    {
        channel->set_key("");
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
    std::string key = buffer.substr(i, (buffer.find_first_of(SEP_CHARSET, i) - i));
    if (key == "x")
    {
        forward_message(forward_RPL(467, serv, FIND_USER(sd), channel->get_key(), ""), sd);
    }
    else
    {
        if (!key.empty())
            channel->set_key(key);
        else
        {
            std::string truc = user_output(FIND_USER(sd));
            forward_message(truc + ": Wrong key!", sd);
        }
    }
    (void) mode;
    (void) serv;
}

void mode_l(Server *serv, Channel *channel, std::string mode, std::string buffer, int sd)
{
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
    // std::string name = buffer.substr(i, buffer.find('\r') != std::string::npos ? buffer.length() - 2 - i : buffer.length() - 1 - i);
    std::string  name;
    int     ending = buffer.length() - 1;
    if (buffer.find('\r') != std::string::npos)
        --ending;
    name = buffer.substr(i, ending - i);

    int maxUser = strtoul(name.c_str(), NULL, 0);
    if (maxUser < 0)
    {
	    // forward_message("Max user key must be superior to 0.", sd);
        forward_message("Invalid user key.", sd);
	    return ;
    }
    channel->set_maximum_users(maxUser);

    (void) mode;
    (void) serv;
}

bool available_mode(char c, std::string available_mode)
{
    return (!(available_mode.find(c) == std::string::npos));
}

void channel_mode(Server *serv, Channel *channel, std::string mode, int sd, std::string buffer)
{
    std::string  channel_mode = channel->get_mode();
    int i;
    typedef void (*modes)(Server *, Channel *, std::string, std::string, int);

    std::map<char, modes>    mode_handler;

	mode_handler.insert(std::make_pair('o', &mode_o));
	mode_handler.insert(std::make_pair('v', &mode_v));
	mode_handler.insert(std::make_pair('b', &mode_b));
    mode_handler.insert(std::make_pair('k', &mode_k));
    mode_handler.insert(std::make_pair('l', &mode_l));

    if (mode[0] == '-')
    {
        std::string      deleted_mode;
        for (i = 1; mode[i]; i++)
        {
            if (available_mode(mode[i], CHANNEL_MODE) == false)
	        {
		        std::string stringMode(1, mode[i]);
                forward_message(forward_RPL(472, serv, FIND_USER(sd), stringMode, channel->get_channelname()), sd);
	        }
            else if (available_mode(mode[i], "ovbkl") == true)
	        {
		        mode_handler[mode[i]](serv, channel, mode, buffer, sd);
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
        std::string user_feedback = user_output(FIND_USER(sd));
        if (channel->get_mode().find("a") != std::string::npos)
            user_feedback = anonymous_output();
        if (!deleted_mode.empty())
            user_feedback += "MODE " + channel->get_channelname() + " -" + deleted_mode;
        if (user_feedback.find("MODE") != std::string::npos)
            send_everyone_in_channel(user_feedback, channel);
    }
    else
    {
        std::string added_mode;
        for ((mode[0] != '+') ? i = 0 : i = 1; mode[i]; i++)
        {
            if (available_mode(mode[i], CHANNEL_MODE) == false)
            {
                std::string stringMode(1, mode[i]);
                forward_message(forward_RPL(472, serv, FIND_USER(sd), stringMode, channel->get_channelname()), sd);
            }
            else if (available_mode(mode[i], "ovbkl") == true)
            {
                mode_handler[mode[i]](serv, channel, mode, buffer, sd);
                if ((mode[i] == 'k' && channel->get_key() != "") || mode[i] == 'l')
                    added_mode += mode[i];
            }
            else if (channel_mode.find(mode[i]) == std::string::npos)
                added_mode += mode[i];
        }
        channel->set_mode(channel_mode + added_mode);
        std::string user_feedback = user_output(FIND_USER(sd));
        if (channel->get_mode().find("a") != std::string::npos)
            user_feedback = anonymous_output();
        if (!added_mode.empty())
            user_feedback += "MODE " + channel->get_channelname() + " +" + added_mode;
        if (user_feedback.find("MODE") != std::string::npos)
            send_everyone_in_channel(user_feedback, channel);
    }
}

void user_mode(Server *serv, User *user, std::string mode, int sd)
{
    std::string user_mode = user->get_mode();
    int i;
    int user_socket_fd = serv->search_user_by_nickname(user->get_nickname());

    if (mode[0] == '-')
    {
        std::string deleted_mode;
        for (i = 1; mode[i]; i++)
        {
            if (available_mode(mode[i], USER_MODE) == false)
            {
                forward_message(forward_RPL(501, serv, FIND_USER(sd), "", ""), sd);
                continue ;
            }
            if (user_mode.find(mode[i]) == std::string::npos)
                continue ;

            // "/mode -o"
            if (mode[i] == 'o' && user->get_nickname() != FIND_USER(sd)->get_nickname())
                forward_message(forward_RPL(481, serv, FIND_USER(sd), "", ""), sd);

            // "/mode -r"
            else if (mode[i] == 'r' && ((FIND_USER(sd)->get_mode().find('r') != std::string::npos) || (FIND_USER(sd)->get_mode().find('o') == std::string::npos )))
                forward_message(forward_RPL(481, serv, FIND_USER(sd), "", ""), sd);
                // 481 : ERR_NOPRIVILEGES

            // "/mode -i" {?}
            else
            {
                deleted_mode += mode[i];
                user_mode.erase(user_mode.find(mode[i]), 1);
            }

            // if (available_mode(mode[i], USER_MODE) == false)
            //     forward_message(forward_RPL(501, serv, FIND_USER(sd), "", ""), sd);
            // else if (user_mode.find(mode[i]) != std::string::npos)
            // {
            //     // o
            //     if (mode[i] == 'o' && user->get_nickname() != FIND_USER(sd)->get_nickname())
            //         forward_message(forward_RPL(481, serv, FIND_USER(sd), "", ""), sd);
            //     // r
            //     else if (mode[i] == 'r' && ((FIND_USER(sd)->get_mode().find('r') != std::string::npos) || (FIND_USER(sd)->get_mode().find('o') == std::string::npos )))
            //         forward_message(forward_RPL(481, serv, FIND_USER(sd), "", ""), sd);
            //     // i {?}
            //     else
            //     {
            //         deleted_mode += mode[i];
            //         user_mode.erase(user_mode.find(mode[i]), 1);
            //     }
            // }
        }
        user->set_mode(user_mode);
        std::string user_feedback = user_output(FIND_USER(user_socket_fd));
        // if (!deleted_mode.empty())
        if (deleted_mode != "")
            user_feedback += "MODE " + user->get_nickname() + " -" + deleted_mode;
        if (user_feedback.find("MODE") != std::string::npos)
            forward_message(user_feedback, user_socket_fd);
    }
    else
    {
        std::string  added_mode;
        int     i = -1;

        // for ((mode[0] != '+') ? i = 0 : i = 1; mode[i]; i++)
        if (mode[0] == '+')
            ++i;
        while (++i < (int) mode.length())
        {
            // if (available_mode(mode[i], USER_MODE) == false)
            if ( !available_mode(mode[i], USER_MODE))
            {
                forward_message(forward_RPL(501, serv, FIND_USER(sd), "", ""), sd);
            }
            else if (user_mode.find(mode[i]) == std::string::npos) // mode requested not exists yet
            {
                if (mode[i] == 'o' && FIND_USER(sd)->get_mode().find('o') == std::string::npos)
                    forward_message(forward_RPL(481, serv, FIND_USER(sd), "", ""), sd);
                else if (mode[i] == 'r' && user->get_nickname() == FIND_USER(sd)->get_nickname())
                    added_mode += mode[i];
                else if (mode[i] == 'r' && ((FIND_USER(sd)->get_mode().find('r') != std::string::npos) || (FIND_USER(sd)->get_mode().find('o') == std::string::npos)))
                    forward_message(forward_RPL(481, serv, FIND_USER(sd), "", ""), sd);
                else
                    added_mode += mode[i];
            }
        }
        user->set_mode(user_mode + added_mode);
        std::string user_feedback = user_output(FIND_USER(user_socket_fd));
        if (!added_mode.empty())
            user_feedback += "MODE " + user->get_nickname() + " +" + added_mode;
        if (user_feedback.find("MODE") != std::string::npos)
            forward_message(user_feedback, user_socket_fd);
    }
}

void mode(Server *serv, std::string buffer, int sd)
{
    std::string buf(buffer);
    int j = 0;
    size_t i;
    std::string target_msg = "";
    if ((i = buf.find_first_not_of(SEP_CHARSET, 5)) != std::string::npos)
        target_msg = buf.substr(i, ((j = buf.find_first_of(SEP_CHARSET, i)) - i));
    if (target_msg.empty())
    {
        forward_message(forward_RPL(461, serv, FIND_USER(sd), "MODE", ""), sd);
        return ;
    }
    std::string chan_id = "#&+";
    j = buf.find_first_not_of(SEP_CHARSET, j);
    std::string mode = "";
    if (buf.find_first_of(SEP_CHARSET, j) != std::string::npos)
        mode = buf.substr(j, (buf.find_first_of(SEP_CHARSET, j) - j));
    if (!target_msg.empty() && chan_id.find(target_msg[0]) != std::string::npos)
    {
        if (serv->get_channels().find(target_msg) == serv->get_channels().end())
            forward_message(forward_RPL(403, serv, FIND_USER(sd), target_msg, ""), sd);
        else if (FIND_USER(sd)->get_mode().find('r') != std::string::npos)
            forward_message(forward_RPL(484, serv, FIND_USER(sd), "", ""), sd);
        else if (FIND_CHANNEL(target_msg)->get_chanops().find(sd) == FIND_CHANNEL(target_msg)->get_chanops().end())
            forward_message(forward_RPL(482, serv, FIND_USER(sd), target_msg, ""), sd);
        else
            channel_mode(serv, FIND_CHANNEL(target_msg), mode, sd, buffer);
    }
    else
    {
        int user_socket_fd;
        if ((user_socket_fd = serv->search_user_by_nickname(target_msg)) == -1)
            forward_message(forward_RPL(401, serv, FIND_USER(sd), target_msg, ""), sd);
        else if (mode.empty() && !FIND_USER(user_socket_fd)->get_mode().empty())
            forward_message(forward_RPL(221, serv, FIND_USER(user_socket_fd), '+' + FIND_USER(user_socket_fd)->get_mode(), ""), sd);
        else if (mode.empty() && FIND_USER(user_socket_fd)->get_mode().empty())
            forward_message("This user don't have any modes!", sd);
        else
            user_mode(serv, FIND_USER(user_socket_fd), mode, sd);
    }
    
}
