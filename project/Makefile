srcs	=	srcs/Freenode.cpp  \
		srcs/User.cpp  \
		srcs/Channel.cpp  \
		srcs/Server.cpp   \
		srcs/send_rpl_err.cpp   \
		srcs/send_outputs.cpp   \
		srcs/commands/restart.cpp  \
		srcs/commands/privmsg.cpp  \
		srcs/commands/join.cpp  \
		srcs/commands/topic.cpp  \
		srcs/commands/quit.cpp  \
		srcs/commands/nick.cpp  \
		srcs/commands/part.cpp  \
		srcs/commands/ping.cpp  \
		srcs/commands/pong.cpp  \
		srcs/commands/mode.cpp  \
		srcs/commands/kill.cpp  \
		srcs/commands/kick.cpp  \
		srcs/commands/oper.cpp

obj	=	$(srcs:.cpp=.o)

inc	=	-I includes

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
