#include "../include/Client.hpp"

Client::Client()
{
	this->_fd = 0;
	this->correctPassword = 0;
	this->_authentication = 0;
	this->status = 0;
}

void	Client::setFd(int fd)
{
	this->_fd = fd;
}

void Client::setUsername(std::string userName) 
{
	this->username = userName;
}

void Client::setNikename(std::string nikeName) 
{
	this->nickname = nikeName;
}

std::string Client::getUsername()
{
	return (this->username);
}

std::string Client::getNickname()
{
	return (this->nickname);
}

Client::~Client()
{

}

int Client::getFd()
{
	return (this->_fd);
}

void	Client::setCorrectPass()
{
	this->correctPassword = 1;
}

bool 	Client::correctpass()
{
	return (this->correctPassword);
}

void	Client::setAuthentication()
{
	this->_authentication = 1;
}

bool 	Client::getAuthentication()
{
	return (this->_authentication);
}

std::string  Client::getLocation() const
{
	return (this->_Location);
}
std::vector<std::string>  Client::getChannelName() const
{
	return (_channelNames);
}

void Client::setChannelName(const std::string& channelName)
{
	_channelNames.push_back(channelName);
}

void Client::setLocation(const std::string& Location)
{
	this->_Location = Location;
}

bool Client::getStatus()
{
	return status;
}

void Client::setStatus(int status)
{
	this->status = status;
}

void Client::eraseClientChannel(const std::string& channelName)
{
	std::vector<std::string>::iterator it = std::find(_channelNames.begin(), _channelNames.end(), channelName);
	if (it != _channelNames.end())
		_channelNames.erase(it);
}

std::vector<std::string> Client::splitString(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + delimiter.length(); // Move past the delimiter
        end = str.find(delimiter, start);
    }

    // Add the last part
    result.push_back(str.substr(start));

    return result;
}

std::vector<std::string> Client::splitString1(const std::string& str) {
    std::istringstream iss(str);
    std::vector<std::string> result;
    std::string word;

    while (iss >> word) {
        result.push_back(word);
    }
    return result;
}

void Client::sendMessage(int clientSocket, const std::string& message) {
	// std::cout  << "id : " <<  clientSocket << "message : "  << message << std::endl;
    send(clientSocket, message.c_str(), message.size(), 0);
}

void Client::sendList(Server *serverObj , __unused int clientSocket, const std::string& channelName)
{
	std::map<std::string, Channel>::iterator it = serverObj->channels.find(channelName);
	if (it != serverObj->channels.end()) {
		Channel& channel = it->second;
		std::map<int, Client*> channelClients = channel.getClients();
		for (std::map<int, Client*>::iterator clientI = channelClients.begin(); clientI != channelClients.end(); ++clientI) {
			Client* cliente = clientI->second;

			std::string msg;
			for (std::map<int, Client*>::iterator clientIt = channelClients.begin(); clientIt != channelClients.end(); ++clientIt) {
				Client* clien = clientIt->second;
				if (cliente->getFd() != clien->getFd())
					msg = msg + " " + clien->getNickname();

			}
			std::string message1 = ": 353 " + serverObj->client[cliente->getFd()]->getNickname() + " = " + channelName + " :@" + serverObj->client[cliente->getFd()]->getNickname() + " " + msg + "\r\n";
			std::string message2 = ": 366 " + serverObj->client[cliente->getFd()]->getNickname() + " " + channelName + " :End of /NAMES list.\r\n";
			send(cliente->getFd(), message1.c_str(), message1.size(), 0);
			send(cliente->getFd(), message2.c_str(), message2.size(), 0);
		}
	}
}

void Client::LeaveSendList(Server *serverObj , __unused int clientSocket, const std::string& channelName)
{
	std::map<std::string, Channel>::iterator it = serverObj->channels.find(channelName);
	if (it != serverObj->channels.end()) {
		Channel& channel = it->second;
		std::map<int, Client*> channelClients = channel.getClients();
		for (std::map<int, Client*>::iterator clientI = channelClients.begin(); clientI != channelClients.end(); ++clientI) {
			Client* cliente = clientI->second;

			std::string msg;
			for (std::map<int, Client*>::iterator clientIt = channelClients.begin(); clientIt != channelClients.end(); ++clientIt) {
				Client* clien = clientIt->second;
				if (cliente->getFd() != clien->getFd())
					msg = msg + " " + clien->getNickname();

			}
			std::string message1 = ": 353 " + serverObj->client[cliente->getFd()]->getNickname() + " = " + channelName + " :@" + serverObj->client[cliente->getFd()]->getNickname() + " " + msg + "\r\n";
			std::string message2 = ": 366 " + serverObj->client[cliente->getFd()]->getNickname() + " " + channelName + " :End of /NAMES list.\r\n";
			send(cliente->getFd(), message1.c_str(), message1.size(), 0);
			send(cliente->getFd(), message2.c_str(), message2.size(), 0);
		}
	}
}

