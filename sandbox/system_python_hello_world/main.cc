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

	// FILE *fp is a pointer to a FILE object, which is a file stream
	//  fp is used to capture the output of the executed python command 
	//  by calling popen() with the complete command as an argument 
	//  to open a pipe to the stdout of the executed command
	//  The resulting FILE object is then used to read the output

	while (fgets(buff, sizeof(buff), fp) != nullptr)
	{
		cout << buff;
	}

	// fgets reads a line from a file stream and stores it in a buffer
	//  here fgets() reads the output of the executed python command
	//  one line at a time from the FILE object fp
	//  then stores it in the buff
	//  once done, we std::cout the contents of the buffer
	//
	//  The loop goes on until the end of the output stream
	//  fgets() returning nullptr by that time


	pclose(fp);
}


