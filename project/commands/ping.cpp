#include "Freenode.hpp"

void ping(Server *serv, std::string buffer, int sd)
{
    (void)  serv;

    buffer[1] = 'O';
    ___Broadcast___(buffer, sd);
}
