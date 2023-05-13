#include "Freenode.hpp"

void    part(Server *serv, std::string buffer, int sd)
{
    std::string buf(buffer);
    size_t i;
    std::string channelsName = "";
    int     total_channels;

    if ((i = buf.find_first_not_of(SEP_CHARSET, 5)) != std::string::npos)
        channelsName = buf.substr(i, (buf.find_first_of(SEP_CHARSET, i) - i));
    if (channelsName.empty())
    {
        Broadcast(get_RPL_ERR(461, serv, FIND_USER(sd), "PART", ""), sd);
        return ;
    }
    std::string message = "";
    size_t j = i;
    j = buf.find_first_of(SEP_CHARSET, j);
    j = buf.find_first_not_of(SEP_CHARSET, j);

    int jj = j;

    if ((j = buf.find_first_of(BUFFER_ENDS, j)) != std::string::npos)
        message = buf.substr(jj, (buf.find_first_of(BUFFER_ENDS, jj) - jj));

    total_channels = 1 + std::count(channelsName.begin(), channelsName.end(), ',');


    i = -1;
    while (++i < (size_t) total_channels)
    {
        std::string channel_name = channelsName.substr(0, channelsName.find(","));
        channelsName.erase(0, channelsName.find(",") + 1);
        if (serv->get_channels().find(channel_name) == serv->get_channels().end())
            Broadcast(get_RPL_ERR(403, serv, FIND_USER(sd), channel_name, ""), sd);
        else if (FIND_USER(sd)->get_channels().find(channel_name) == FIND_USER(sd)->get_channels().end())
            Broadcast(get_RPL_ERR(442, serv, FIND_USER(sd), channel_name, ""), sd);
        else
        {
            std::string user_answer = user_output(FIND_USER(sd));
            user_answer += "PART " + channel_name + " " + message;
            ////    +/- a :: anonymous mode (draft)
            // if (FIND_CHANNEL(channel_name)->get_mode().find("a") == std::string::npos)
            // {
            //     send_everyone_in_channel(user_answer, FIND_CHANNEL(channel_name));
            // }
            // else
            // {
                Broadcast(user_answer, sd); // Keep this line
            // }
            FIND_CHANNEL(channel_name)->left_user_of_what_use(sd);
            if (FIND_CHANNEL(channel_name)->get_user_number() == 0)
            {
                delete  serv->get_channels().find(channel_name)->second;

                serv->get_channels().erase(channel_name);
            }
            FIND_USER(sd)->get_channels().erase(channel_name);
        }
    }
}
