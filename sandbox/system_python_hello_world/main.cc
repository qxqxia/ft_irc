#include "iostream"

using	namespace std;

int	main()
{
	char		cmd_long[1024];
	char		buff[1024];

	const char	*cmd = "print('Hello, world!')";
	const char	*python_interpreter = "/usr/bin/python3";
	// path to interpreter

	snprintf(
		cmd_long,
		sizeof(cmd_long),
		"%s -c \"%s\"",
		python_interpreter,
		cmd
	);

	// use FILE *fp to read the command

	FILE	*fp;

	if ((fp = popen(cmd_long, "r")) == nullptr)
	{
		cout << "failed. \n";
		return 0;
	}

	while (fgets(buff, sizeof(buff), fp) != nullptr)
	{
		cout << buff;
	}

	pclose(fp);
}


