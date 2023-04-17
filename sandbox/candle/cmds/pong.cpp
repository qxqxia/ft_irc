#include "Freenode.hpp"

void pong(Server *serv, std::string buffer, int sd)
{
    (void)serv;
    buffer[1] = 'I';
    forward_message(buffer, sd);
}