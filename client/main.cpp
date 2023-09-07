#include "connect.h"
#include "menu.h"

int main()
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
