#include "Freenode.hpp"

//  /kick #channel nickname
//  Kicks a nickname off a channel that you are on

void kick(Server *serv, std::string buffer, int sd)
{
    std::string buf(buffer);
    int kick_count = count(buf.begin(), buf.end(), '\n');
    int k = 0;
    size_t i, j;
    std::string channel_names_as_string = "";
    if ((i = buf.find_first_not_of(SEP_CHARSET, 5)) != std::string::npos)
        channel_names_as_string = buf.substr(i, ((j = buf.find_first_of(SEP_CHARSET, i)) - i));
    if (channel_names_as_string.empty())
    {
        forward_message(forward_RPL(461, serv, FIND_USER(sd), "KICK", ""), sd);
        return ;
    }
    int total_channel_count = 1 + count(channel_names_as_string.begin(), channel_names_as_string.end(), ',');
    j = buf.find_first_not_of(SEP_CHARSET, j);
    k = j;
    for (int l = 0; l < kick_count; l++)
    {
        std::string user_nicks_as_string = buf.substr(k, (buf.find_first_of(SEP_CHARSET, k) - k));
        int total_nick_count = 1 + count(user_nicks_as_string.begin(), user_nicks_as_string.end(), ',');
        if (user_nicks_as_string.empty())
        {
            forward_message(forward_RPL(461, serv, FIND_USER(sd), "KICK", ""), sd);
            return ;
        }
        for (int j = 0; j < total_nick_count; j++)
        {
            std::string user_nick = user_nicks_as_string.substr(0, user_nicks_as_string.find(","));
            user_nicks_as_string.erase(0, user_nicks_as_string.find(",") + 1);
            for (int i = 0; i < total_channel_count; i++)
            {
                std::string tmp = channel_names_as_string;
                std::string channel_name = tmp.substr(0, tmp.find(","));
                tmp.erase(0, tmp.find(",") + 1);
                if (serv->get_channels().find(channel_name) == serv->get_channels().end())
                {
                    if (j == 0)
                        forward_message(forward_RPL(403, serv, FIND_USER(sd), channel_name, ""), sd);
                    continue;
                }
                if (FIND_CHANNEL(channel_name)->get_mode().find("a") != std::string::npos)
                    continue;
                if (FIND_CHANNEL(channel_name)->search_user_by_nickname(FIND_USER(sd)->get_nickname()) == -1)
                {
                    forward_message(forward_RPL(442, serv, FIND_USER(sd), channel_name, ""), sd);
                    continue;
                }
                if (FIND_USER(sd)->get_mode().find('r') != std::string::npos)
                {
                    forward_message(forward_RPL(484, serv, FIND_USER(sd), "", ""), sd);
                    continue ;
                }
                if (!FIND_CHANNEL(channel_name)->is_chanop(sd))
                {
                    forward_message(forward_RPL(482, serv, FIND_USER(sd), channel_name, ""), sd);
                    continue ;
                }
                int userToKickSd;
                if ((userToKickSd = FIND_CHANNEL(channel_name)->search_user_by_nickname(user_nick)) == -1)
                {
                    forward_message(forward_RPL(441, serv, FIND_USER(sd), user_nick, channel_name), sd);
                    continue;
                }
                std::string user_feedback = user_output(FIND_USER(userToKickSd));
                user_feedback += "PART " + channel_name;
                send_everyone_in_channel(user_feedback, FIND_CHANNEL(channel_name));
                FIND_CHANNEL(channel_name)->left_user_of_what_use(userToKickSd);
                if (FIND_CHANNEL(channel_name)->get_user_number() == 0)
                {
                    delete serv->get_channels().find(channel_name)->second;
                    serv->get_channels().erase(channel_name);
                }
                FIND_USER(userToKickSd)->get_channels().erase(channel_name);
            }
        }
        if (buf.find('\r') != std::string::npos)
            buf.erase(0, buf.find("\r\n") + 2);
        else
            buf.erase(0, buf.find('\n') + 1);
    }
}
