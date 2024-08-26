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
	// std::cout  << "id : " <<  clientSocket << "message : "  << message << std::endl;
    send(clientSocket, message.c_str(), message.size(), 0);
}

void Client::sendList(Server *serverObj , const std::string& channelName)
{
	std::map<std::string, Channel> currentChannels = serverObj->getChannels();
	std::map<std::string, Channel>::iterator it = currentChannels.find(channelName);
	if (it != currentChannels.end()) {
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
			std::string message1 = ": 353 " + serverObj->getClient()[cliente->getFd()]->getNickname() + " = " + channelName + " :@" + serverObj->getClient()[cliente->getFd()]->getNickname() + " " + msg + "\r\n";
			std::string message2 = ": 366 " + serverObj->getClient()[cliente->getFd()]->getNickname() + " " + channelName + " :End of /NAMES list.\r\n";
			send(cliente->getFd(), message1.c_str(), message1.size(), 0);
			send(cliente->getFd(), message2.c_str(), message2.size(), 0);
		}
	}
	
}

void Client::LeaveSendList(Server *serverObj , __unused int clientSocket, const std::string& channelName)
{
	std::map<std::string, Channel> currentChannels = serverObj->getChannels();
	std::map<std::string, Channel>::iterator it = currentChannels.find(channelName);
	if (it != currentChannels.end()) {
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
			std::string message1 = ": 353 " + serverObj->getClient()[cliente->getFd()]->getNickname() + " = " + channelName + " :@" + serverObj->getClient()[cliente->getFd()]->getNickname() + " " + msg + "\r\n";
			std::string message2 = ": 366 " + serverObj->getClient()[cliente->getFd()]->getNickname() + " " + channelName + " :End of /NAMES list.\r\n";
			send(cliente->getFd(), message1.c_str(), message1.size(), 0);
			send(cliente->getFd(), message2.c_str(), message2.size(), 0);
		}
	}
}

