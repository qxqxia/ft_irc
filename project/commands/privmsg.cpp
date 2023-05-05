#include "Freenode.hpp"

void privmsg(Server *serv, std::string buffer, int sd)
{
    int occAfterCmd;
    std::string buf(buffer);

    // if (buf.compare(0, 6, "NOTICE") == 0)
    if (buf == "NOTICE")
        occAfterCmd = 7;
    else
        occAfterCmd = 8;

    size_t i;
    std::string msgtarget = "";

    if ((i = buf.find_first_not_of(SEP_CHARSET, occAfterCmd)) != std::string::npos)
        msgtarget = buf.substr(i, (buf.find_first_of(SEP_CHARSET, i) - i));

    std::string msg = "";
    size_t j;

    j = buf.find_first_of(SEP_CHARSET, i);
    if ((j = buf.find_first_not_of(SEP_CHARSET, j)) != std::string::npos)
    {
        msg = buf.substr(j, (buf.find_first_of(BUFFER_ENDS, j) - j));
    }

    std::string idOfChannel = "#&+";
    int user_to_send_socket_fd;
    std::string user_answer;

    user_answer = user_output(FIND_USER(sd));
    user_answer += "PRIVMSG " + msgtarget + " " + msg;

    if (!msgtarget.empty() && idOfChannel.find(msgtarget[0]) != std::string::npos)
    {
        if (serv->get_channels().find(msgtarget) == serv->get_channels().end())
            ___Broadcast___(___Broadcast_RPL_ERR___(401, serv, FIND_USER(sd), msgtarget, ""), sd);
        else if ((FIND_CHANNEL(msgtarget)->get_mode().find("n") != std::string::npos) && (FIND_CHANNEL(msgtarget)->search_user_by_nickname(FIND_USER(sd)->get_nickname()) == -1))
            ___Broadcast___(___Broadcast_RPL_ERR___(404, serv, FIND_USER(sd), msgtarget, ""), sd);
        else if ((FIND_CHANNEL(msgtarget)->get_mode().find("m") != std::string::npos) && (!FIND_CHANNEL(msgtarget)->is_chanop(sd)) && (!FIND_CHANNEL(msgtarget)->is_voice(sd)))
            ___Broadcast___(___Broadcast_RPL_ERR___(404, serv, FIND_USER(sd), msgtarget, ""), sd);
		else if (FIND_CHANNEL(msgtarget)->is_banned(FIND_USER(sd)->get_nickname()) == true)
            ___Broadcast___(___Broadcast_RPL_ERR___(404, serv, FIND_USER(sd), msgtarget, ""), sd);
        else if ((FIND_CHANNEL(msgtarget)->get_mode().find("a") != std::string::npos))
        {
            user_answer = anonymous_output() + "PRIVMSG " + msgtarget + " " + msg;
            send_everyone_in_channel_except_user(user_answer, FIND_CHANNEL(msgtarget), sd);
        }
        else
        {
            send_everyone_in_channel_except_user(user_answer, FIND_CHANNEL(msgtarget), sd);
        }
    }
    else
    {
        if ((user_to_send_socket_fd = serv->search_user_by_nickname(msgtarget)) == -1)
            ___Broadcast___(___Broadcast_RPL_ERR___(401, serv, FIND_USER(sd), msgtarget, ""), sd);
        else
            ___Broadcast___(user_answer, user_to_send_socket_fd);
    }
}
