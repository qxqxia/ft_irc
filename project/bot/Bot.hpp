#pragma once

#include "Freenode.hpp"

class   Server;
class   Channel;

class   Bot
{
	typedef void (*command)(Server *, Channel *, int);

	private:
		std::map<std::string, command>  m_command_handler;

        std::string		m_name;
		// std::string		m_weekday_answer;
		// std::string		m_mulgame_answer;

	public:
		Bot();
		~Bot();
		// virtual ~Bot();


		std::string get_name() const;
		void find_command(Server * serv, Channel *chan, int socket_fd, std::string cmd);

		// void play_mulgame(Server *serv, std::string cmd, int socket_fd); // Added
		// void play_weekday(Server *serv, std::string cmd, int socket_fd); // Added
} ;

void    help(Server *serv, Channel *chan, int sd);

void    summon(Server *serv, Channel *chan, int sd);
void    dismiss(Server *serv, Channel *chan, int sd);

void    tell_date(Server *serv, Channel *chan, int sd);
void    tell_time(Server *serv, Channel *chan, int sd);
