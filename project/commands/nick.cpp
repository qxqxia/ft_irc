#include "Freenode.hpp"

//  /nick nickname
//  Changes your nickname to a new nickname

bool nickname_is_validated(std::string nick)
{
    std::string alpha, nonalnum, digit;

    alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    digit = "0123456789";
    nonalnum = "[]\\`_^{|}";

    std::string firstChar = alpha + nonalnum;
    std::string all = alpha + nonalnum + digit + "-";

    if (nick.size() > 9)
        return (false);
    if (firstChar.find(nick[0]) == std::string::npos)
        return (false);

    int i = 0;

    while (nick[++i] && i < 9)
    {
        if (all.find(nick[i]) == std::string::npos)
            return (false);
    }

    return (true);
}

bool nickname_is_in_use(Server *serv, std::string nick)
{

    std::map<int, User *>::iterator     it;

    it = serv->get_users().begin();
    while (it != serv->get_users().end())
    {
        if (it->second->get_nickname() == nick)
            return (true);
        ++it;
    }

    return (false);
}

void    nick(Server *serv, std::string buffer, int sd)
{
    size_t i = 0;

    std::string     nick_to_set = "";
    std::string     buf(buffer);

    if ((i = buf.find_first_not_of(SEP_CHARSET, 5)) != std::string::npos)
    {
        nick_to_set = buf.substr(i, (buf.find_first_of(SEP_CHARSET, i) - i));
    }

    if (nick_to_set.empty())
    {
        Broadcast(Get_RPL_ERR(431, serv, FIND_USER(sd), "", ""), sd);

        return ;
    }

    if (FIND_USER(sd)->get_mode().find('r') != std::string::npos)
    {
        Broadcast(Get_RPL_ERR(484, serv, FIND_USER(sd), "", ""), sd);

        return ;
    }

    if ( ! nickname_is_validated(nick_to_set))
    {
        Broadcast(Get_RPL_ERR(432, serv, FIND_USER(sd), nick_to_set, ""), sd);

        return ;
    }

    if (nickname_is_in_use(serv, nick_to_set))
    {
        Broadcast(Get_RPL_ERR(433, serv, FIND_USER(sd), nick_to_set, ""), sd);

        return ;
    }

    std::string user_answer;
    
    user_answer = user_output(FIND_USER(sd));
    user_answer += "NICK " + nick_to_set;

    Broadcast(user_answer, sd);

    FIND_USER(sd)->set_nick(nick_to_set);
}