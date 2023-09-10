#include<iostream>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>


#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных
#define PORT 700 // Будем использовать этот номер порта
#define PORT2 30000

 class ConnectTCP
 {
    struct sockaddr_in serveraddress, client;
    socklen_t length;
    int sockert_file_descriptor, connection, bind_status, connection_status;
public:
    ConnectTCP()
    {
        sockert_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
        serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
        serveraddress.sin_port = htons(PORT);
        serveraddress.sin_family = AF_INET;
    
    }
    int connect()
    {
        bind_status = bind(sockert_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
        if(bind_status == -1)
        {
            std::cout << "Socket binding failed.!" << std::endl;
            exit(1);
        }
    return sockert_file_descriptor;
    }
 };