void Client::leaveChannel(Server *serverObj, int clientSocket, const std::string& channelName) {
	if (serverObj->client.find(clientSocket) != serverObj->client.end()) {
		Client* clientObj = serverObj->client[clientSocket];
		if (serverObj->channels.find(channelName) != serverObj->channels.end()) {
			serverObj->channels[channelName].removeClient(clientObj);
			clientObj->eraseClientChannel(channelName);

			std::string message2 = ":" + clientObj->getNickname() + "!" + clientObj->getUsername() + clientObj->getLocation() + " PART " + channelName + " :Leaving\r\n";
			send(clientObj->getFd(), message2.c_str(), message2.size(), 0);
			// sendList(clientSocket , channelName);
			LeaveSendList(serverObj , clientSocket, channelName);
		}
	}
}

void Client::joinChannel(Server *serverObj, int clientSocket, const std::string& channelName) {
    if (serverObj->client.find(clientSocket) != serverObj->client.end()) {
        Client* clientObj = serverObj->client[clientSocket];

        // Check if the channel exists, if not, create it
        if (serverObj->channels.find(channelName) == serverObj->channels.end()) {
            serverObj->channels[channelName] = Channel(channelName);
        }
        serverObj->channels[channelName].addClient(clientObj);
		clientObj->setChannelName(channelName);
		std::string message = ":" + serverObj->client[clientSocket]->getNickname() + "!" + serverObj->client[clientSocket]->getUsername() + "@"  + serverObj->client[clientSocket]->getLocation() + " JOIN " + channelName + " * :realname\r\n";
		send(clientSocket, message.c_str(), message.size(), 0);
	}
	sendList(serverObj , clientSocket , channelName);
}






void Client::broadcastMessage(Server *serverObj, const std::string& channelName, const std::string& message, int clientSocket) {
    std::map<std::string, Channel>::iterator it = serverObj->channels.find(channelName);
	std::vector<std::string> channelNames = serverObj->client[clientSocket]->getChannelName();
    if (it != serverObj->channels.end()) {
        Channel& channel = it->second;
        std::map<int, Client*> channelClients = channel.getClients();
		// std::string message1 = "353 " + client[clientSocket]->getNickname() + " = " + channelName + " :" + client[clientSocket]->getNickname() + "!" + client[clientSocket]->getUsername() + "@localhost\r\n";
		// std::string message2 = "366 " + client[clientSocket]->getNickname() + " " + channelName + " :End of /NAMES list.\r\n";
		std::string  msg = ":" + serverObj->client[clientSocket]->getNickname() + "!" + serverObj->client[clientSocket]->getUsername() + "@" + serverObj->client[clientSocket]->getLocation() + " " + message + "\r\n";
        for (std::map<int, Client*>::iterator clientIt = channelClients.begin(); clientIt != channelClients.end(); ++clientIt) {
            Client* cliente = clientIt->second;
			for (std::vector<std::string>::iterator nt = channelNames.begin(); nt != channelNames.end(); ++nt)
			{
				if (*nt == channelName)
				{
					if (cliente->getFd() != clientSocket)
						cliente->sendMessage(cliente->getFd() , msg);
					std::cout << "channelName : " << *nt << std::endl;
				}
			}
        }
    }
}

