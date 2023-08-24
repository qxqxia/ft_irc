#include "Freenode.hpp"

//  /kick #channel nickname
//      - Kicks a nickname off a channel that you are on

void kick(Server *serv, std::string buffer, int sd)
{
    std::string buf(buffer);

    int     total_kicks = std::count(buf.begin(), buf.end(), '\n');
    int     total_chans;
    
    size_t  i, j;
    int     key;

    // int     key, l;

    std::string channel_names_str = "";
    if ((i = buf.find_first_not_of(SEP_CHARSET, 5)) != std::string::npos)
        channel_names_str = buf.substr(i, ((j = buf.find_first_of(SEP_CHARSET, i)) - i));
    if (channel_names_str.empty())
    {
        Broadcast(Get_RPL_ERR(461, serv, FIND_USER(sd), "KICK", ""), sd);
        return ;
    }

    total_chans = 1 + std::count(channel_names_str.begin(), channel_names_str.end(), ',');

    j = buf.find_first_not_of(SEP_CHARSET, j);
    key = j;


    int     x = -1;
    while (++x < total_kicks)
    {
        std::string user_nicks_str = buf.substr(key, (buf.find_first_of(SEP_CHARSET, key) - key));
        int         total_users = 1 + std::count(user_nicks_str.begin(), user_nicks_str.end(), ',');

        if (user_nicks_str.empty())
        {
            Broadcast(Get_RPL_ERR(461, serv, FIND_USER(sd), "KICK", ""), sd);
            return ;
        }


        int     y = -1;
        while (++y < total_users)
        {
            std::string userNick = user_nicks_str.substr(0, user_nicks_str.find(","));
            user_nicks_str.erase(0, user_nicks_str.find(",") + 1);


            int     z = -1;
            while (++z < total_chans)            
            {
                std::string tmp = channel_names_str;
                std::string channel_name = tmp.substr(0, tmp.find(","));
                tmp.erase(0, tmp.find(",") + 1);

                if (serv->get_channels().find(channel_name) == serv->get_channels().end())
                {
                    if (j == 0)
                        Broadcast(Get_RPL_ERR(403, serv, FIND_USER(sd), channel_name, ""), sd);
                    continue ;
                }
                // if (FIND_CHANNEL(channel_name)->get_mode().find("a") != std::string::npos)
                // {
                //     continue ;
                // }
                if (FIND_CHANNEL(channel_name)->search_user_by_nickname(FIND_USER(sd)->get_nickname()) == -1)
                {
                    Broadcast(Get_RPL_ERR(442, serv, FIND_USER(sd), channel_name, ""), sd);
                    continue ;
                }
                if (FIND_USER(sd)->get_mode().find('r') != std::string::npos)
                {
                    Broadcast(Get_RPL_ERR(484, serv, FIND_USER(sd), "", ""), sd);
                    continue ;
                }
                if ( !FIND_CHANNEL(channel_name)->is_chanop(sd))
                {
                    Broadcast(Get_RPL_ERR(482, serv, FIND_USER(sd), channel_name, ""), sd);
                    continue ;
                }

                int socket_fd_user_to_kick;

                if ((socket_fd_user_to_kick = FIND_CHANNEL(channel_name)->search_user_by_nickname(userNick)) == -1)
                {
                    Broadcast(Get_RPL_ERR(441, serv, FIND_USER(sd), userNick, channel_name), sd);
                    continue ;
                }

                std::string user_answer = user_output(FIND_USER(socket_fd_user_to_kick));
                user_answer += "PART " + channel_name;

                send_everyone_in_channel(user_answer, FIND_CHANNEL(channel_name));
                FIND_CHANNEL(channel_name)->left_user_of_what_use(socket_fd_user_to_kick);

                if (FIND_CHANNEL(channel_name)->get_user_number() == 0)
                {
                    delete serv->get_channels().find(channel_name)->second;
                    serv->get_channels().erase(channel_name);
                }
                FIND_USER(socket_fd_user_to_kick)->get_channels().erase(channel_name);
            }
        }
        if (buf.find('\r') != std::string::npos)
        {
            buf.erase(0, buf.find("\r\n") + 2);
        }
        else
        {
            buf.erase(0, buf.find('\n') + 1);
        }
    }
}
