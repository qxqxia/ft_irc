#include "Freenode.hpp"

void ping(Server *serv, std::string buffer, int sd)
{
    (void)  serv;

    buffer[1] = 'O';
    Broadcast(buffer, sd);
    Broadcast(buffer, sd);
    /*
    
    PING
    
    - In IRC protocol, "ping" message is sent by server to client
    - Client must respond a "pong" to indicate that it is still connected & responsive
    - this is to ensure that the connection btw the server & clients is still active

    */
}

/*
void pong(Server *serv, std::string buffer, int sd)
{
    (void)serv;
    buffer[1] = 'I';
    Broadcast(buffer, sd);
}
*/