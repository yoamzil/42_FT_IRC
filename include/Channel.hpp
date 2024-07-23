#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../include/Server.hpp"
#include "../include/Client.hpp"

class Client;
class Server;

class Channel
{
	public:
		std::string name;
		std::string password;
		std::string topic;
		std::string mode;//invite-topic-key-operator-limit
		int limit;//number of members
    	std::map<int, Client*> clients;
		std::map<int, Client*> operators;
		std::map<int, Client*> inviteList;
	public:
		Channel();
		Channel(std::string& name);
		std::string getName() ;
		std::string getTopic();
		void setTopic(std::string newTopic);
		std::string getMode();
		void setMode(std::string newMode);
		std::string getPassword();
		void setPassword(std::string newPassword);
		int getLimit();
		void setLimit(int newLimit);
		std::map<int, Client*> getOperators();
		void setOperator(Client* newOperator);
		std::map<int, Client*> getClients();
		void addClient(Client* client);
		std::map<int, Client*> getInviteList();
		void addToInviteList();
		~Channel();
		vo
};
		// Channel & operator=(Channel const &src);
		// Channel(Channel const &src);
		// std::string getChannelName();
		// void removeClient(int clientFd);
		// Client* getClient(int clientFd);


#endif