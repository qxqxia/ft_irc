#include "iostream"

int	main()
{
	{
		system("ls -a");
		system("ls | grep out");
		// system("python3");
		system("node");
		system("1 + 1"); // this line executed after python exited
	}
}
