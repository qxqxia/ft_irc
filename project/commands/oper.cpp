#include "Freenode.hpp"

void oper(Server *serv, std::string buffer, int sd)
{
    std::string buf(buffer);
    std::string user;
    std::string user_answer;
    std::string password;

    int     j = 0;
    size_t  i;

    if ((i = buf.find_first_not_of(SEP_CHARSET, 5)) != std::string::npos)
    {
        user = buf.substr(i, ((j = buf.find_first_of(SEP_CHARSET, i)) - i));
    }
    if (user == "")
    {
        ___Broadcast___(___Broadcast_RPL_ERR___(461, serv, FIND_USER(sd), "OPER", ""), sd);
        return ;
    }
    if (serv->search_user_by_nickname(user) == -1)
    {
        ___Broadcast___(___Broadcast_RPL_ERR___(401, serv, FIND_USER(sd), user, ""), sd);
        return ;
    }
    j = buf.find_first_not_of(SEP_CHARSET, j);
    password = buf.substr(j, (buf.find_first_of(SEP_CHARSET, j) - j));
    if (password == "")
    {
        ___Broadcast___(___Broadcast_RPL_ERR___(461, serv, FIND_USER(sd), "OPER", ""), sd);
        return ;
    }
    if (password != OPER_PW)
    {
        ___Broadcast___(___Broadcast_RPL_ERR___(464, serv, FIND_USER(serv->search_user_by_nickname(user)), "", ""), sd);
        return ;
    }

    // else
    // {

    user_answer = user_output(FIND_USER(serv->search_user_by_nickname(user)));
    user_answer += "MODE " + FIND_USER(serv->search_user_by_nickname(user))->get_nickname() + " +o";

    FIND_USER(serv->search_user_by_nickname(user))->set_mode(
        FIND_USER(serv->search_user_by_nickname(user))->get_mode() + "o"
    );

    ___Broadcast___(user_answer, serv->search_user_by_nickname(user));
    ___Broadcast___(___Broadcast_RPL_ERR___(381, serv, FIND_USER(serv->search_user_by_nickname(user)), "", ""), serv->search_user_by_nickname(user));

    // }

}