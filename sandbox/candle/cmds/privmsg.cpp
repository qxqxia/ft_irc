#include "Freenode.hpp"

//  /privmsg nickname message
//  Sends a private message to nickname that will open a query window for the other user.

void privmsg(Server *serv, std::string buffer, int sd)
{
    int occAfterCmd;
    std::string buf(buffer);
    if (buf.substr(0, 6) == "NOTICE")
        occAfterCmd = 7;
    else
        occAfterCmd = 8;
    size_t i;
    std::string target_msg = "";
    if ((i = buf.find_first_not_of(SEP_CHARSET, occAfterCmd)) != std::string::npos)
        target_msg = buf.substr(i, (buf.find_first_of(SEP_CHARSET, i) - i));
    std::string msg = "";
    size_t j;
    j = buf.find_first_of(SEP_CHARSET, i);
    if ((j = buf.find_first_not_of(SEP_CHARSET, j)) != std::string::npos)
        msg = buf.substr(j, (buf.find_first_of(BUFFER_ENDS, j) - j));
    std::string chan_id = "#&+";
    int userToSendSd;
    std::string user_feedback;
    user_feedback = user_output(FIND_USER(sd));
    user_feedback += "PRIVMSG " + target_msg + " " + msg;
    if (!target_msg.empty() && chan_id.find(target_msg[0]) != std::string::npos)
    {
        if (serv->get_channels().find(target_msg) == serv->get_channels().end())
            forward_message(forward_RPL(401, serv, FIND_USER(sd), target_msg, ""), sd);
        else if ((FIND_CHANNEL(target_msg)->get_mode().find("n") != std::string::npos) && (FIND_CHANNEL(target_msg)->search_user_by_nickname(FIND_USER(sd)->get_nickname()) == -1))
            forward_message(forward_RPL(404, serv, FIND_USER(sd), target_msg, ""), sd);
        else if ((FIND_CHANNEL(target_msg)->get_mode().find("m") != std::string::npos) && (!FIND_CHANNEL(target_msg)->is_chanop(sd)) && (!FIND_CHANNEL(target_msg)->is_voice(sd)))
            forward_message(forward_RPL(404, serv, FIND_USER(sd), target_msg, ""), sd);
		else if (FIND_CHANNEL(target_msg)->is_banned(FIND_USER(sd)->get_nickname()) == true)
            forward_message(forward_RPL(404, serv, FIND_USER(sd), target_msg, ""), sd);
        else if ((FIND_CHANNEL(target_msg)->get_mode().find("a") != std::string::npos))
        {
            user_feedback = anonymous_output() + "PRIVMSG " + target_msg + " " + msg;
            send_everyone_in_channel_except_user(user_feedback, FIND_CHANNEL(target_msg), sd);
        }
        else
            send_everyone_in_channel_except_user(user_feedback, FIND_CHANNEL(target_msg), sd);
    }
    else
    {
        if ((userToSendSd = serv->search_user_by_nickname(target_msg)) == -1)
            forward_message(forward_RPL(401, serv, FIND_USER(sd), target_msg, ""), sd);
        else
            forward_message(user_feedback, userToSendSd);
    }
}
