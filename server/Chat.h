#pragma once
#include "User.h"
#include "Message.h"
#include <vector>
#include <memory>

#define FILEUSERS "users.txt"
#define FILEMESS "messages.txt"
#define MESSAGE_LENGTH 1024


class Chat
{
	std::vector<User> users_;
	std::vector<Message> messages_;
	bool chatStatus_ = false;
public:
	void chatOn();
	void chatOff();
	const bool getStatus() const;
	const std::shared_ptr<User>& getSelectedUser() const;
	void regChat(const char* data, char* ans);
	void signIn(const char* data, char* ans, std::shared_ptr<User>& selectedUser_);
	void writeMessage(const char* data, char* ans, std::shared_ptr<User>& selectedUser_);
	void usersList(const int& connection, std::shared_ptr<User>& selectedUser_);
	void dispChat(const int& connection, std::shared_ptr<User>& selectedUser_);
	template<typename T>
	void load(std::string file_path, std::vector<T>& mass);
	template <typename T>
	void upload(std::string file_path, std::vector<T>& mass);
	void messToParam(std::string* param, const char* data, int countParam);
	int getUsersSize();
};
