#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <cstdlib>
#include <vector>

class Channel;
class Client;
class Server;

class Commands
{
    
	public:
		Commands();
        void kick(Server* serverObj, int isAdmin, int toKick, Channel* channel);
        void invite(Server* serverObj, int isAdmin, std::vector<std::string> words);
        void topic(Server* serverObj, int isAdmin, std::vector<std::string> words);
        void mode(int isAdmin, Channel* channel, std::vector<std::string> words);
		~Commands();
};


#endif
