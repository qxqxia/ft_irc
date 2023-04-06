#include "iostream"

using	namespace std;

int	main()
{
	string	python_interpreter = "/usr/bin/python3";

	string	cmd;
	char	cmd_long[1024];
	char	buff[1024];

	cout << "gimme a python command: ";
	getline(cin, cmd);

	snprintf(
		cmd_long,
		sizeof(cmd_long),
		"%s -c \"%s\"",
		python_interpreter.c_str(),
		cmd.c_str()
	);

	FILE	*fp;

	if ((fp = popen(cmd_long, "r")) == nullptr)
	{
		cout << "failed. \n";
		return 1;
	}

	while ( fgets(buff, sizeof(buff), fp) != nullptr)
	{
		cout << buff;
	}

	pclose(fp);
}

