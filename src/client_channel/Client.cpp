#include "../../include/client_channel/Client.hpp"

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
    send(clientSocket, message.c_str(), message.size(), 0);
}

void Client::sendList(Server *serverObj , __unused int clientSocket, const std::string& channelName)
{
	std::map<std::string, Channel> currentChannels = serverObj->getChannels();
	std::map<std::string, Channel>::iterator it = currentChannels.find(channelName);
	if (it != currentChannels.end()) {
		Channel& channel = it->second;
		std::map<int, Client*> channelClients = channel.getClients();
		std::map<int, Client*> list_operator = channel.getOperators();

		for (std::map<int, Client*>::iterator clientI = channelClients.begin(); clientI != channelClients.end(); ++clientI) {
			Client* cliente = clientI->second;

			std::string msg;
			for (std::map<int, Client*>::iterator clientIt = channelClients.begin(); clientIt != channelClients.end(); ++clientIt) {
				Client* clien = clientIt->second;
				if (list_operator.begin()->second->getNickname() != clien->getNickname())
					msg = msg + " " + clien->getNickname();

			}
			if (!list_operator.empty())
			{
				std::string message1 = ": 353 " + serverObj->getClient()[cliente->getFd()]->getNickname() + " = " + channelName + " :@" + list_operator.begin()->second->getNickname()  + " " + msg + "\r\n";
				std::string message2 = ": 366 " + serverObj->getClient()[cliente->getFd()]->getNickname() + " " + channelName + " :End of /NAMES list.\r\n";
				send(cliente->getFd(), message1.c_str(), message1.size(), 0);
				send(cliente->getFd(), message2.c_str(), message2.size(), 0);
			}
			else if (list_operator.empty())
			{
				std::string message1 = ": 353 " + serverObj->getClient()[cliente->getFd()]->getNickname() + " = " + channelName + " :" + msg + "\r\n";
				std::string message2 = ": 366 " + serverObj->getClient()[cliente->getFd()]->getNickname() + " " + channelName + " :End of /NAMES list.\r\n";
				send(cliente->getFd(), message1.c_str(), message1.size(), 0);
				send(cliente->getFd(), message2.c_str(), message2.size(), 0);
			}

			// }
		}
	}
	
}

void Client::LeaveSendList(Server *serverObj ,  __unused int clientSocket, const std::string& channelName)
{
	std::map<std::string, Channel> currentChannels = serverObj->getChannels();
	std::map<std::string, Channel>::iterator it = currentChannels.find(channelName);
	if (it != currentChannels.end())
	{
		Channel& channel = it->second;
		std::map<int, Client*> channelClients = channel.getClients();
		std::map<int, Client*> list_operator = channel.getOperators();
		for (std::map<int, Client*>::iterator clientI = channelClients.begin(); clientI != channelClients.end(); ++clientI) {
			Client* cliente = clientI->second;

			std::string msg;
			for (std::map<int, Client*>::iterator clientIt = channelClients.begin(); clientIt != channelClients.end(); ++clientIt) {
				Client* clien = clientIt->second;
				if (list_operator.begin()->second->getNickname() != clien->getNickname())
					msg = msg + " " + clien->getNickname();

			}
			if (!list_operator.empty())
			{
				std::string message1 = ": 353 " + serverObj->getClient()[cliente->getFd()]->getNickname() + " = " + channelName + " :@" + list_operator.begin()->second->getNickname()  + " " + msg + "\r\n";
				std::string message2 = ": 366 " + serverObj->getClient()[cliente->getFd()]->getNickname() + " " + channelName + " :End of /NAMES list.\r\n";
				send(cliente->getFd(), message1.c_str(), message1.size(), 0);
				send(cliente->getFd(), message2.c_str(), message2.size(), 0);
			}
			else if (list_operator.empty())
			{
				std::string message1 = ": 353 " + serverObj->getClient()[cliente->getFd()]->getNickname() + " = " + channelName + " :" + msg + "\r\n";
				std::string message2 = ": 366 " + serverObj->getClient()[cliente->getFd()]->getNickname() + " " + channelName + " :End of /NAMES list.\r\n";
				send(cliente->getFd(), message1.c_str(), message1.size(), 0);
				send(cliente->getFd(), message2.c_str(), message2.size(), 0);
			}
		}
	}
}

