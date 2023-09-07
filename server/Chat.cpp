#include "Chat.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <vector>


#define FILEUSERS "users.txt"
#define FILEMESS "messages.txt"
#define MESSAGE_LENGTH 1024



void Chat::chatOn()
{
	chatStatus_ = true;
}

void Chat::chatOff()
{
	chatStatus_ = false;

}

const bool Chat::getStatus() const
{
	return chatStatus_;
}

const std::shared_ptr<User>& Chat::getSelectedUser() const
{
	return selectedUser_;
}

//void Chat::mainMenu()
//{
//	char act;
//	
//	load(FILEUSERS, users_);
//	load(FILEMESS, messages_);
//	std::cout << div << "Choose an action:\n1 - Reg in chat\n2 - Sign in\n0 - quit" << std::endl;
//	std::cin >> act;
//	switch (act-48) {
//	case 0:
//	{
//		chatOff();
//		upload(FILEUSERS, users_);
//		upload(FILEMESS, messages_);
//		break;
//	}
//	case 1:
//		regChat();
//		break;
//	case 2:
//		signIn();
//		break;
//	default:
//		std::cout << "Unknown command!" << std::endl;
//	}
//}

void Chat::mainMenu(const int& connection, bool& on)
{
	char act;
	read(connection, &act, sizeof(act));
	switch (act)
	{
	case '1':
	{
		this->regChat(connection);
		break;
	}
	case '2':
	{
		this->signIn(connection);
		break;
	}
	case '0':
	{
		on = false;
		break;
	}
	default:
		break;
	}
}

void Chat::regChat(const int& connection)
{
	std::string userParam[3] = {};
	auto allow = 0;
	char message[MESSAGE_LENGTH];
	read(connection, message, sizeof(message));
	 do{
		 if (users_.empty())
		 {
			 write(connection, "true", sizeof("true"));
			 break;
		 }
		allow = 0;
		for (auto& user : users_)
		{
			if (message == user.getLogin())
			{
				write(connection, "false", sizeof("false"));
				break;
			}
			else
			{
				allow ++;
				write(connection, "true", sizeof("true"));
			}
		}
	}while (allow!=users_.size());
	read(connection, message, sizeof(message));
	int j = 0;
	for (auto i = 0; i < strlen(message); i++)
	{
		if (message[i] == ' ')
			j++;
		else
			userParam[j] += message[i];
	}
	users_.push_back(User(userParam[0], userParam[1], userParam[2]));
}

void Chat::signIn(const int& connection)
{
	char message[MESSAGE_LENGTH];
	std::string userParam[2] = {};
	char allow = '0';
	if (users_.empty())
	{
		write(connection, &allow, sizeof(allow));
		return;
	}
	else
	{
		allow = '1';
		write(connection, &allow, sizeof(allow));
	}
	read(connection, message, sizeof(message));
	int j = 0;
	for (auto i = 0; i < strlen(message); i++)
	{
		if (message[i] == ' ')
			j++;
		else
			userParam[j] += message[i];
	}
	allow = '0';
	int buf = 0;
	for (auto& user : users_)
	{
		if (userParam[0] == user.getLogin())
		{
			allow = '1';
			break;
		}
		buf++;
	}
	if (allow == '1')
	{
		if (userParam[1] == users_[buf].getPass())
		{
			write(connection, &allow, sizeof(allow));
			selectedUser_ = std::make_shared<User>(users_[buf]);
		}
	}
	else
		write(connection, &allow, sizeof(allow));
}

void Chat::userMenu(const int& connection)
{
	char act;
	read(connection, &act, sizeof(act));
	switch (act) {
	case '0':
		signOut(connection);
		break;
	case '1':
		writeMessage(connection);
		break;
	case '2':
		usersList(connection);
		break;
	case '3':
		dispChat(connection);
		break;
	}
}

void Chat::writeMessage(const int& connection)
{
	char message[MESSAGE_LENGTH]{};
	std::string data[2];
	char allow = '1';
	bool find = false;
	read(connection, message, sizeof(message));
	for (int i = 0; i < strlen(message); i++)
		data[0] += message[i];
	for (auto& user : users_)
	{
		if (data[0] == user.getLogin())
		{
			find = true;
			break;
		}
	}
	if (data[0] != selectedUser_->getLogin() && (find || data[0] == "all"))
	{
		write(connection, &allow, sizeof(allow));
		read(connection, message, sizeof(message));
		for (int i = 0; i < strlen(message); i++)
			data[1] += message[i];
		messages_.push_back(Message(data[0], selectedUser_->getLogin(), data[1], selectedUser_->getName()));
	}
	else
	{
		allow = '0';
		write(connection, &allow, sizeof(allow));
	}
	
		
}

void Chat::usersList(const int& connection)
{
	char message[MESSAGE_LENGTH]{};
	char buf[MESSAGE_LENGTH]{};
	int size = users_.size();
	write(connection, &size, (sizeof(size)));
	for (auto& user : users_)
	{
		buf[0]='\0';
		if (user.getLogin() == selectedUser_->getLogin())
			strcpy(buf, " (this account)");
		std::cout << "Login : '" << user.getLogin()<< "' " << buf << std::endl;
		strcpy(message, "Login : '");
		strcat(message, user.getLogin().c_str());
		strcat(message, "' ");
		strcat(message, buf);
		write(connection, message, sizeof(message));
	}
	std::cout << std::endl;
}

void Chat::signOut(const int& connection)
{
	selectedUser_ = nullptr;
}

void Chat::dispChat(const int& connection)
{
	char message[MESSAGE_LENGTH]{};
	int size = messages_.size();
	write(connection, &size, sizeof(size));
	for (auto& messages : messages_)
	{
		if ((selectedUser_->getLogin() == messages.getFrom() || selectedUser_->getLogin() == messages.getTo() || messages.getTo() == "all"))
		{
			std::cout << div << "From : " << messages.getFromPname() << "\nTo : " << messages.getTo() << "\nText : " << messages.getText() << std::endl;
			strcpy(message, "From : ");
			strcat(message, messages.getFromPname().c_str());
			strcat(message, "\nTo : ");
			strcat(message, messages.getTo().c_str());
			strcat(message, "\nText : ");
			strcat(message, messages.getText().c_str());
			write(connection, message, sizeof(message));
		}
	}
}

template<typename T>
inline void Chat::load(std::string file_path, std::vector<T>& mass)
{
	std::fstream file = std::fstream(file_path, std::ios::in);
	if (!file)
	{
		std::cout << "Download from the file cannot be finished, file not found!";
		return;
	}
	file.seekg(0, std::ios_base::beg);
	while (!file.eof())
	{
		if (!file.eof())
		mass.push_back(T());
		file >> mass.back();
	}
	file.close();
}

template<class T>
void Chat::upload(std::string file_path, std::vector<T>& mass)
{
	std::fstream file = std::fstream(file_path, std::ios::out | std::ios::trunc);
		if (!file)
			std::cout << "Download from the file cannot be finished, file not found!";
		file.seekg(0, std::ios_base::beg);
		for (auto& i : mass)
		{
			file << i;
		}
}
