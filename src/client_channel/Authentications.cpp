#include "../../include/client_channel/Authentications.hpp"

Authentications::Authentications() {
}

Authentications::~Authentications() {
}


bool check_nick(std::string &nick) {
	if (nick.size() > 9) {
		return 1;
	}
	for (size_t i = 0; i < nick.size(); i++) {
		if (!std::isalnum(nick[i]) && nick[i] != '_' && nick[i] != '-') {
			return 1;
		}
	}
	return 0;
}


void Authentications::Pass(Server* serverObj, Client* clientObj, int clientSocket, std::vector<std::string> & words) {
	if (words[0] == "PASS" && words.size() == 2)
	{
		if (words[1] == serverObj->getPassword())
		{
			clientObj->setCorrectPass();
		}
		else if (words[1] != serverObj->getPassword())
		{
			std::string str = ":server_name 464 " + clientObj->getNickname() + " :Password incorrect\r\n";
			clientObj->sendMessage(clientSocket, str);
		}
	}
}

void Authentications::Nick(Server* serverObj, Client* clientObj, int clientSocket, std::vector<std::string> & words) {
	
	std::map<int, Client *> mapClient = serverObj->getClient();
	for (std::map<int, Client *>::iterator it = mapClient.begin(); it != mapClient.end(); ++it)
	{
		if (it->second->getNickname() == words[1]  && !words[1].empty() && words.size() == 2)
		{
			std::string str = ":server_name 433 * " + words[1] + " :Nickname is already in use\r\n";
			clientObj->sendMessage(clientSocket, str);
			return;
		}
	}
	if (words[0] == "NICK" && !words[1].empty() && words.size() == 2)
	{
		if (check_nick(words[1])) {
			std::string str = ":server_name 432 * " + words[1] + " :Erroneous nickname\r\n";
			clientObj->sendMessage(clientSocket, str);
			return;
		}
		else if (clientObj->getNickname() != words[1] && clientObj->getNickname().empty()){
			clientObj->setNikename(words[1]);
			std::cout <<  "Create a Nick : " << words[1] << " , Id : " << clientSocket << " correct ID : " << clientObj->correctpass()  << std::endl;
			return ;
		}
		else if (clientObj->getNickname() != words[1] && !clientObj->getNickname().empty())
		{
			std::string str = clientObj->getNickname();
			clientObj->setNikename(words[1]);
			clientObj->sendMessage(clientSocket , ":" + str + "!" + clientObj->getUsername() + "@localhost NICK :" + clientObj->getNickname() + "\r\n");
			return;
		}
	}
	else if (!words[1].empty() && words[0] == "NICK")
	{
		clientObj->sendMessage(clientSocket , ":server_name 431 * :No nickname given\r\n");
		return;
	}
}


void Authentications::User(Client* clientObj, int clientSocket, std::vector<std::string> & words) {
	if (words[0] == "USER" && !words[1].empty())
	{
		if (words.size() == 5 && clientObj->getUsername().empty())
		{
			clientObj->setUsername(words[1]);
			clientObj->setLocation(words[3]);
			std::cout <<  "Create a USER : " << words[1] << " , Id : " << clientSocket << " correct ID : " << clientObj->correctpass() << std::endl;
		}
		else if (words.size() == 5 && !clientObj->getUsername().empty())
		{
			clientObj->sendMessage(clientSocket , ":server_name 462 * :You may not reregister\r\n");
		}
		else if (words.size() != 5)
		{
			clientObj->sendMessage(clientSocket , ":server_name 461 * USER :Not enough parameters\r\n");
		}
		
	}
}


void Authentications::Login(Server* serverObj, Client* clientObj, int clientSocket, std::vector<std::string>& words) {
    if (words.empty())
		return;

    typedef void (Authentications::*CommandFunc)(Client*, int, std::vector<std::string>&);
    typedef void (Authentications::*CommandPass)(Server*, Client*, int, std::vector<std::string>&);

    std::map<std::string, CommandPass> commands0;
    std::map<std::string, CommandFunc> commands1;
    commands0["PASS"] = &Authentications::Pass;
    commands1["USER"] = &Authentications::User;
    commands0["NICK"] = &Authentications::Nick;

    std::map<std::string, CommandPass>::iterator it = commands0.find(words[0]);
    if (it != commands0.end()) {
        CommandPass Pass = it->second;
        (this->*Pass)(serverObj, clientObj, clientSocket, words);
    }
    std::map<std::string, CommandFunc>::iterator ite = commands1.find(words[0]);
	if (ite != commands1.end()) {
		CommandFunc func = ite->second;
		(this->*func)(clientObj, clientSocket, words);
	}
}