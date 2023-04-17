#include "Freenode.hpp"

//  OPER <name> <password>
//  You use this command to become an IRCOp. IRCOps are configured in the Oper block.

void oper(Server *serv, std::string buffer, int sd)
{
    std::string buf(buffer);
    int j = 0;
    size_t i;
    std::string user;
    if ((i = buf.find_first_not_of(SEP_CHARSET, 5)) != std::string::npos)
        user = buf.substr(i, ((j = buf.find_first_of(SEP_CHARSET, i)) - i));
    if (user.empty())
    {
        forward_message(forward_RPL(461, serv, FIND_USER(sd), "OPER", ""), sd);
        return ;
    }
    if (serv->search_user_by_nickname(user) == -1)
    {
        forward_message(forward_RPL(401, serv, FIND_USER(sd), user, ""), sd);
        return ;
    }
    j = buf.find_first_not_of(SEP_CHARSET, j);
    std::string password = buf.substr(j, (buf.find_first_of(SEP_CHARSET, j) - j));
    if (password.empty())
    {
        forward_message(forward_RPL(461, serv, FIND_USER(sd), "OPER", ""), sd);
        return ;
    }
    if (password == OPER_PW)
    {
        std::string user_feedback = user_output(FIND_USER(serv->search_user_by_nickname(user)));
        user_feedback += "MODE " + FIND_USER(serv->search_user_by_nickname(user))->get_nickname() + " +o";
        FIND_USER(serv->search_user_by_nickname(user))->set_mode(FIND_USER(serv->search_user_by_nickname(user))->get_mode() + "o");
        forward_message(user_feedback, serv->search_user_by_nickname(user));
        forward_message(forward_RPL(381, serv, FIND_USER(serv->search_user_by_nickname(user)), "", ""), serv->search_user_by_nickname(user));
    }
    else
        forward_message(forward_RPL(464, serv, FIND_USER(serv->search_user_by_nickname(user)), "", ""), sd);
}