void	Client::authentication(Server* serverObj, Client* clientObj, int clientSocket, std::vector<std::string> & words)
{
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
	else if (words[0] == "NICK" && clientObj->correctpass() == 1 && !words[1].empty() && words.size() == 2)
	{
		// int i = 0;
		for (std::map<int, Client*>::iterator clientIt = serverObj->client.begin(); clientIt != serverObj->client.end(); ++clientIt) {
        	Client* client = clientIt->second;
			if (client->getNickname() != words[1] && client->getNickname().empty()){
				// std::string str = client->getNickname();
				client->setNikename(words[1]);
				// clientObj->setNikename(words[1]);
				// sendMessage(clientSocket , ":" +  clientObj->getNickname()  + "!" + client->getUsername() + "@localhost NICK :" + client->getNickname() + "\r\n");
				clientObj->setStatus(0);

				std::cout <<  "Create a Nick : " << words[1] << " , Id : " << clientSocket << " correct ID : " << clientObj->correctpass()  << std::endl;
				return ;
			}
			else if (client->getNickname() != words[1] && !clientObj->getNickname().empty())
			{
				std::string str = client->getNickname();
				client->setNikename(words[1]);
				clientObj->setNikename(words[1]);
				clientObj->sendMessage(clientSocket , ":" + str + "!" + client->getUsername() + "@localhost NICK :" + client->getNickname() + "\r\n");
				std::cout <<  "change a Nick  : " << words[1] << " , Id : " << clientObj->getFd() << " correct ID : " << client->correctpass()  << std::endl;
				return;
			}
			else if (client->getNickname() == words[1])
			{
				std::cout <<  "Nick already exist : " << words[1] <<  " , Id : " << clientSocket << " correct ID : " << clientObj->correctpass()  << std::endl;
				// 433 * hamza :Nickname is already in use.
				clientObj->sendMessage(clientSocket , ": 433 * " + client->getNickname() + " :Nickname is already in use.\r\n");
				// std::cout << "nick "  << client->getNickname() << " Nick already exist : " << words[1]  << "  -- > " << i << std::endl;
				return;
			}
		}
	}
	else if (words[0] == "USER" && clientObj->correctpass() == 1 && !words[1].empty())
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

void Client::handleMessage(Server* serverObj, int clientSocket, const std::string& message)
{
	Client* clientObj;
	
	std::vector<std::string> input;
	if (serverObj->client.find(clientSocket) != serverObj->client.end()) {

        clientObj = serverObj->client[clientSocket];
        // Use clientObj to interact with the client
    }
	if (message.find("\r\n"))
	{
		std::vector<std::string> words;
		std::string delimiter = "\r\n";
		input = clientObj->splitString(message, delimiter);
		if (input.size() > 1)
		{
			for (size_t i = 0; i < input.size() - 1; ++i) {
				words =clientObj->splitString1(input[i]);
				clientObj->authentication(serverObj, clientObj, clientSocket, words);
			}
		}
		else{
			words = clientObj->splitString1(input[0]);
			clientObj->authentication(serverObj, clientObj, clientSocket, words);
		}

		if (clientObj->correctpass() == 1 && clientObj->getNickname() != "\0" && clientObj->getUsername() != "\0" && clientObj->getStatus() == 0)
		{
			clientObj->setFd(clientSocket);
			clientObj->setAuthentication();
		}
		if (clientObj->getAuthentication() == 1)
		{

			if (words[0] == "JOIN" && !words[1].empty() && clientObj->getStatus() == 1)
			{

				if (words[1][0] == '#')
				{
					clientObj->joinChannel(serverObj, clientSocket, words[1]);
				}
				else if (words[1][0] != '#')
				{
					std::string message = ": 403 * " + clientObj->getNickname() + " " + words[1] + " :No such channel\r\n";
					clientObj->sendMessage(clientSocket, message);
				}
			}
			else if (words[0] == "PART" && !words[1].empty() && clientObj->getStatus() == 1)
			{
				if (words[1][0] == '#')
				{
					std::string message = ": remove\r\n";
					clientObj->leaveChannel(serverObj, clientSocket, words[1]);
				}
				else if (words[1][0] != '#')
				{
					std::string message = ": not remove\r\n";
				}
			}
			else if(words[0] != "JOIN" && clientObj->getStatus() == 1)
			{
				if (!message.find("PRIVMSG"))
				{
					clientObj->broadcastMessage(serverObj , words[1], message, clientSocket);
				}
				
			}
		}
		if (clientObj->getAuthentication() == 1 && clientObj->getStatus() == 0)
		{
			// :irc.example.com 001 dan :Welcome to the IRCcom Network, dan
			std::string message = ": 001 " + clientObj->getNickname() + " :Welcome to the IRC Network, " + clientObj->getNickname() + "!" + clientObj->getUsername() + "@" + serverObj->client[clientSocket]->getLocation() + "\r\n";
			clientObj->sendMessage(clientSocket, message); //"<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]"
			clientObj->setStatus(1);
		}
	}
}
