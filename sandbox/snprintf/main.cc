#include "iostream"

// `sn` means "size and null"

int	main()
{
	{
		char	buff[1024];
		int	n = 42;

		snprintf(buff, sizeof(buff), "let's print this number %d ", n);
		std::cout << buff << '\n';
	}
	/*{
		char	buff[1024];
		int	num = 21;

		snprintf(
			buff,
			sizeof(buff),
			"let's print this number: %d ",
			num
		);
		printf("%s \n", buffer);
	}*/
}
