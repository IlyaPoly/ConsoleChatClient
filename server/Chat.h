#pragma once
#include "User.h"
#include "Message.h"
#include <vector>
#include <memory>
#include <libpq-fe.h>

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
	void regChat(const char* data, char* ans, PGconn *conn, PGresult *res);
	void signIn(const char* data, char* ans, char *id, PGconn *conn, PGresult *res);
	void writeMessage(const char* data, char* ans, char *id, PGconn *conn, PGresult *res);
	void usersList(const int& connection, char *id, PGconn *conn, PGresult *res);
	void dispChat(const int& connection, char *id, PGconn *conn, PGresult *res);
	void messToParam(std::string* param, const char* data, int countParam);
};
