#include "Freenode.hpp"

//  /join #channel
//  Joins the specified channel

bool check_invalid_character(char c)
{
    return (c == '\0' || c == '\a' || c == '\r' || c == '\n' || c == ' ' || c == ',' || c == ':');
}

bool channelNameInvalid(std::string name)
{
    std::string::iterator   it;
    std::string             channel_id;

    if (name.size() > 50)
        return false;

    channel_id = "#&+";
    if (!name.empty() && channel_id.find(name[0]) == std::string::npos)
    {
        return false;
    }

    it = name.begin();
    while (it != name.end() && !check_invalid_character(*it))
    {
        ++it;
    }
    return (it == name.end());
}

void join(Server *serv, std::string buffer, int sd)
{
    size_t i = 0, j = 0;
    std::string buf(buffer);
    std::string channel_list_str = "";
    if ((i = buf.find_first_not_of(SEP_CHARSET, 5)) != std::string::npos)
        channel_list_str = buf.substr(i, ((j = buf.find_first_of(SEP_CHARSET, i)) - i));
    if (channel_list_str.empty())
    {
        ___Broadcast___(___Broadcast_RPL_ERR___(461, serv, FIND_USER(sd), "JOIN", ""), sd);
        return ;
    }
    int total_chans = 1 + std::count(channel_list_str.begin(), channel_list_str.end(), ',');
    std::string keys_for_chans = "";
    if (buf.find_first_not_of(SEP_CHARSET, j) != std::string::npos)
    {
        j = buf.find_first_not_of(SEP_CHARSET, j);
        keys_for_chans = buf.substr(j, (buf.find_first_of(SEP_CHARSET, j) - j));
    }
    for (int i = 0; i < total_chans; i++)
    {
        std::string channel_name = channel_list_str.substr(0, channel_list_str.find(","));
        channel_list_str.erase(0, channel_list_str.find(",") + 1);
        if (!(FIND_USER(sd)->get_channels_size() < MAX_CLIENTS /* 10 */))
        {
            ___Broadcast___(___Broadcast_RPL_ERR___(405, serv, FIND_USER(sd), channel_name, ""), sd);
            break ;
        }
        if (!channelNameInvalid(channel_name))
        {
            ___Broadcast___(channel_name + ":Erroneous Channel Name", sd);
            continue ;
        }
        std::string key = keys_for_chans.substr(0, keys_for_chans.find(","));
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
                ___Broadcast___(___Broadcast_RPL_ERR___(474, serv, FIND_USER(sd), channel_name, ""), sd);
                continue ;
            }
        }
        if (FIND_CHANNEL(channel_name)->get_mode().find("k") != std::string::npos)
        {
            if (key.empty())
            {
                ___Broadcast___(___Broadcast_RPL_ERR___(461, serv, FIND_USER(sd), "JOIN", ""), sd);
                return ;
            }
            // if (FIND_CHANNEL(channel_name)->get_key().compare(key) != 0)
            if (FIND_CHANNEL(channel_name)->get_key() != key)
            {
                ___Broadcast___(___Broadcast_RPL_ERR___(475, serv, FIND_USER(sd), channel_name, ""), sd);
                continue ;
            }
        }
        if (FIND_CHANNEL(channel_name)->get_mode().find("l") != std::string::npos)
        {
            if (FIND_CHANNEL(channel_name)->get_maximum_users() <= FIND_CHANNEL(channel_name)->get_user_number())
            {
                ___Broadcast___(___Broadcast_RPL_ERR___(471, serv, FIND_USER(sd), channel_name, ""), sd);
                continue ;
            }
        }
        //Adding client to server
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
        if (FIND_CHANNEL(channel_name)->get_topic() == "")
        {
            ___Broadcast___(___Broadcast_RPL_ERR___(331, serv, FIND_USER(sd), channel_name, ""), sd);
        }
        else
        {
            ___Broadcast___(___Broadcast_RPL_ERR___(332, serv, FIND_USER(sd), channel_name, FIND_CHANNEL(channel_name)->get_topic()), sd);
        }
        std::string listOfUser = FIND_CHANNEL(channel_name)->get_list_of_users_in_channel();
        if (FIND_CHANNEL(channel_name)->get_mode().find("a") == std::string::npos)
        {
            ___Broadcast___(___Broadcast_RPL_ERR___(353, serv, FIND_USER(sd), channel_name, listOfUser), sd);
            ___Broadcast___(___Broadcast_RPL_ERR___(366, serv, FIND_USER(sd), channel_name, ""), sd);
        }
        if (!FIND_CHANNEL(channel_name)->get_mode().empty())
        {
            ___Broadcast___(___Broadcast_RPL_ERR___(324, serv, FIND_USER(sd), channel_name, FIND_CHANNEL(channel_name)->get_mode()), sd);
        }
    }
}
