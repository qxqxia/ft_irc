#include "Freenode.hpp"

//  /nick nickname
//  Changes your nickname to a new nickname

bool nickname_is_valid(std::string nick)
{
    std::string letter = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string special = "[]\\`_^{|}";
    std::string digit = "0123456789";
    std::string minus = "-";
    std::string firstChar = letter + special;
    std::string all = letter + special + digit + minus;
    if (nick.size() > 9)
        return (false);
    if (firstChar.find(nick[0]) == std::string::npos)
        return (false);
    for (int i = 1; nick[i] && i < 9; i++)
    {
        if (all.find(nick[i]) == std::string::npos)
            return (false);
    }
    return (true);
}

bool nickname_is_in_use(Server *serv, std::string nick)
{
    for (std::map<int, User *>::iterator it = serv->get_users().begin(); it != serv->get_users().end(); it++)
    {
        if (nick == it->second->get_nickname())
            return (true);
    }
    return (false);
}

void nick(Server *serv, std::string buffer, int sd)
{
    size_t i = 0;
    std::string newNickname = "";
    std::string buf(buffer);
    if ((i = buf.find_first_not_of(SEP_CHARSET, 5)) != std::string::npos)
        newNickname = buf.substr(i, (buf.find_first_of(SEP_CHARSET, i) - i));
    if (newNickname.empty())
    {
        forward_message(forward_RPL(431, serv, FIND_USER(sd), "", ""), sd);
        return ;
    }
    if (FIND_USER(sd)->get_mode().find('r') != std::string::npos)
    {
        forward_message(forward_RPL(484, serv, FIND_USER(sd), "", ""), sd);
        return ;
    }
    if (!nickname_is_valid(newNickname))
    {
        forward_message(forward_RPL(432, serv, FIND_USER(sd), newNickname, ""), sd);
        return ;
    }
    if (nickname_is_in_use(serv, newNickname))
    {
        forward_message(forward_RPL(433, serv, FIND_USER(sd), newNickname, ""), sd);
        return ;
    }
    std::string user_feedback = user_output(FIND_USER(sd));
    user_feedback += "NICK " + newNickname;
    forward_message(user_feedback, sd);
    FIND_USER(sd)->set_nick(newNickname);
}