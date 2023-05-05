#pragma once
#ifndef __COMMAND_HPP__
# define __COMMAND_HPP__

# include "Freenode.hpp"

class	Server;

void	privmsg(Server *, std::string, int);

void	nick(Server *, std::string, int);
void	join(Server *, std::string, int);
void	ping(Server *, std::string, int);
void	part(Server *, std::string, int);

void	topic(Server *, std::string, int);

void	kick(Server *, std::string, int);
void	mode(Server *, std::string, int);
void	oper(Server *, std::string, int);
void	kill(Server *, std::string, int);
void	quit(Server *, std::string, int);

void	restart(Server *, std::string, int);


#endif



