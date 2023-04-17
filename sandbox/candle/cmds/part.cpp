#include "Freenode.hpp"

void    part(Server *serv, std::string buffer, int sd)
{
    std::string buf(buffer);
    size_t i;
    std::string channel_names_as_string = "";
    if ((i = buf.find_first_not_of(SEP_CHARSET, 5)) != std::string::npos)
        channel_names_as_string = buf.substr(i, (buf.find_first_of(SEP_CHARSET, i) - i));
    if (channel_names_as_string.empty())
    {
        forward_message(forward_RPL(461, serv, FIND_USER(sd), "PART", ""), sd);
        return ;
    }
    std::string message = "";
    size_t j = i;
    j = buf.find_first_of(SEP_CHARSET, j);
    j = buf.find_first_not_of(SEP_CHARSET, j);
    int u = j;
    if ((j = buf.find_first_of(BUFFER_ENDS, j)) != std::string::npos)
        message = buf.substr(u, (buf.find_first_of(BUFFER_ENDS, u) - u));
    int total_channel_count = 1 + count(channel_names_as_string.begin(), channel_names_as_string.end(), ',');
    for (int i = 0; i < total_channel_count; i++)
    {
        std::string channel_name = channel_names_as_string.substr(0, channel_names_as_string.find(","));
        channel_names_as_string.erase(0, channel_names_as_string.find(",") + 1);
        if (serv->get_channels().find(channel_name) == serv->get_channels().end())
            forward_message(forward_RPL(403, serv, FIND_USER(sd), channel_name, ""), sd);
        else if (FIND_USER(sd)->get_channels().find(channel_name) == FIND_USER(sd)->get_channels().end())
            forward_message(forward_RPL(442, serv, FIND_USER(sd), channel_name, ""), sd);
        else
        {
            std::string user_feedback = user_output(FIND_USER(sd));
            user_feedback += "PART " + channel_name + " " + message;
            if (FIND_CHANNEL(channel_name)->get_mode().find("a") == std::string::npos)
                send_everyone_in_channel(user_feedback, FIND_CHANNEL(channel_name));
            else
                forward_message(user_feedback, sd);
            FIND_CHANNEL(channel_name)->left_user_of_what_use(sd);
            if (FIND_CHANNEL(channel_name)->get_user_number() == 0)
            {
                delete serv->get_channels().find(channel_name)->second;
                serv->get_channels().erase(channel_name);
            }
            FIND_USER(sd)->get_channels().erase(channel_name);
        }
    }
}
