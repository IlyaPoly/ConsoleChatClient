#include "Chat.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <vector>


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

/*const std::shared_ptr<User>& Chat::getSelectedUser() const
{
	return selectedUser_;
}*/

void Chat::messToParam(std::string* param, const char* data)
{
	int j = 0;
	for (auto i = 0; i < strlen(data); i++)
	{
		if (data[i] == ' ')
			j++;
		else
			param[j] += data[i];
	}
}

void Chat::regChat(const char* data, char* ans)
{
	std::string userParam[3] = {};
	messToParam(userParam, data);
	auto allow = 0;
	 do{
		for (auto& user : users_)
		{
			if (userParam[0] == user.getLogin())
			{
				break;
			}
			else
			{
				allow ++;
			}
		}
	}while (allow!=users_.size());
	if (users_.empty() || allow == users_.size())
	{
		users_.push_back(User(userParam[0], userParam[1], userParam[2]));
		ans[0] = '1';
	}
	else 
	strcpy(ans, "0Login is busi!");
}

void Chat::signIn(const char* data, char* ans, std::shared_ptr<User>& selectedUser_)
{
	std::string userParam[2] = {};
	if (users_.empty())
	{
		strcpy(ans, "0Register users not found!");
		return;
	}
	messToParam(userParam, data);
	bool allow = false;
	int buf = 0;
	for (auto& user : users_)
	{
		if (userParam[0] == user.getLogin())
		{
			allow = true;
			break;
		}
		buf++;
	}
	if (allow)
	{
		if (userParam[1] == users_[buf].getPass())
		{
			selectedUser_ = std::make_shared<User>(users_[buf]);
			ans[0] = '1';
		}
	}
	if (selectedUser_ == nullptr)
	strcpy(ans, "0Wrong login/pass!");
}

void Chat::writeMessage(const char* data, char* ans, std::shared_ptr<User>& selectedUser_)
{
	std::string mess[3] = {};
	messToParam(mess, data);
	char allow = '1';
	bool find = false;
	for (auto& user : users_)
	{
		if (mess[0] == user.getLogin())
		{
			find = true;
			break;
		}
	}
	if (mess[0] != selectedUser_->getLogin() && (find || mess[0] == "all"))
	{
		messages_.push_back(Message(mess[0], selectedUser_->getLogin(), mess[1], selectedUser_->getName()));
		ans[0] = '1';
	}
	else 
	strcpy(ans, "0Recipient is not found!");
}

void Chat::usersList(const int& connection, std::shared_ptr<User>& selectedUser_)
{
	const char end[] = "!!END!!";
	char buf[MESSAGE_LENGTH]{};
	char message[MESSAGE_LENGTH]{};
	for (auto& user : users_)
	{
		if (user.getLogin() == selectedUser_->getLogin())
			strcpy(buf, " (this account)");
		strcpy(message, "Login : '");
		strcat(message, user.getLogin().c_str());
		strcat(message, "' ");
		strcat(message, buf);
		bzero(buf, sizeof(buf));
		buf[0] = strlen(message);
		strcat(buf, message);
		write(connection, buf, sizeof(buf));
	}
	bzero(buf, sizeof(buf));
	buf[0] = strlen(end);
	strcat(buf, end);
	write(connection, buf , sizeof(buf));
}

void Chat::dispChat(const int& connection, std::shared_ptr<User>& selectedUser_)
{
	char message[MESSAGE_LENGTH]{};
	char buf[MESSAGE_LENGTH]{};
	const char end[] = "!!END!!";
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
			bzero(buf, sizeof(buf));
			buf[0] = strlen(message);
			strcat(buf, message);
			write(connection, buf, sizeof(buf));
		}
	}
	bzero(buf, sizeof(buf));
	buf[0] = strlen(end);
	strcat(buf, end);
	write(connection, buf , sizeof(buf));
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
int Chat::getUsersSize()
{
	return users_.size();
}
