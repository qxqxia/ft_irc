#include "Freenode.hpp"

void restart(Server *serv, std::string buffer, int sd)
{
	(void) buffer;

	if (FIND_USER(sd)->get_mode().find('o') == std::string::npos)
    {
		Broadcast(Get_RPL_ERR(481, serv, FIND_USER(sd), "", ""), sd);
	}
	else
	{
		serv->set_is_restarting();
	}
}