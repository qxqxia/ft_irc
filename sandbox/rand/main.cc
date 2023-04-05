#include "iostream"
#include "unistd.h"
#include "Fmt.hpp"

int	main()
{
	int	end = 10;
	int	i = -1;

	srand(time(0));
	while (++i < end)
	{
		int n = rand() % 10;
		std::cout << n << ' ';
		usleep(13579);
	}
	std::cout << std::endl;
}

