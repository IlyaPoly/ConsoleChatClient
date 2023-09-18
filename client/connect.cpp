#include "connect.h"
int connect()
{
	int socket_file_descriptor, connection;
	struct sockaddr_in serveraddress;
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == -1) {
        std::cout << "Creation of Socket failed!" << std::endl;
        exit(1);
    }

    serveraddress.sin_addr.s_addr = inet_addr("158.69.104.15");
    //serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;
    connection = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if (connection == -1) {
        std::cout << "Connection with the server failed.!" << std::endl;
        exit(1);
    }
    return socket_file_descriptor;
}
