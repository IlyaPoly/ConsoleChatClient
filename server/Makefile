all: consolechat

consolechat: main.o Chat.o Message.o User.o Connect.o
	g++ -pthread main.o Chat.o Message.o User.o Connect.o -o consolechat
main.o: main.cpp
	g++ -g -c -pthread main.cpp
Chat.o: Chat.cpp
	g++ -g -c Chat.cpp
Message.o: Message.cpp
	g++ -g -c Message.cpp
User.o: User.cpp
	g++ -g -c User.cpp
Connect.o: Connect.cpp
	g++ -g -c Connect.cpp
clean:
	rm -rf *.o
