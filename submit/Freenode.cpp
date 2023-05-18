#include "Freenode.hpp"

int		is_valid_pass(char *);
int		is_valid_port(char *);


//	Drive

int	main(int c, char **v)
{
	try
	{
		if (c ^ 3)
		{
			throw (std::runtime_error("Usage: ./ircserv <port> <token>"));
		}
		if ( !is_valid_port(v[1]))
		{
			throw (std::runtime_error("Port is invalid."));
		}
		if ( !is_valid_pass(v[2]))
		{
			throw (std::runtime_error("Password is invalid."));
		}

		// Server server( v[1], v[2] );
		// server.connect_to_server();

		Server	MY_SERVER( v[1], v[2] );

		MY_SERVER.connect_to_server();
	}
	catch (const std::exception & e)
	{
		std::cerr << e.what() << std::endl;

		return (1);
	}
	return (0);
}


// helper

int is_valid_pass(char * pass_str)
{
	int		i;

	if (strlen(pass_str) ^ 4)
		return 0;
	i = -1;
	while (pass_str[++i])
	{
		if ( !isprint(pass_str[i])) // now tolerates all printable chars // if (isdigit(str[i]) == 0)
		{
			return (0);
		}
	}
	return (1);
}

int is_valid_port(char *port_str)
{
	int		port;

	if (strlen(port_str) ^ 4)
	{
		return 0;
	}
	port = static_cast<int>(std::strtoul(port_str, NULL, 0));

	// port should have a range like : ((port >= 6660 && port < 6670) || port == 6679 || port == 6697)

	// The standard port for IRC connection is 6667
	// More often, IRC networks use a range instead of just one

	// A typical range is [6660, 6669] or [6660, 7000]
	// 6679 and 6697 are used for secure IRC connections
	// 6697 = standard port for SSL-encrypted IRC connections
	// 6679 = an alternative port for SSL connections
	// [*, 1024] = "well-known ports" reserved for specific services or applications

	if (port >= 6660 && port < 7001)
	{
		return (1);
	}
	return (0);
}