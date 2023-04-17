#include "Freenode.hpp"

//  /join #channel
//  Joins the specified channel

bool is_char_invalid(char c)
{
    return (c == '\0' || c == '\a' || c == '\r' || c == '\n' || c == ' ' || c == ',' || c == ':');
}

bool is_channelname_valid(std::string name)
{
    std::string::iterator        it;
    if (name.size() > 50)
        return false;
    std::string channelId = "#&+";
    if (!name.empty() && channelId.find(name[0]) == std::string::npos)
        return false;
    it = name.begin();
    while (it != name.end() && !is_char_invalid(*it))
        ++it;
    return (it == name.end());
}

void join(Server *serv, std::string buffer, int sd)
{
    size_t i = 0, j = 0;
    std::string buf(buffer);
    std::string channel_names_as_string = "";
    if ((i = buf.find_first_not_of(SEP_CHARSET, 5)) != std::string::npos)
        channel_names_as_string = buf.substr(i, ((j = buf.find_first_of(SEP_CHARSET, i)) - i));
    if (channel_names_as_string.empty())
    {
        forward_message(forward_RPL(461, serv, FIND_USER(sd), "JOIN", ""), sd);
        return ;
    }
    int total_channel_count = 1 + count(channel_names_as_string.begin(), channel_names_as_string.end(), ',');
    std::string keysForChannels = "";
    if (buf.find_first_not_of(SEP_CHARSET, j) != std::string::npos)
    {
        j = buf.find_first_not_of(SEP_CHARSET, j);
        keysForChannels = buf.substr(j, (buf.find_first_of(SEP_CHARSET, j) - j));
    }
    for (int i = 0; i < total_channel_count; i++)
    {
        std::string channel_name = channel_names_as_string.substr(0, channel_names_as_string.find(","));
        channel_names_as_string.erase(0, channel_names_as_string.find(",") + 1);
        if (!(FIND_USER(sd)->get_channels_size() < 10))
        {
            forward_message(forward_RPL(405, serv, FIND_USER(sd), channel_name, ""), sd);
            break;
        }
        if (!is_channelname_valid(channel_name))
        {
            forward_message(channel_name + ":Erroneous Channel Name", sd);
            continue;
        }
        std::string key = keysForChannels.substr(0, keysForChannels.find(","));
        keysForChannels.erase(0, keysForChannels.find(",") + 1);
        if (serv->get_channels().find(channel_name) == serv->get_channels().end())
        {
            Channel *chan = new Channel(channel_name);
            serv->get_channels(channel_name, chan);
        }
        if (FIND_CHANNEL(channel_name)->get_mode().find("b") != std::string::npos)
        {
            if (FIND_CHANNEL(channel_name)->is_banned(FIND_USER(sd)->get_nickname()) == true)
            {
                forward_message(forward_RPL(474, serv, FIND_USER(sd), channel_name, ""), sd);
                continue;
            }
        }
        if (FIND_CHANNEL(channel_name)->get_mode().find("k") != std::string::npos)
        {
            if (key.empty())
            {
                forward_message(forward_RPL(461, serv, FIND_USER(sd), "JOIN", ""), sd);
                return ;
            }
            if (FIND_CHANNEL(channel_name)->get_key() == key)
            {
                forward_message(forward_RPL(475, serv, FIND_USER(sd), channel_name, ""), sd);
                continue;
            }
        }
        if (FIND_CHANNEL(channel_name)->get_mode().find("l") != std::string::npos)
        {
            if (FIND_CHANNEL(channel_name)->get_maximum_users() <= FIND_CHANNEL(channel_name)->get_user_number())
            {
                forward_message(forward_RPL(471, serv, FIND_USER(sd), channel_name, ""), sd);
                continue;
            }
        }
        // Adding client to server
        if (FIND_CHANNEL(channel_name)->get_user_number() == 0)
            FIND_CHANNEL(channel_name)->add_chanop(sd, FIND_USER(sd));
        else if (FIND_CHANNEL(channel_name)->search_user_by_nickname(FIND_USER(sd)->get_nickname()) != -1)
            return ;
        else
            FIND_CHANNEL(channel_name)->add_user(sd, FIND_USER(sd));
        FIND_USER(sd)->add_channel(channel_name);
        std::string user_feedback = user_output(FIND_USER(sd));
        user_feedback += "JOIN " + channel_name;
        if (FIND_CHANNEL(channel_name)->get_mode().find("a") == std::string::npos)
            send_everyone_in_channel(user_feedback, FIND_CHANNEL(channel_name));
        if (FIND_CHANNEL(channel_name)->get_topic() == "")
            forward_message(forward_RPL(331, serv, FIND_USER(sd), channel_name, ""), sd);
        else
            forward_message(forward_RPL(332, serv, FIND_USER(sd), channel_name, FIND_CHANNEL(channel_name)->get_topic()), sd);
        std::string listOfUser = FIND_CHANNEL(channel_name)->get_list_of_users_in_channel();
        if (FIND_CHANNEL(channel_name)->get_mode().find("a") == std::string::npos)
        {
            forward_message(forward_RPL(353, serv, FIND_USER(sd), channel_name, listOfUser), sd);
            forward_message(forward_RPL(366, serv, FIND_USER(sd), channel_name, ""), sd);
        }
        if (!FIND_CHANNEL(channel_name)->get_mode().empty())
            forward_message(forward_RPL(324, serv, FIND_USER(sd), channel_name, FIND_CHANNEL(channel_name)->get_mode()), sd);
    }
}
