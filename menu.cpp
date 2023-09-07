#include "menu.h"

void regChat(const int& connection)
{
	char message[MESSAGE_LENGTH]{}, buf[MESSAGE_LENGTH];
	char allow[6];
	//std::system("clear");
	std::cout << "Input your name : ";
	std::cin >> buf;
	strcpy(message, buf);
	do {
		std::cout << "Input login : ";
		std::cin >> buf;
		write(connection, buf, sizeof(buf));
		read(connection, allow, sizeof(allow));
		if (allow == "false")
			std::cout << "Login is busy!" << std::endl;	
	} while (allow == "true");
	strcat(message, " ");
	strcat(message, buf);
	std::cout << "Input password : ";
	std::cin >> buf;
	strcat(message, " ");
	strcat(message, buf);
	write(connection, message, sizeof(message));
}

void mainMenu(const int& connection, bool& in)
{
	char act;
	std::cout << div << "Choose an action:\n1 - Reg in chat\n2 - Sign in\n0 - quit" << std::endl;
	std::cin >> act;
	switch (act - 48) {
	case 0:
	{
		write(connection, &act, sizeof(act));
		exit(0);
		break;
	}
	case 1:
	{
		write(connection, &act, sizeof(act));
		regChat(connection);
		break;
	}
	case 2:
	{
		write(connection, &act, sizeof(act));
		signIn(connection, in);
		break;
	}
	default:
		std::cout << "Unknown command!" << std::endl;
	}
}

void signIn(const int& connection, bool& in)
{
	char message[MESSAGE_LENGTH]{}, buf[MESSAGE_LENGTH];
	char allow;
	read(connection,message,sizeof(message));
	if (message[0] == '0')
	{
		std::cout << "No registered accounts!" << std::endl;
		return;
	}
	std::cout << "Input your login: ";
	std::cin >> message;
	std::cout << "Input your password: ";
	std::cin >> buf;
	strcat(message, " ");
	strcat(message, buf);
	write(connection, message, sizeof(message));
	read(connection, &allow,sizeof(allow));
	if (allow == '1')
	in = true;
	else if (allow == '0')
	std::cout << "invalid username/password" << std::endl;
}



void signOut(bool& in)
{
in = false;
}

void writeMessage(const int& connection)
{
	char message[MESSAGE_LENGTH]{}, buf[MESSAGE_LENGTH];
	//std::system("clear");
	std::cout << "Write login recipient or 'all' : ";
	std::cin >> buf;
	write(connection, buf, sizeof(buf));
	read(connection, message, sizeof(message));
	if (message[0] == '0')
	{
		std::cout << "Wrong!" << std::endl;
		return;
	} else 
	if(message[0] == '1')
	{
		std::cout << "Write your message : ";
		std::cin.ignore();
		std::cin.getline(message,sizeof(message));
		write(connection, message, sizeof(message));
	}
}

void outInfo(const int& connection, const int& size)
{
	char message[MESSAGE_LENGTH]{};
	for (int i = 0; i < size; i++)
	{
		read(connection, message, sizeof(message));
		std::cout << message << std::endl;
	}
}



void userMenu(const int& connection, bool& in)
{
	char act;
	
	std::cout << "Choose an action:\n1 - Write message\n2 - Users list\n3 - Display chat\n0 - Sign out" << std::endl;
	std::cin >> act;
	switch (act - 48) {
	case 0:
	{
		write(connection, &act, sizeof(act));
		signOut(in);
		break;
	}
	case 1:
	{
		write(connection, &act, sizeof(act));
		writeMessage(connection);
		break;
	}
	case 2:
	{
		int size=0;
		write(connection, &act, sizeof(act));
		read(connection, &size, sizeof(size));
		std::cout << "     Users :" << std::endl;
		outInfo(connection, size);
		break;
	}
	case 3:
	{
		int size=0;
		write(connection, &act, sizeof(act));
		read(connection, &size, sizeof(size));
		if (size == 0)
		{
			std::cout << "No message!" << std::endl;
			break;
		}
		outInfo(connection, size);
		break;
	}
	default:
		std::cout << "Unknown command!" << std::endl;
	}
}
