#pragma once

#include <iostream>
#include "Client.hpp"
#include "../Server.hpp"
#include "../Commands.hpp"

class Server;
class Client;
class Commande;

class ModeUser : public Commands {
	public :
		ModeUser();
		void Join(Server* serverObj, Client* clientObj, int clientSocket, std::vector<std::string> & words);
		void Part(Server* serverObj, Client* clientObj, int clientSocket, std::vector<std::string> & words);
		void Mode(Server* serverObj, Client* clientObj, int clientSocket, std::vector<std::string> & words);
		void Invite(Server* serverObj, Client* clientObj, int clientSocket, std::vector<std::string> & words);
		void Topic(Server* serverObj, Client* clientObj, int clientSocket, std::vector<std::string> & words);
		void Kick(Server* serverObj, Client* clientObj, int clientSocket, std::vector<std::string> & words);
		void Commande(Server* serverObj, Client* clientObj, int clientSocket, std::vector<std::string> & words);
		bool Check_Join();
		bool Check_Part();
		bool Check_Topic();
		bool Check_Invite();
		bool Check_Mode();
		bool Check_Kick();
		bool check_Comande(std::vector<std::string> & words);
		~ModeUser();
};