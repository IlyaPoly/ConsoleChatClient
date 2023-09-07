#include <iostream>
#include <stdlib.h>
#include <string>
#include "Chat.h"
#include "Connect.h"
#include <vector>
#include <thread>

void con(Chat& chat)
{
    std::vector<std::thread> clients;
    
    struct sockaddr_in client;
    ConnectTCP myConnect;
    socklen_t length;
    int connection, connection_status;
    int sockert_file_descriptor = myConnect.connect();
    while (1)
    {
        connection_status = listen(sockert_file_descriptor, 5);
        length = sizeof(client);
        connection = accept(sockert_file_descriptor, (struct sockaddr*)&client, &length);
        if (connection == -1)
            std::cout << "Connection failed!";
        clients.push_back(std::thread(work, std::ref(connection), std::ref(chat)));
    }
}


int main()
{
	Chat chat;
	chat.chatOn();
    std::thread f(con,std::ref(chat));
    //f.join();
    while (1)
    {

    }
    return 0;
}