void Client::leaveChannel(Server *serverObj, __unused int clientSocket, const std::string& channelName) {
	std::map<std::string, Channel> currentChannels = serverObj->getChannels();
	if (serverObj->getClient().find(clientSocket) != serverObj->getClient().end()) {
		Client* clientObj = serverObj->getClient()[clientSocket];
		
		if (currentChannels.find(channelName) != currentChannels.end()) {
			currentChannels[channelName].removeClient(clientObj);
			clientObj->eraseClientChannel(channelName);
			serverObj->setChannels(currentChannels);

			std::string message2 = ":" + clientObj->getNickname() + "!" + clientObj->getUsername() + clientObj->getLocation() + " PART " + channelName + " :Leaving\r\n";
			send(clientObj->getFd(), message2.c_str(), message2.size(), 0);
			// sendList(clientSocket , channelName);
			LeaveSendList(serverObj , clientSocket, channelName);
		}
	}
}


void Client::joinChannel(Server *serverObj, int clientSocket, const std::string& channelName, std::vector<std::string> words)
{
	bool joined = false;
    if (serverObj->mapClient.find(clientSocket) != serverObj->mapClient.end()) 
	{
        Client* clientObj = serverObj->mapClient[clientSocket];

        if (serverObj->channels.find(channelName) == serverObj->channels.end())
		{
            serverObj->channels[channelName] = Channel(channelName);
            serverObj->channels[channelName].setOperator(clientSocket, clientObj);
            serverObj->channels[channelName].setModes("n");
			std::string message = ":" + serverObj->mapClient[clientSocket]->getNickname() + "!" + serverObj->mapClient[clientSocket]->getUsername() + "@"  + serverObj->mapClient[clientSocket]->getLocation() + " JOIN " + channelName + " * :realname\r\n";
			send(clientSocket, message.c_str(), message.size(), 0);
			
			serverObj->channels[channelName].addClient(clientObj);
			clientObj->setChannelName(channelName);
			joined = true;
        }
		else
        {
            if (serverObj->channels[channelName].getModes().size() == 0)
            {
                std::string message = ":" + serverObj->mapClient[clientSocket]->getNickname() + "!" + serverObj->mapClient[clientSocket]->getUsername() + "@"  + serverObj->mapClient[clientSocket]->getLocation() + " JOIN " + channelName + " * :realname\r\n";
				send(clientSocket, message.c_str(), message.size(), 0);
                joined = true;
            }
            else 
            {
                int modesCount = 1;
                if (serverObj->channels[channelName].find_mode("i"))
                {
                    std::map<int, Client*> inviteList = serverObj->channels[channelName].getInviteList();
                    std::map<int, Client*>::iterator it = inviteList.find(clientSocket);
                    if (it != inviteList.end())
                        modesCount++;
                    else
                    {
                        std::string message = ": 473 " + serverObj->mapClient[clientSocket]->getNickname() + " " + channelName + " :Cannot join channel (+i)\r\n";
                        send(clientSocket, message.c_str(), message.size(), 0);
                    }
                }
                if (serverObj->channels[channelName].find_mode("l"))
                {
                    int clientsCnt = serverObj->channels[channelName].getClients().size();
                    if (clientsCnt < serverObj->channels[channelName].getLimit())
                        modesCount++;
                    else
                    {
                        std::string message = ": 471 " + serverObj->mapClient[clientSocket]->getNickname() + " " + channelName + " :Cannot join channel (+l)\r\n";
                        send(clientSocket, message.c_str(), message.size(), 0);
                    }
                }
                if (serverObj->channels[channelName].find_mode("k"))
                {
                    if (serverObj->channels[channelName].find_mode("i"))
                        modesCount++;
                    else
                    {
                        if (words[2] == serverObj->channels[channelName].getKey())
                            modesCount++;
                        else
                        {
                            std::string message = ": 475 " + serverObj->mapClient[clientSocket]->getNickname() + " " + channelName + " :Cannot join channel (+k)\r\n";
                            send(clientSocket, message.c_str(), message.size(), 0);
                        }
                    }
                }
                if (serverObj->channels[channelName].find_mode("t"))
                        modesCount++;
                int modes = serverObj->channels[channelName].getModes().size();
                
                if (modesCount == modes)
                {
                    std::vector<std::string> channel_modes = serverObj->channels[channelName].getModes();
                   	std::string message = ":" + serverObj->mapClient[clientSocket]->getNickname() + "!" + serverObj->mapClient[clientSocket]->getUsername() + "@"  + serverObj->mapClient[clientSocket]->getLocation() + " JOIN " + channelName + " * :realname\r\n";
					send(clientSocket, message.c_str(), message.size(), 0);
                    serverObj->channels[channelName].addClient(clientObj);
                    clientObj->setChannelName(channelName);
                    joined = true;
                }
            }//
        }
	}
   	if (joined)
   	{
        int channelSize = serverObj->channels[channelName].getClients().size();
        if (channelSize > 1)
        {
            std::map<int, Client*> members = serverObj->channels[channelName].getClients();
            for (std::map<int, Client*>::iterator it = members.begin(); it != members.end(); ++it) 
            {
                std::string message = ":" + members[clientSocket]->getNickname() + "!" + members[clientSocket]->getUsername() + "@" + members[clientSocket]->getLocation() + " JOIN " + channelName + "\r\n";
                send(it->first, message.c_str(), message.size(), 0);
            }
        }
       	sendList(serverObj, clientSocket, channelName);
   	}
}


