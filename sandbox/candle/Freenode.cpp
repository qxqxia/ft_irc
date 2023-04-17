#include "Freenode.hpp"

int checkarg2(std::string);
int checkarg1(char *);

int main(int argc, char **argv)
{
	try
	{
		if (argc != 3)
		{
			throw std::runtime_error("Usage: ./ircserv <port> <token>");
		}
		if (checkarg1(argv[1]) == 1)
		{
			throw std::runtime_error("Port is invalid.");
		}
		if (checkarg2(argv[2]) == 1)
		{
			throw std::runtime_error("Token is invalid.");
		}

		Server	server(argv[1], argv[2]);
		server.connect_to_server();
	}
	catch (const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}

int checkarg2(std::string str)
{
	if (str.size() != 4)
		return (1);
	for (int i = 0;str[i]; i++)
		// if (isdigit(str[i]) == 0)
		if (isprint(str[i]) == 0)
			return 1;
	return 0;
}

int checkarg1(char *v1)
{
	if (strlen(v1) != 4)
		return 1;
	int port = static_cast<int>(strtoul(v1, NULL, 0));
	// if ((port >= 6660 && port <= 6669) || port == 6679 || port == 6697)
	if (port ^ 6666)
		return 0;
	return 1;
}