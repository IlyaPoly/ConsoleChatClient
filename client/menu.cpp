#include "menu.h"


void regChat(char* message)
{
	char buf[MESSAGE_LENGTH];
	//std::system("clear");
	std::cout << "Input your name : ";
	std::cin >> buf;
	strcpy(message, buf);
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
	char message[MESSAGE_LENGTH];
	char act;
	std::cout << div << "Choose an action:\n1 - Reg in chat\n2 - Sign in\n0 - quit" << std::endl;
	std::cin >> act;
	strcpy(message, &act);
	switch (act - 48) {
	case 0:
	{
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
	}
	write(connection, message, sizeof(message));
	read(connection, message, sizeof(message));
	if (act == '2' && message[0] == '1')
	in = true;
}

void signIn(char* message)
{
	char buf[MESSAGE_LENGTH];
	std::cout << "Input your login: ";
	std::cin >> message;
	std::cout << "Input your password: ";
	std::cin >> buf;
	strcat(message, " ");
	strcat(message, buf);
}
void signOut(bool& in)
{
in = false;
}
void writeMessage(char* message)
{
	char buf[MESSAGE_LENGTH];
	//std::system("clear");
	std::cout << "Write login recipient or 'all' : ";
	std::cin >> buf;
	std::cout << "Write your message : ";
	std::cin.ignore();
	std::cin.getline(message,sizeof(message));
}
void outInfo(const int& connection, char* message)
{
	write(connection, message, sizeof(message));
	do{
		read(connection, message, sizeof(message));
		if (message == "!!END!!")
		break;
		std::cout << message << std::endl;
	}while(!true);
}
void userMenu(const int& connection, bool& in)
{
	char act;
	char message[MESSAGE_LENGTH]{};
	std::cout << "Choose an action:\n3 - Write message\n4 - Users list\n5 - Display chat\n0 - Sign out" << std::endl;
	std::cin >> act;
	strcpy(message, &act);
	switch (act - 48) {
	case 0:
	{
		signOut(in);
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
		outInfo(connection,message);
		break;
	}
	case 5:
	{
		outInfo(connection, message);
		break;
	}
	default:
		std::cout << "Unknown command!" << std::endl;
	}
	write(connection, message, sizeof(message));
	read(connection, message, sizeof(message));
	if (message[0] == '0')
	{
		strcat(message,message+1);
		std::cout << "Wrong!" << message << std::endl;
	}

}
