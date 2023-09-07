all: consolechatClient

consolechatClient: connect.o menu.o main.o
	g++ connect.o menu.o main.o  -o consolechatClient
main.o: main.cpp
	g++ -c main.cpp
menu.o: menu.cpp
	g++ -c menu.cpp
connect.o: connect.cpp
	g++ -c connect.cpp
clean:
	rm -rf *.o