void Client::leaveChannel(Server *serverObj, int clientSocket, const std::string& channelName) {
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
    if (serverObj->getClient().find(clientSocket) != serverObj->getClient().end()) 
	{
        Client* clientObj = serverObj->getClient()[clientSocket];

		std::map<std::string, Channel> currentChannels = serverObj->getChannels();
        // Check if the channel exists, if not, create it
        if (currentChannels.find(channelName) == currentChannels.end())
		{
			std::cout << "channel creation\n";
            currentChannels[channelName] = Channel(channelName);
            currentChannels[channelName].setOperator(clientSocket, clientObj);
            currentChannels[channelName].setModes("n");
			std::string message = ":" + serverObj->getClient()[clientSocket]->getNickname() + "!" + serverObj->getClient()[clientSocket]->getUsername() + "@"  + serverObj->getClient()[clientSocket]->getLocation() + " JOIN " + channelName + " * :realname\r\n";
			send(clientSocket, message.c_str(), message.size(), 0);
			joined = true;
        }
		else
        {
            std::cout << "----------Channel Modes--------\n";
            std::vector<std::string> channel_modes = currentChannels[channelName].getModes();
            std::vector<std::string>::iterator it_modes;
            for (it_modes = channel_modes.begin(); it_modes != channel_modes.end(); ++it_modes)
            {
                std::cout << "***(" << it_modes->c_str() << ")***\n";
            }
            std::cout << "---------------------------\n";

            if (currentChannels[channelName].getModes().size() == 0)
            {
            	// serverObj->serverObj->channels[channelName].addClient(clientObj);
				// clientObj->setChannelName(channelName);

                //first part
                std::string message = ":" + serverObj->getClient()[clientSocket]->getNickname() + "!" + serverObj->getClient()[clientSocket]->getUsername() + "@"  + serverObj->getClient()[clientSocket]->getLocation() + " JOIN " + channelName + " * :realname\r\n";
				send(clientSocket, message.c_str(), message.size(), 0);
            
                //Hamza code variable
                joined = true;
            }
            else 
            {
                std::cout << "invite 2\n";
                int modesCount = 1;
                if (currentChannels[channelName].find_mode("k"))
                {
                    if (words[2] == currentChannels[channelName].key)
                        modesCount++;
                    else
                        std::cout << "Incorrect password" << std::endl;
                }
                if (currentChannels[channelName].find_mode("i"))
                {
                    std::cout << "invite 1\n";
                    std::map<int, Client*> inviteList = currentChannels[channelName].getInviteList();
                    std::cout << "invite 0\n";
                    std::map<int, Client*>::iterator it = inviteList.find(clientSocket);
                    if (it != inviteList.end())
                    {
                        std::cout << "it +++" << it->first << std::endl;
                        std::cout << "cS +++" << clientSocket << std::endl;
                    }
					std::cout << "invite 3\n";
                    if (it != inviteList.end())
                    {
                        std::cout << "invite 4\n";
                        modesCount++;
                    }
                    else
                    {
                        std::string message = ": 473 " + serverObj->getClient()[clientSocket]->getNickname() + " " + channelName + " :Cannot join channel (+i)\r\n";
                        send(clientSocket, message.c_str(), message.size(), 0);
                        std::cout << "You Should be invited to join this channel!" << std::endl;
                    }
                }
                if (currentChannels[channelName].find_mode("l"))
                {
                    int clientsCnt = currentChannels[channelName].clients.size();
                    if (clientsCnt < currentChannels[channelName].getLimit())
                        modesCount++;
                    else
                        std::cout << "Channel Full" << std::endl;
                }
                int modes = currentChannels[channelName].getModes().size();
                // std::cout << "........." << modes << std::endl;
                // std::vector<std::string> modat = serverObj->serverObj->channels[channelName].getModes();
                // std::cout << "......" << modat[0] << std::endl;
                
                if (modesCount == modes)
                {
                    std::cout << "modesCount --> " << modesCount << std::endl;
                    std::cout << "modes --> " << modes << std::endl;
                    std::cout << "---------------------------\n";
                    std::vector<std::string> channel_modes = currentChannels[channelName].getModes();
                    std::vector<std::string>::iterator it_modes;
                    for (it_modes = channel_modes.begin(); it_modes != channel_modes.end(); ++it_modes)
                    {
                        std::cout << "***(" << it_modes->c_str() << ")***\n";
                    }
                    std::cout << "---------------------------\n";
                    // serverObj->serverObj->channels[channelName].addClient(clientObj);
					// clientObj->setChannelName(channelName);

                    //first part
                   	std::string message = ":" + serverObj->getClient()[clientSocket]->getNickname() + "!" + serverObj->getClient()[clientSocket]->getUsername() + "@"  + serverObj->getClient()[clientSocket]->getLocation() + " JOIN " + channelName + " * :realname\r\n";
					send(clientSocket, message.c_str(), message.size(), 0);
            
                    //Hamza code variable
                    joined = true;
                }
            }
        }
		currentChannels[channelName].addClient(clientObj);
		clientObj->setChannelName(channelName);
		serverObj->setChannels(currentChannels);
	}
	// std::cout << "---------Operators--------\n";
    // std::map<int, Client*> admins = serverObj->channels[channelName].getOperators();
    // std::map<int, Client*>::iterator it_operator;
    // for (it_operator = admins.begin(); it_operator != admins.end(); ++it_operator)
    // {
    //     std::cout << "***(" << it_operator->second->getNickname() << ")***\n";
    // }
	if (joined)
    	sendList(serverObj , channelName);
}






void Client::broadcastMessage(Server *serverObj, const std::string& channelName, const std::string& message, int clientSocket) {
    std::map<std::string, Channel> currentChannels = serverObj->getChannels();
	std::map<std::string, Channel>::iterator it = currentChannels.find(channelName);
	std::vector<std::string> channelNames = serverObj->getClient()[clientSocket]->getChannelName();
    if (it != currentChannels.end()) {
        Channel& channel = it->second;
        std::map<int, Client*> channelClients = channel.getClients();
		// std::string message1 = "353 " + client[clientSocket]->getNickname() + " = " + channelName + " :" + client[clientSocket]->getNickname() + "!" + client[clientSocket]->getUsername() + "@localhost\r\n";
		// std::string message2 = "366 " + client[clientSocket]->getNickname() + " " + channelName + " :End of /NAMES list.\r\n";
		std::string  msg = ":" + serverObj->getClient()[clientSocket]->getNickname() + "!" + serverObj->getClient()[clientSocket]->getUsername() + "@" + serverObj->getClient()[clientSocket]->getLocation() + " " + message + "\r\n";
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
				command.Commande(serverObj, clientObj, clientSocket, words);
			}
			if (!command.check_Comande(words) && clientObj->getStatus() == 1)
			{
                if (!message.find("PRIVMSG"))
				{
                    clientObj->broadcastMessage(serverObj, words[1], message, clientSocket);
                }
			}
        }
        if (clientObj->getAuthentication() == 1 && clientObj->getStatus() == 0)
		{
            std::string message = ": 001 " + clientObj->getNickname() + " :Welcome to the IRC Network, " + clientObj->getNickname() + "!" + clientObj->getUsername() + "@" + serverObj->getClient()[clientSocket]->getLocation() + "\r\n";
            clientObj->sendMessage(clientSocket, message);
            clientObj->setStatus(1);
        }
    }
}
