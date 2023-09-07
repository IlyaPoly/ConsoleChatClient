all: consolechat

consolechat: main.o Chat.o Message.o User.o Connect.o
	g++ main.o Chat.o Message.o User.o Connect.o -o consolechat
main.o: main.cpp
	g++ -c -pthread main.cpp
Chat.o: Chat.cpp
	g++ -c Chat.cpp
Message.o: Message.cpp
	g++ -c Message.cpp
User.o: User.cpp
	g++ -c User.cpp
Connect.p: Connect.h
	g++ -c Connect.h
clean:
	rm -rf *.o
