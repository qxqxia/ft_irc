#include "Freenode.hpp"

void    part(Server *serv, std::string buffer, int sd)
{
    std::string buf(buffer);
    size_t i;

    std::string channel_name_string = "";

    int     total_channels;

    if ((i = buf.find_first_not_of(SEP_CHARSET, 5)) != std::string::npos)
    {
        channel_name_string = buf.substr(i, (buf.find_first_of(SEP_CHARSET, i) - i));
    }
    if (channel_name_string.empty())
    {
        Broadcast(Get_RPL_ERR(461, serv, FIND_USER(sd), "PART", ""), sd);
        return ;
    }
    std::string reason_message = "";
    size_t j = i;
    j = buf.find_first_of(SEP_CHARSET, j);
    j = buf.find_first_not_of(SEP_CHARSET, j);

    int jj = j;

    // find the space btw the [reason] and [room]
    if ((j = buf.find_first_of(BUFFER_ENDS, j)) != std::string::npos)
    {
        reason_message = buf.substr(jj, (buf.find_first_of(BUFFER_ENDS, jj) - jj));
    }

    // bug fix :: chances that when a user wants to leave multiple channels at once
    //  `/part channelalpha,channelbeta `
    total_channels = 1 + std::count(channel_name_string.begin(), channel_name_string.end(), ',');

    i = -1;
    while (++i < (size_t) total_channels)
    {
        std::string channel_name = channel_name_string.substr(0, channel_name_string.find(","));
        channel_name_string.erase(0, channel_name_string.find(",") + 1);
        
        // 2 things might happen before leaving
        //  1/2 :: chan not found ----> will send RPL `No Saj Chan`

        if (serv->get_channels().find(channel_name) == serv->get_channels().end())
        {
            Broadcast(Get_RPL_ERR(403, serv, FIND_USER(sd), channel_name, ""), sd);
        }

        //  2/2 :: user not in chan --> will send RPL `Not On Chan`
        else if (FIND_USER(sd)->get_channels().find(channel_name) == FIND_USER(sd)->get_channels().end())
        {
            Broadcast(Get_RPL_ERR(442, serv, FIND_USER(sd), channel_name, ""), sd);
        }
        else
        {
            std::string user_answer = user_output(FIND_USER(sd));
            user_answer += "PART " + channel_name + " " + reason_message;

            ////    +/- a :: anonymous mode (draft)
            // if (FIND_CHANNEL(channel_name)->get_mode().find("a") == std::string::npos)
            // {
            //     send_everyone_in_channel(user_answer, FIND_CHANNEL(channel_name));
            // }
            // else
            // {
                Broadcast(user_answer, sd); // Keep this line
            // }
            FIND_CHANNEL(channel_name)->clear_user_possible_privilege(sd);
            if (FIND_CHANNEL(channel_name)->get_user_number() == 0)
            {
                delete  serv->get_channels().find(channel_name)->second;

                serv->get_channels().erase(channel_name);
            }
            FIND_USER(sd)->get_channels().erase(channel_name);
        }
    }
}
