#include "Freenode.hpp"

void ping(Server *serv, std::string buffer, int sd)
{
    (void)  serv;

    // :D

    buffer[1] = 'O';
    Broadcast(buffer, sd);
}