int userFd(Server* serverObj, const std::string& client)
{
   	std::map<int, Client*>::iterator toKickIt;
   	for (toKickIt = serverObj->mapClient.begin(); toKickIt != serverObj->mapClient.end(); toKickIt++)
   	{
  		if (toKickIt->second->getNickname() == client)
  		{
 			return (toKickIt->first);
 			break;
  		}
   	}
    return (0);
}

void Client::broadcastMessage(Server *serverObj, const std::string& channelName, const std::string& message, int clientSocket) {
	std::string  msg = ":" + serverObj->getClient()[clientSocket]->getNickname() + "!" + serverObj->getClient()[clientSocket]->getUsername() + "@" + serverObj->getClient()[clientSocket]->getLocation() + " " + message + "\r\n";
    std::map<std::string, Channel> currentChannels = serverObj->getChannels();
	std::map<std::string, Channel>::iterator it = currentChannels.find(channelName);
	std::vector<std::string> channelNames = serverObj->getClient()[clientSocket]->getChannelName();
    if (it != currentChannels.end()) {
        Channel& channel = it->second;
        std::map<int, Client*> channelClients = channel.getClients();
		if (channelName[0] == '#')
		{
			for (std::map<int, Client*>::iterator clientIt = channelClients.begin(); clientIt != channelClients.end(); ++clientIt) {
				Client* cliente = clientIt->second;
				for (std::vector<std::string>::iterator nt = channelNames.begin(); nt != channelNames.end(); ++nt)
				{
					if (*nt == channelName)
					{
						if (cliente->getFd() != clientSocket)
							cliente->sendMessage(cliente->getFd() , msg);
					}
				}
			}
		}
    }
	if (channelName[0] != '#')
	{
		int reciever = userFd(serverObj, channelName);
		if (reciever != 0)
			sendMessage(reciever , msg);
	}
}

void Client::handleMessage(Server* serverObj, int clientSocket, const std::string& message) {
    Client* clientObj;
    std::vector<std::string> input;
	Authentications auth;
	ModeUser command;
    
    // Check if the client exists
	if (message.empty()) 
		return ;
    if (serverObj->getClient().find(clientSocket) != serverObj->getClient().end()) {
        clientObj = serverObj->getClient()[clientSocket];
    }

    if (message.find("\r\n")) {
        std::vector<std::string> words;
        std::string delimiter = "\r\n";
        input = clientObj->splitString(message, delimiter);

        if (input.size() > 1) {
            for (size_t i = 0; i < input.size() - 1; ++i) {
                words = clientObj->splitString1(input[i]);
                auth.Login(serverObj, clientObj, clientSocket, words);
            }
        } else {
            words = clientObj->splitString1(input[0]);
            auth.Login(serverObj, clientObj, clientSocket, words);
        }

        if (clientObj->correctpass() == 1 && !clientObj->getNickname().empty() && !clientObj->getUsername().empty() && clientObj->getStatus() == 0) {
            clientObj->setFd(clientSocket);
            clientObj->setAuthentication();
        }

        if (clientObj->getAuthentication() == 1)
		{
			if (clientObj->getStatus() == 1)
			{
				if (words[0][0] != '/')
					command.Commande(serverObj, clientObj, clientSocket, words);
				else
					clientObj->sendMessage(clientSocket, ":server_name 421 " + clientObj->getNickname() + " " + words[0] + " :Unknown command\r\n");
			}
			if (!command.check_Comande(words) && clientObj->getStatus() == 1)
			{
                if (!message.find("PRIVMSG") && words.size() > 2)
				{
                    clientObj->broadcastMessage(serverObj, words[1], message, clientSocket);
                }
			}
        }
        if (clientObj->getAuthentication() == 0 && clientObj->getStatus() == 0 && words[0] != "NICK" && words[0] != "USER" && words[0] != "PASS")
		{
            std::string message = ":irc.example.com 451 " + clientObj->getNickname() + " :You have not registered\r\n";
            clientObj->sendMessage(clientSocket, message);
        }
        if (clientObj->getAuthentication() == 1 && clientObj->getStatus() == 0)
		{
            std::string message = ": 001 " + clientObj->getNickname() + " :Welcome to the IRC Network, " + clientObj->getNickname() + "!" + clientObj->getUsername() + "@" + serverObj->getClient()[clientSocket]->getLocation() + "\r\n";
            clientObj->sendMessage(clientSocket, message);
            clientObj->setStatus(1);
        }
    }
}