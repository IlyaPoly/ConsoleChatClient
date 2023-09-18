#include "connect.h"
#include "menu.h"

#define MESSAGE_LENGTH 1024

int  main(int argc, char* argv[])
{
	int connection = connect();
	bool in = false;
	while(1)
	{
		if (!in)
		mainMenu(connection, in);
		else
		userMenu(connection, in);
	}
	return 0;
}
