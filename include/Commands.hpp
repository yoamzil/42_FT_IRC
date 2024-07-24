#ifndef COMMANDS_HPP
#define COMMANDS_HPP

// #include "../include/Channel.hpp"

class Channel;

class Commands
{
	public:
		Commands();
        void kick(int isAdmin, int toKick, Channel* channel);
        // void invite(int isAdmin, int newMember, Channel* channel);
        // void topic(int isAdmin, std::string newTopic, Channel* channel);
        // void mode(int isAdmin, std::string newMode, Channel* channel);
		~Commands();
};


#endif
