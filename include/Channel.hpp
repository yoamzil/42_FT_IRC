#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../include/Server.hpp"
#include "../include/Client.hpp"

class Client;
class Server;

class Channel
{
	public:
		std::string password;
		std::string name;
    	std::map<int, Client*> clients;

	public:
		Channel();
		Channel(std::string& name);
		// Channel & operator=(Channel const &src);
		// Channel(Channel const &src);
		void addClient(Client* client);
		// std::string getChannelName();
		// void removeClient(int clientFd);
		// Client* getClient(int clientFd);
		std::map<int, Client*> getClients();
		std::string getName() ;
		~Channel();

};


#endif