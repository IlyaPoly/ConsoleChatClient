#pragma once
#include "User.h"
#include "Message.h"
#include <vector>
#include <memory>
class Chat
{
	const std::string div = "*****************\n";
	std::vector<User> users_;
	std::vector<Message> messages_;
	std::shared_ptr<User> selectedUser_ = nullptr;
	bool chatStatus_ = false;
public:
	void chatOn();
	void chatOff();
	const bool getStatus() const;
	const std::shared_ptr<User>& getSelectedUser() const;
	void mainMenu(const int& connection, bool& on);
	void regChat(const int& connection);
	void signIn(const int& connection);
	void userMenu(const int& connection);
	void writeMessage(const int& connection);
	void usersList(const int& connection);
	void signOut(const int& connection);
	void dispChat(const int& connection);
	template<typename T>
	void load(std::string file_path, std::vector<T>& mass);
	template <typename T>
	void upload(std::string file_path, std::vector<T>& mass);
};
