#pragma once

#include <iostream>
#include "../Server.hpp"
#include "Client.hpp"

class Server;
class Client;

class Authentications {
	public :
		Authentications();
		void Pass(Server* serverObj, Client* clientObj, int clientSocket, std::vector<std::string> & words);
		void User( Client* clientObj, int clientSocket, std::vector<std::string> & words);
		void Nick(Server* serverObj, Client* clientObj, int clientSocket, std::vector<std::string> & words);
		void Login(Server* serverObj, Client* clientObj, int clientSocket, std::vector<std::string> & words);
		~Authentications();
};