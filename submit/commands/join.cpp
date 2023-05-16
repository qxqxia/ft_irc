#include "Freenode.hpp"

//  /join #channel
//      - Joins the specified channel

bool is_invalid_character(char c)
{
    return (c == '\0' || c == '\a' || c == '\r' || c == '\n' || c == ' ' || c == ',' || c == ':');
}

bool is_invalid_channel_name(std::string name)
{
    std::string::iterator   it;
    std::string             channel_prefixes;

    channel_prefixes = "#&+";

    if (name.size() > 50)
    {
        return (false);
    }
    if (!name.empty() && channel_prefixes.find(name[0]) == std::string::npos)
    {
        return (false);
    }

    it = name.begin();

    while (it != name.end() && !is_invalid_character(*it))
    {
        ++it;
    }
    return (it == name.end());
}

void join(Server *serv, std::string buffer, int sd)
{

    std::string     buf(buffer);
    std::string     channel_list_str = "";

    size_t  i = 0, j = 0;

    if ((i = buf.find_first_not_of(SEP_CHARSET, 5)) != std::string::npos)
    {
        channel_list_str = buf.substr(i, ((j = buf.find_first_of(SEP_CHARSET, i)) - i));
    }

    //  ERR :: No chanlist means empty params
    if (channel_list_str.empty())
    {
        Broadcast(Get_RPL_ERR(461, serv, FIND_USER(sd), "JOIN", ""), sd);
        return ;
    }

    int total_chans = 1 + std::count(channel_list_str.begin(), channel_list_str.end(), ',');
    std::string keys_for_chans = "";

    if (buf.find_first_not_of(SEP_CHARSET, j) != std::string::npos)
    {
        j = buf.find_first_not_of(SEP_CHARSET, j);
        keys_for_chans = buf.substr(j, (buf.find_first_of(SEP_CHARSET, j) - j));
    }

    //  ~~TODO~~ DBG To Delete on Submit
    // std::cout << CYAN "(dbg)(JOIN)(chan_list_str): " << channel_list_str << nlreset;
    // std::cout << YELLOW "(dbg)(JOIN)(keys_for_chan): " << keys_for_chans << nlreset;


    for (int _ = 0; _ < total_chans; _++)
    {
        std::string channel_name = channel_list_str.substr(0, channel_list_str.find(","));
        channel_list_str.erase(0, channel_list_str.find(",") + 1);

        if (!(FIND_USER(sd)->get_channels_size() < MAX_CLIENTS /* 10*/ ))
        {
            Broadcast(Get_RPL_ERR(405, serv, FIND_USER(sd), channel_name, ""), sd);
            break ;
        }

        if (!is_invalid_channel_name(channel_name))
        {
            Broadcast(channel_name + ":Erroneous Channel Name", sd);
            continue ;
        }

        std::string     key;
        
        key = keys_for_chans.substr(0, keys_for_chans.find(","));
        keys_for_chans.erase(0, keys_for_chans.find(",") + 1);

        if (serv->get_channels().find(channel_name) == serv->get_channels().end())
        {
            Channel *chan = new Channel(channel_name);
            serv->get_channels(channel_name, chan);
        }

        if (FIND_CHANNEL(channel_name)->get_mode().find("b") != std::string::npos)
        {
            if (FIND_CHANNEL(channel_name)->is_banned(FIND_USER(sd)->get_nickname()) == true)
            {
                Broadcast(Get_RPL_ERR(474, serv, FIND_USER(sd), channel_name, ""), sd);
                continue ;
            }
        }

        if (FIND_CHANNEL(channel_name)->get_mode().find("l") != std::string::npos)
        {
            if (FIND_CHANNEL(channel_name)->get_maximum_users() <= FIND_CHANNEL(channel_name)->get_user_number())
            {
                Broadcast(Get_RPL_ERR(471, serv, FIND_USER(sd), channel_name, ""), sd);
                continue ;
            }
        }

        //  add client to server
        if (FIND_CHANNEL(channel_name)->get_user_number() == 0)
        {
            FIND_CHANNEL(channel_name)->add_chanop(sd, FIND_USER(sd));
        }
        else if (FIND_CHANNEL(channel_name)->search_user_by_nickname(FIND_USER(sd)->get_nickname()) != -1)
        {
            return ;
        }
        else
        {
            FIND_CHANNEL(channel_name)->add_user(sd, FIND_USER(sd));
        }

        FIND_USER(sd)->add_channel(channel_name);

        std::string user_answer = user_output(FIND_USER(sd));
        user_answer += "JOIN " + channel_name;

        if (FIND_CHANNEL(channel_name)->get_mode().find("a") == std::string::npos)
        {
            send_everyone_in_channel(user_answer, FIND_CHANNEL(channel_name));
        }

        std::string list_of_users = FIND_CHANNEL(channel_name)->get_list_of_users_in_channel();

        if (FIND_CHANNEL(channel_name)->get_mode().find("a") == std::string::npos)
        {
            Broadcast(Get_RPL_ERR(353, serv, FIND_USER(sd), channel_name, list_of_users), sd);
            Broadcast(Get_RPL_ERR(366, serv, FIND_USER(sd), channel_name, ""), sd);
        }
        if (!FIND_CHANNEL(channel_name)->get_mode().empty())
        {
            Broadcast(Get_RPL_ERR(324, serv, FIND_USER(sd), channel_name, FIND_CHANNEL(channel_name)->get_mode()), sd);
        }
    }
}
