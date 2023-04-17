#include "Freenode.hpp"

void ping(Server *serv, std::string buffer, int sd)
{
    (void)serv;
    buffer[1] = 'O';
    forward_message(buffer, sd);
}
