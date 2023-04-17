srcs	=	Freenode.cpp  \
		User.cpp  \
		Channel.cpp  \
		Server.cpp   \
		forward_rpl.cpp   \
		forward_out.cpp   \
		cmds/restart.cpp  \
		cmds/privmsg.cpp  \
		cmds/join.cpp  \
		cmds/topic.cpp  \
		cmds/quit.cpp  \
		cmds/nick.cpp  \
		cmds/part.cpp  \
		cmds/ping.cpp  \
		cmds/pong.cpp  \
		cmds/mode.cpp  \
		cmds/kill.cpp  \
		cmds/kick.cpp  \
		cmds/oper.cpp

obj	=	$(srcs:.cpp=.o)

inc	=	-I includes
inc	=	-I ./

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
		@ printf "\t\"Are you // using the right compiler? \n\n"
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

