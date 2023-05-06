#pragma once

#include "Freenode.hpp"

class   Server;
class   Channel;

class   Bot
{
	typedef void (*command)(Server *, Channel *, int);

	private:
		std::map<std::string, command>  m_command_handler;

        std::string m_name;

	public:
		Bot();
		virtual ~Bot();

		std::string get_name() const;
		void find_command(Server * serv, Channel *chan, int sd, std::string cmd);
} ;

void    callin(Server *serv, Channel *chan, int sd);

void    help(Server *serv, Channel *chan, int sd);
void    callout(Server *serv, Channel *chan, int sd);

void    tell_date(Server *serv, Channel *chan, int sd);
void    tell_time(Server *serv, Channel *chan, int sd);
