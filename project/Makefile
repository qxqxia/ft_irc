srcs	=	Freenode.cpp  \
		User.cpp  \
		Channel.cpp  \
		Server.cpp   \
		forward_rpl_err.cpp   \
		forward_outputs.cpp   \
		commands/restart.cpp  \
		commands/privmsg.cpp  \
		commands/join.cpp  \
		commands/topic.cpp  \
		commands/quit.cpp  \
		commands/nick.cpp  \
		commands/part.cpp  \
		commands/ping.cpp  \
		commands/mode.cpp  \
		commands/kill.cpp  \
		commands/kick.cpp  \
		commands/oper.cpp  \
		bot/Bot.cpp

obj	=	$(srcs:.cpp=.o)

inc	=	-I includes
inc	=	-I ./ -I ./bot/

comp	=	c++ -Wall -Wextra -Werror -std=c++98 -g3

name	=	ircserv

rm	=	rm -rf

%.o	:	%.cpp
		@ echo "compiling [$^] "
		@ $(comp) $(inc) -c $< -o $@


all	:	$(name)

$(name)	:	$(obj)
		@ echo "compiling [$@] "
		@ $(comp) $(inc) $(obj) -o $(name)
		@ printf "\n\n"
		@ echo ""
		@ printf "\t\"Are you using the right compiler? \n\n"
		@ echo ""
		@ echo ""
		@ echo "[$@] is ready "

clean:
		@ $(rm) $(obj)

fclean:		clean
		@ $(rm) $(name)

f	:	fclean
fo	:	clean

r	:	re
re	:	fclean all

.PHONY:		all clean fclean re

