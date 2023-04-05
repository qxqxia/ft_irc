#include "iostream"
#include "Fmt.hpp"

int	main()
{
	srand(time(0));
	if (rand() % 3)
	{
		std::cout << TICK TICK TICK " :: ";
		perror("funny & odd");
	}
	else
	{
		std::cout << CROSS CROSS CROSS " :: golden even \n";
	}
}

