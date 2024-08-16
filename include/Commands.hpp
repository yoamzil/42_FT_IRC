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
        void invite(int isAdmin, Client* newMember, Channel* channel);
        void topic(int isAdmin, std::string newTopic, Channel* channel);
        void mode(int isAdmin, Channel* channel, std::vector<std::string> words);
		~Commands();
};


#endif
