#pragma once
#include <iostream>
#include <unistd.h>
#include <string.h>
#define MESSAGE_LENGTH 1024


void regChat(char* message);
void mainMenu(const int& connection, bool& in);
void signIn(char* message);
void signOut(bool& in);
void writeMessage(char* message);
void outInfo(const int& connection, char* message);
void userMenu(const int& connection, bool& in);
