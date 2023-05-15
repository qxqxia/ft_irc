#include "Freenode.hpp"

//  /quit [message]
//  Disconnects you from IRC and will give the optional message as the reason for your departure
//  (this message only appears to people who are on the same channels as you)

extern int g_server_client_socket[MAX_CLIENTS];

void disconnect_user(Server* serv, int sd)
{
    std::set<std::string> user_channels = FIND_USER(sd)->get_channels();

    // erase user from each channel;

    std::set<std::string>::iterator     it;

    it = user_channels.begin();
    while (it != user_channels.end())
    {
        std::string     user_answer;
        
        user_answer = user_output(FIND_USER(sd));
        user_answer += "PART " + *it;

        send_everyone_in_channel(user_answer, FIND_CHANNEL(*it));
        FIND_CHANNEL(*it)->clear_user_possible_privilege(sd);

        if (FIND_CHANNEL(*it)->get_user_number() == 0)
        {
            delete  serv->get_channels().find(*it)->second;

            serv->get_channels().erase(*it);
        }
        ++it;
    }
    FIND_USER(sd)->get_channels().clear();
    delete  FIND_USER(sd);

    serv->get_users().erase(sd);
    std::cout << "Host disconnected , ip " << inet_ntoa(serv->get_server().sin_addr) << " , port " << ntohs(serv->get_server().sin_port) << " , number of users: " <<  serv->get_users().size() << std::endl;

    int i = -1;
    while (++i < MAX_CLIENTS)
    {
        if (g_server_client_socket[i] == sd)
            g_server_client_socket[i] = 0;
    }
    close(sd);
}

void quit(Server *serv, std::string buffer, int sd)
{
    std::string     buf(buffer);
    std::string     message;

    size_t      i;

    if ((i = buf.find_first_not_of(SEP_CHARSET, 5)) != std::string::npos)
    {
        message = buf.substr(i, (buf.find_first_of(SEP_CHARSET, i) - i));
    }
    std::string user_answer;

    if (!message.empty())
    {
        user_answer = user_output(FIND_USER(sd)) + buffer;
    }
    else
    {
        user_answer = user_output(FIND_USER(sd)) + "QUIT :leaving";
    }

    disconnect_user(serv, sd);
    send_everyone(user_answer, serv->get_users());

}