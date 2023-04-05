comp	:=	c++ -Wall -Werror -Wextra -pedantic -MMD -MP -std=c++98
comp	:=	c++ -Wall -Werror -Wextra -I ../../project/ -std=c++98
rmv	:=	rm -rf

name	:=	btc
name	:=	RPN
name	:=	PmergeMe
name	:=	ircserv
name	:=	out

src	:=	main.cc
bud	:=	$(src:%.cc=%.o)


%.o	:	%.cc
		@ echo "compiling [$^] "
		@ $(comp) -c $^ -o $@

$(name)	:	$(bud)
		@ echo "compiling [$@] "
		@ $(comp) $^ -o $@
		@ printf "\n\n"
		@#@ printf "\t[en] come on baby break my code\n"
		@#@ printf "\t[fr] viens, brise mon code\n\n\n"
		@ printf "\t\"are you using the right compiler? \"\n\n\n"
		@ echo "you can do [make run] "
		@ echo "you can do [make r] "
		@ echo "you can do [make f] "
		@ echo ""
		@ echo "-"
		@#@ echo ":::: (makefile ends) :::: "
		@ echo ""

test	:	re
		@ for i in {1..6}; do echo $$i; ./$(name); done
		@ echo ""
		@ for a in A B C D E F; do echo $$a && ./$(name); done

all	:	$(name)

clean	:
		@ $(rmv) $(bud)

fclean	:	clean
		@ $(rmv) $(name) a.out out

t	:	test
f	:	fclean
r	:	re
re	:	f all

.PHONY	:	fclean clean all re f

