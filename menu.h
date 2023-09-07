#pragma once
#include <iostream>
#include <unistd.h>
#include <string.h>
#define MESSAGE_LENGTH 1024

void regChat(const int& connection);
void mainMenu(const int& connection, bool& in);
void signIn(const int& connection, bool& in);
void signOut(bool& in);
void writeMessage(const int& connection);
void outInfo(const int& connection, const int& size);
void userMenu(const int& connection, bool& in);
