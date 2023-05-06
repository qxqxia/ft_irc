#include "Freenode.hpp"

//  /topic #channel newtopic
//  Changes the topic for a channel that you are on

void topic(Server *serv, std::string buffer, int sd)
{
    std::string     channel_name = "";
    std::string     buf(buffer);

    size_t i, j = 0;

    if ((i = buf.find_first_not_of(SEP_CHARSET, 6)) != std::string::npos)
    {
        channel_name = buf.substr(i, ((j = buf.find_first_of(SEP_CHARSET, i)) - i));
    }
    if (channel_name.empty())
    {
        Broadcast(get_RPL_ERR(461, serv, FIND_USER(sd), "TOPIC", ""), sd);
    }
    else if (serv->get_channels().find(channel_name) == serv->get_channels().end())
    {
        Broadcast(get_RPL_ERR(403, serv, FIND_USER(sd), channel_name, ""), sd);
    }
    else if (FIND_USER(sd)->get_channels().find(channel_name) == FIND_USER(sd)->get_channels().end())
    {
        Broadcast(get_RPL_ERR(442, serv, FIND_USER(sd), channel_name, ""), sd);
    }
    else if ((FIND_CHANNEL(channel_name)->get_mode().find("t") != std::string::npos) && (FIND_USER(sd)->get_mode().find('r') != std::string::npos))
    {
        Broadcast(get_RPL_ERR(484, serv, FIND_USER(sd), "", ""), sd);
    }
    else if ((FIND_CHANNEL(channel_name)->get_mode().find("t") != std::string::npos) && (FIND_CHANNEL(channel_name)->get_chanops().find(sd) == FIND_CHANNEL(channel_name)->get_chanops().end()))
    {
        Broadcast(get_RPL_ERR(482, serv, FIND_USER(sd), channel_name, ""), sd);
    }
    else
    {
        std::string topic = "";
        if ((j = buf.find_first_not_of(SEP_CHARSET, j)) != std::string::npos)
        {
            if (buf[j] == ':')
            {
                j++;
            }
            topic = buf.substr(j, (buf.find_first_of(BUFFER_ENDS, j) - j));
        }
        if (topic.empty())
        {
            Broadcast(get_RPL_ERR(332, serv, FIND_USER(sd), channel_name, FIND_CHANNEL(channel_name)->get_topic()), sd);
        }
        else
        {
            FIND_CHANNEL(channel_name)->set_topic(topic);
            Broadcast(get_RPL_ERR(332, serv, FIND_USER(sd), channel_name, FIND_CHANNEL(channel_name)->get_topic()), sd);
            std::string user_answer = user_output(FIND_USER(sd));
            if (FIND_CHANNEL(channel_name)->get_mode().find("a") != std::string::npos)
            {
                user_answer = anonymous_output();
            }
            user_answer += "TOPIC " + FIND_CHANNEL(channel_name)->get_topic();
            send_everyone_in_channel(user_answer, FIND_CHANNEL(channel_name));
        }
    }
}