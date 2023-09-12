#include "menu.h"


void regChat(char* message)
{
	char buf[MESSAGE_LENGTH]{};
	std::system("clear");
	std::cout << "Input your name : ";
	std::cin >> buf;
	strcat(message, buf);
	std::cout << "Input login : ";
	std::cin >> buf;
	strcat(message, " ");
	strcat(message, buf);
	std::cout << "Input password : ";
	std::cin >> buf;
	strcat(message, " ");
	strcat(message, buf);
}
void mainMenu(const int& connection, bool& in)
{
	char message[MESSAGE_LENGTH]{};
	char act;
	std::cout <<"Choose an action:\n1 - Reg in chat\n2 - Sign in\n0 - quit" << std::endl;
	std::cin >> act;
	message[0] = act;
	switch (act - 48) {
	case 0:
	{
		write(connection, "0", sizeof("0"));
		exit(0); 
		break;
	}
	case 1:
	{
		regChat(message);
		break;
	}
	case 2:
	{
		signIn(message);
		break;
	}
	default:
		std::cout << "Unknown command!" << std::endl;
		return;
	}
	write(connection, message, sizeof(message));
	read(connection, message, sizeof(message));
	if (act == '2' && message[0] == '1')
	in = true;
	if (message[0] == '0')
	{
	strcpy(message,message+1);
	std::cout << message << std::endl;
	}
}

void signIn(char* message)
{
	char buf[MESSAGE_LENGTH]{};
	std::system("clear");
	std::cout << "Input your login: ";
	std::cin >> buf;
	strcat(message, buf);
	strcat(message, " ");
	std::cout << "Input your password: ";
	std::cin >> buf;
	strcat(message, buf);
}
void signOut(bool& in)
{
in = false;
}
void writeMessage(char* message)
{
	char buf[MESSAGE_LENGTH]{};
	std::system("clear");
	std::cout << "Write login recipient or 'all' : ";
	std::cin >> buf;
	strcat(message, buf);
	strcat(message, " ");
	std::cout << "Write your message : ";
	std::cin.ignore();
	std::cin.getline(buf,sizeof(buf));
	strcat(message, buf);
}
void outInfo(const int& connection, int act)
{
	std::system("clear");
	int size=0;
	char buf[MESSAGE_LENGTH]{};
	char message[MESSAGE_LENGTH]{};
	char end[] = "!!END!!";
	message[0] = act;
	write(connection, message, sizeof(message));
	while(1){
		read(connection, message, sizeof(message));
		size = message[0];
		strcpy(message,message+1);
		if (strlen(message) < size)
		while (strlen(message) < size)
		{
			read(connection, buf, sizeof(buf));
			strcat(message, buf);
		};
		if (!strcmp(message, end))
		break;
		std::cout << message << std::endl;
		bzero(message, sizeof(message));
	};
}
void userMenu(const int& connection, bool& in)
{
	const std::string div = "*****************\n";
	char act;
	char message[MESSAGE_LENGTH]{};
	std::cout << div << "Choose an action:\n1 - Write message\n2 - Users list\n3 - Display chat\n0 - Sign out" << std::endl;
	std::cin >> act;
	act += 2;
	message[0] = act;
	switch (act - 48) {
	case 2:
	{
		signOut(in);
		message[0] = '0';
		break;
	}
	case 3:
	{
		writeMessage(message);
		break;
	}
	case 4:
	{
		std::cout << "     Users :" << std::endl;
		outInfo(connection, act);
		break;
	}
	case 5:
	{
		std::cout << "     Messages :" << std::endl;
		outInfo(connection, act);
		break;
	}
	default:
		std::cout << "Unknown command!" << std::endl;
		return;
	}
	if (act != '4' && act != '5')
	{
	write(connection, message, sizeof(message));
	read(connection, message, sizeof(message));
	if (message[0] == '0')
	{
		strcpy(message,message+1);
		std::cout << "Wrong! " << message << std::endl;
	}
	}

}
