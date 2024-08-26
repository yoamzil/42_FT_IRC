#include "../../include/client_channel/Authentications.hpp"

Authentications::Authentications() {
}

Authentications::~Authentications() {
}


void Authentications::Pass(Server* serverObj, Client* clientObj, int clientSocket, std::vector<std::string> & words) {
	if (words[0] == "PASS" && words.size() == 2)
	{
		if (words[1] == serverObj->getPassword())
		{
			clientObj->setCorrectPass();
			// client.setPassword(password);
			std::cout << clientSocket << " : password is correct" << std::endl;
		}
		else if (words[1] != serverObj->getPassword())
		{
			std::cout << clientSocket << " : password  incorrect" << std::endl;
		}
	}
}

void Authentications::Nick(Client* clientObj, int clientSocket, std::vector<std::string> & words) {
	if (words[0] == "NICK" && clientObj->correctpass() == 1 && !words[1].empty() && words.size() == 2)
	{
		if (clientObj->getNickname() != words[1] && clientObj->getNickname().empty()){
			clientObj->setNikename(words[1]);
			clientObj->setStatus(0);
			std::cout <<  "Create a Nick : " << words[1] << " , Id : " << clientSocket << " correct ID : " << clientObj->correctpass()  << std::endl;
			return ;
		}
		else if (clientObj->getNickname() != words[1] && !clientObj->getNickname().empty())
		{
			std::string str = clientObj->getNickname();
			clientObj->setNikename(words[1]);
			clientObj->sendMessage(clientSocket , ":" + str + "!" + clientObj->getUsername() + "@localhost NICK :" + clientObj->getNickname() + "\r\n");
			std::cout <<  "change a Nick  : " << words[1] << " , Id : " << clientObj->getFd() << " correct ID : " << clientObj->correctpass()  << std::endl;
			return;
		}
		else if (clientObj->getNickname() == words[1])
		{
			std::cout <<  "Nick already exist : " << words[1] <<  " , Id : " << clientSocket << " correct ID : " << clientObj->correctpass()  << std::endl;
			clientObj->sendMessage(clientSocket , ": 433 * " + clientObj->getNickname() + " :Nickname is already in use.\r\n");
			return;
		}
	}
}


void Authentications::User(Client* clientObj, int clientSocket, std::vector<std::string> & words) {
	if (words[0] == "USER" && clientObj->correctpass() == 1 && !words[1].empty())
	{
		if (words.size() == 5)
		{
			clientObj->setUsername(words[1]);
			clientObj->setLocation(words[3]);
			std::cout <<  "Create a USER : " << words[1] << " , Id : " << clientSocket << " correct ID : " << clientObj->correctpass() << std::endl;
		}
		else if (words.size() != 5)
		{
			std::cout << "please enter the correct USER" << std::endl;
		}
		
	}
}


void Authentications::Login(Server* serverObj, Client* clientObj, int clientSocket, std::vector<std::string>& words) {
    if (words.empty())
		return; // Check for empty command

    typedef void (Authentications::*CommandFunc)(Client*, int, std::vector<std::string>&);
    typedef void (Authentications::*CommandPass)(Server*, Client*, int, std::vector<std::string>&);

    std::map<std::string, CommandPass> commands0;
    std::map<std::string, CommandFunc> commands1;
    commands0["PASS"] = &Authentications::Pass;
    commands1["USER"] = &Authentications::User;
    commands1["NICK"] = &Authentications::Nick;

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
	else {
        std::cout << "Unknown command: " << words[0] << std::endl;
    }
}