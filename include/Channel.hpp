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
		std::string key;
		std::string topic;
		std::vector<std::string> modes;
		// std::string mode;//invite-topic-key-operator-limit
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
		std::vector<std::string> getModes();
		void setModes(std::string newMode);
		std::string getKey();
		void setKey(std::string newPassword);
		int getLimit();
		void setLimit(int newLimit);
		std::map<int, Client*> getOperators();
		void setOperator(int clientSocket, Client* newOperator);
		std::map<int, Client*> getClients();
		void addClient(Client* client);
		std::map<int, Client*> getInviteList();
		void addToInviteList(Client* newMember);
		~Channel();
};


#endif
