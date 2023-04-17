#include "Freenode.hpp"

//  KILL <nick> <reason>
//  Kill a user. This will forcibly disconnect the user from the IRC server, like a forceful QUIT.

void kill(Server *serv, std::string buffer, int sd)
{
    std::string buf(buffer);
    int j = 0;

    if (FIND_USER(sd)->get_mode().find('o') == std::string::npos)
    {
        forward_message(forward_RPL(481, serv, FIND_USER(sd), "", ""), sd);
        return ;
    }
    size_t i;
    std::string name = "";
    if ((i = buf.find_first_not_of(SEP_CHARSET, 5)) != std::string::npos)
        name = buf.substr(i, ((j = buf.find_first_of(SEP_CHARSET, i)) - i));
    if (name.empty())
    {
        forward_message(forward_RPL(461, serv, FIND_USER(sd), "KILL", ""), sd);
        return ;
    }
    if (serv->search_user_by_nickname(name) == -1)
    {
        forward_message(forward_RPL(401, serv, FIND_USER(sd), name, ""), sd);
        return ;
    }
    disconnect_user(serv, serv->search_user_by_nickname(name));
}