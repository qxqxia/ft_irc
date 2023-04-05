comp	:=	c++ -Wall -Werror -Wextra -I ../../project/ -std=c++98
rmv	:=	rm -rf

name	=	${Server} ${Client}

Server	:=	server
Client	:=	client

src	:=	server.cc client.cc
bud	:=	$(src:%.cc=%.o)

all	:	$(name)

%.o	:	%.cc
		@ echo "compiling [$^] "
		@ $(comp) -c $< -o $@

$(Server):	$(bud)
		@ echo "compiling [$@] "
		@ $(comp) $(Server).o -o ${Server}

$(Client):	$(bud)
		@ echo "compiling [$@] "
		@ $(comp) $(Client).o -o ${Client}

test	:	re
		@ ./server
		@ make f

clean	:
		$(rmv) $(bud)

fclean	:	clean
		$(rmv) $(name)

t	:	test
f	:	fclean
r	:	re
re	:	f all

.PHONY	:	fclean clean all re f

