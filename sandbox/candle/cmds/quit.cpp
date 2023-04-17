#include "Freenode.hpp"

//  /quit [message]
//  Disconnects you from IRC and will give the optional message as the reason for your departure
//  (this message only appears to people who are on the same channels as you)

extern int g_Socket_Queue[MAX_CLIENTS];

void disconnect_user(Server* serv, int sd)
{
    std::set<std::string> userChannels = FIND_USER(sd)->get_channels();
    // erase user from each channel;
    for (std::set<std::string>::iterator it = userChannels.begin(); it != userChannels.end(); it++)
    {
        std::string user_feedback = user_output(FIND_USER(sd));
        user_feedback += "PART " + *it;
        send_everyone_in_channel(user_feedback, FIND_CHANNEL(*it));
        FIND_CHANNEL(*it)->left_user_of_what_use(sd);
        if (FIND_CHANNEL(*it)->get_user_number() == 0)
        {
            delete serv->get_channels().find(*it)->second;
            serv->get_channels().erase(*it);
        }
    }
    FIND_USER(sd)->get_channels().clear();
    delete FIND_USER(sd);
    serv->get_users().erase(sd);
    std::cout << "Host disconnected , ip " << inet_ntoa(serv->get_server().sin_addr) << " , port " << ntohs(serv->get_server().sin_port) << " , number of users: " <<  serv->get_users().size() << std::endl;
    for (int i = 0; i < MAX_CLIENTS; i++)
        if (g_Socket_Queue[i] == sd)
            g_Socket_Queue[i] = 0;
    close(sd);
}

void quit(Server *serv, std::string buffer, int sd)
{
    std::string buf(buffer);
    size_t i;
    std::string message;
    if ((i = buf.find_first_not_of(SEP_CHARSET, 5)) != std::string::npos)
        message = buf.substr(i, (buf.find_first_of(SEP_CHARSET, i) - i));
    std::string user_feedback;
    if (!message.empty())
        user_feedback = user_output(FIND_USER(sd)) + buffer;
    else
        user_feedback = user_output(FIND_USER(sd)) + "QUIT :leaving";
    disconnect_user(serv, sd);
    send_everyone(user_feedback, serv->get_users());
}