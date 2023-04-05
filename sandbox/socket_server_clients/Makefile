comp	:=	c++ -std=c++98 -Wall -Werror -Wextra -pedantic -MMD -MP
comp	:=	c++ -std=c++17 -Wall -Werror -Wextra -I ../../project/
rmv	:=	rm -rf

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
		@ echo ""
		@ echo ""

test	:	re
		@ ./$(name)

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
