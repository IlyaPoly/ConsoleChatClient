#include <iostream>
#include <stdlib.h>
#include <string>
#include "Chat.h"
#include "Connect.h"
#include <vector>
#include <thread>


 void work(const int connection, Chat& chat)
 {
    char message[MESSAGE_LENGTH]{};
	char act;
	char ans[MESSAGE_LENGTH]{};
    std::shared_ptr<User> selectedUser_ = nullptr;
	while(1)
	{
	read(connection, message, sizeof(message));
	act = message[0];
	strcpy(message,message+1);
	switch (act - 48)
	{
		case 1:
		{
			chat.regChat(message, ans);  //?
			break;
		}
		case 2:
		{
			chat.signIn(message, ans, selectedUser_);
			break;
		}
		case 3:
		{
			chat.writeMessage(message, ans, selectedUser_);
			break;
		}
		case 4:
		{
			chat.usersList(connection, selectedUser_);
			break;
		}
		case 5:
		{
			chat.dispChat(connection, selectedUser_);
			break;
		}
		case 0:
		{
			if (selectedUser_)
			{
				selectedUser_ = nullptr;
				ans[0] = '1';
				break;
			} else return;
		}
		default:
			break;
		}
		if (act != '4' && act != '5')
		write(connection, ans, sizeof(ans));
		bzero(message, sizeof(message));
		bzero(ans, sizeof(ans));
	}
 }

void con(Chat& chat)
{
    std::vector<std::thread> working;
    std::vector<struct sockaddr_in> sockClients;
    struct sockaddr_in client;
    ConnectTCP myConnect;
    socklen_t length;
    int connection, connection_status;
    int sockert_file_descriptor = myConnect.connect();
    connection_status = listen(sockert_file_descriptor, 20);
        length = sizeof(client);
    while (1)
    {
        connection = accept(sockert_file_descriptor, (struct sockaddr*)&client, &length);
        sockClients.push_back(client);
        if (connection == -1)
            std::cout << "Connection failed!";
		else
        working.push_back(std::thread(work, connection, std::ref(chat)));
    }
}



int main()
{
	Chat chat;
	chat.chatOn();
    con(chat);
}
