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
		int limit;
    	std::map<int, Client*> clients;
		std::map<int, Client*> operators;
		std::map<int, Client*> inviteList;
	public:
		Channel();
		Channel(std::string& name);
		int	find_mode(std::string mode);
		int getLimit();
		std::string getName() ;
		std::string getTopic();
		std::string getKey();
		std::vector<std::string> getModes();
		std::map<int, Client*> getOperators();
		std::map<int, Client*> getClients();
		std::map<int, Client*> getInviteList();
		void setTopic(std::string newTopic);
		void setModes(std::string newMode);
		void deleteMode(std::string newMode);
		void setKey(std::string newPassword);
		void setLimit(int newLimit);
		void setOperator(int clientSocket, Client* newOperator);
		void addClient(Client* client);
		void addToInviteList(Client* newMember);
		~Channel();
};


#endif
