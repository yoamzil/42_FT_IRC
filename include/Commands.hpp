#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>

class Channel;
class Client;

class Commands
{
	public:
		Commands();
        void kick(int isAdmin, int toKick, Channel* channel);
        void invite(int isAdmin, Client* newMember, Channel* channel);
        void topic(int isAdmin, std::string newTopic, Channel* channel);
        void mode(Channel* channel, std::vector<std::string> words);
		~Commands();
};


#endif
