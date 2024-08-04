#include "../include/Server.hpp"
#include <ostream>
#include <cerrno>

void    Server::setNonBlocking(int socket)
{
    int flags = fcntl(socket, F_GETFL, 0);
    if (flags < 0)
    {
        std::cerr << "Failed to get socket flags" << std::endl;
        close(socket);
        exit(1);
    }

    if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        std::cerr << "Failed to set socket to non-blocking" << std::endl;
        close(socket);
        exit(1);
    }
}

Server::Server(int port, const std::string& password) : port(port), password(password)
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        std::cerr << "Socket creation failed." << std::endl;
        exit(1);
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &serverAddr, sizeof(serverAddr)) < 0) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        std::cerr << "Failed to bind server socket" << std::endl;
        close(serverSocket);
        exit(1);
    }

    if (listen(serverSocket, 5) == -1)
    {
        std::cerr << "Failed to listen on server socket" << std::endl;
        close(serverSocket);
        exit(1);
    }
    setNonBlocking(serverSocket);

    pollfd serverPollFd = {serverSocket, POLLIN, 0};
    clientSockets.push_back(serverPollFd);
}

std::vector<std::string> splitString(const std::string& str, const std::string& delimiter) {
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



std::vector<std::string> splitString1(const std::string& str) {
    std::istringstream iss(str);
    std::vector<std::string> result;
    std::string word;

    while (iss >> word) {
        result.push_back(word);
    }
    return result;
}

void	Server::authentication(Client* clientObj, int clientSocket, std::vector<std::string> & words)
{
	if (words[0] == "PASS")
	{
		if (words[1] == password)
		{
			clientObj->setCorrectPass();
			// client.setPassword(password);
			std::cout << clientSocket << " : password is correct" << std::endl;
		}
		else if (words[1] != password)
		{
			std::cout << clientSocket << " : password  incorrect" << std::endl;
		}
	}
	else if (words[0] == "NICK" && clientObj->correctpass() == 1 && words[1] != "\0")
	{
		// int i = 0;
		for (std::map<int, Client*>::iterator clientIt = client.begin(); clientIt != client.end(); ++clientIt) {
        	Client* client = clientIt->second;
			if (client->getNickname() != words[1] && client->getNickname().empty()){
				// std::string str = client->getNickname();
				std::cout <<  "nick " << " : " << client->getNickname() << std::endl;
				std::cout <<  "nick " << " : " << clientObj->getNickname() << std::endl;
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
				sendMessage(clientSocket , ":" + str + "!" + client->getUsername() + "@localhost NICK :" + client->getNickname() + "\r\n");
				std::cout <<  "change a Nick  : " << words[1] << " , Id : " << clientObj->getFd() << " correct ID : " << client->correctpass()  << std::endl;
				return;
			}

			else if (client->getNickname() == words[1])
			{
				std::cout <<  "Nick already exist : " << words[1] <<  " , Id : " << clientSocket << " correct ID : " << clientObj->correctpass()  << std::endl;
				// 433 * hamza :Nickname is already in use.
				sendMessage(clientSocket , ": 433 * " + client->getNickname() + " :Nickname is already in use.\r\n");
				// std::cout << "nick "  << client->getNickname() << " Nick already exist : " << words[1]  << "  -- > " << i << std::endl;
				return;
			}
		}
	}
	else if (words[0] == "USER" && clientObj->correctpass() == 1 && !words[1].empty())
	{
		clientObj->setUsername(words[1]);
		std::cout <<  "Create a USER : " << words[1] << " , Id : " << clientSocket << " correct ID : " << clientObj->correctpass() << std::endl;
	}
	
}


void Server::broadcastMessage(const std::string& channelName, const std::string& message, int clientSocket) {
    std::map<std::string, Channel>::iterator it = channels.find(channelName);
    if (it != channels.end()) {
        Channel& channel = it->second;
        std::map<int, Client*> channelClients = channel.getClients();
		// std::string message1 = "353 " + client[clientSocket]->getNickname() + " = " + channelName + " :" + client[clientSocket]->getNickname() + "!" + client[clientSocket]->getUsername() + "@localhost\r\n";
		// std::string message2 = "366 " + client[clientSocket]->getNickname() + " " + channelName + " :End of /NAMES list.\r\n";
		// std::cout << "352 " << channelName << std::endl;
		std::string  msg = ":" + client[clientSocket]->getNickname() + "!" + client[clientSocket]->getUsername() + "@localhost " + message + "\r\n";
        for (std::map<int, Client*>::iterator clientIt = channelClients.begin(); clientIt != channelClients.end(); ++clientIt) {
            Client* cliente = clientIt->second;
			// message = " :haarab!~hamza@197.230.24.20 PRIVMSG #hh :;jg"
			if (cliente->getFd() != clientSocket)
            	sendMessage(cliente->getFd() , msg);
            	// sendMessage(client->getFd(), message1);
            	// sendMessage(client->getFd(), message2);
            // Logic to send message to client
        }
    }
}


void Server::sendMessage(int clientSocket, const std::string& message) {
	// std::cout  << "id : " <<  clientSocket << "message : "  << message << std::endl;
    send(clientSocket, message.c_str(), message.size(), 0);
}



void Server::joinChannel(int clientSocket, __unused std::string& channelName, std::vector<std::string> words)
{
	// std::string message = ":" + client[clientSocket]->getNickname() + "!" + client[clientSocket]->getUsername() + "@localhost JOIN " + channelName + " * :realname\r\n";
    // send(clientSocket, message.c_str(), message.size(), 0);
    if (client.find(clientSocket) != client.end()) 
	{
        std::cout << "f join \n";
        Client* clientObj = client[clientSocket];

        // Check if the channel exists, if not, create it
        if (channels.find(channelName) == channels.end()) 
        {
            std::cout << "channel creation\n";
            channels[channelName] = Channel(channelName);
            channels[channelName].addClient(clientObj);
            channels[channelName].setOperator(clientSocket, clientObj);
            channels[channelName].setModes("n");

            //first part
            std::string message = ":" + client[clientSocket]->getNickname() + "!" + client[clientSocket]->getUsername() + "@localhost JOIN " + channelName + " * :realname\r\n";
            send(clientSocket, message.c_str(), message.size(), 0);

            //Done by hamza trying to fit it somewhere in coda
            std::map<std::string, Channel>::iterator it = channels.find(channelName);
            if (it != channels.end()) 
            {
                Channel& channel = it->second;
                std::map<int, Client*> channelClients = channel.getClients();
                for (std::map<int, Client*>::iterator clientI = channelClients.begin(); clientI != channelClients.end(); ++clientI)
                {
                    Client* cliente = clientI->second;

                    std::string msg;
                    for (std::map<int, Client*>::iterator clientIt = channelClients.begin(); clientIt != channelClients.end(); ++clientIt)
                    {
                        Client* clien = clientIt->second;
                        if (cliente->getFd() != clien->getFd())
                            msg = msg + " " + clien->getNickname();
                    }
                    std::string message1 = ": 353 " + client[cliente->getFd()]->getNickname() + " = " + channelName + " :@" + client[cliente->getFd()]->getNickname() + " " + msg + "\r\n";
                    std::string message2 = ": 366 " + client[cliente->getFd()]->getNickname() + " " + channelName + " :End of /NAMES list.\r\n";
                    send(cliente->getFd(), message1.c_str(), message1.size(), 0);
                    send(cliente->getFd(), message2.c_str(), message2.size(), 0);
                }
            }
        }
        else
        {
            std::cout << "----------Channel Modes--------\n";
            std::vector<std::string> channel_modes = channels[channelName].getModes();
            std::vector<std::string>::iterator it_modes;
            for (it_modes = channel_modes.begin(); it_modes != channel_modes.end(); ++it_modes)
            {
                std::cout << "***(" << it_modes->c_str() << ")***\n";
            }
            std::cout << "---------------------------\n";

            if (channels[channelName].getModes().size() == 0)
            {
            	channels[channelName].addClient(clientObj);

                //first part
                std::string message = ":" + client[clientSocket]->getNickname() + "!" + client[clientSocket]->getUsername() + "@localhost JOIN " + channelName + " * :realname\r\n";
                send(clientSocket, message.c_str(), message.size(), 0);
            
                //Done by hamza trying to fit it somewhere in coda
                std::map<std::string, Channel>::iterator it = channels.find(channelName);
                if (it != channels.end()) 
                {
                    Channel& channel = it->second;
                    std::map<int, Client*> channelClients = channel.getClients();
                    for (std::map<int, Client*>::iterator clientI = channelClients.begin(); clientI != channelClients.end(); ++clientI)
                    {
                        Client* cliente = clientI->second;

                        std::string msg;
                        for (std::map<int, Client*>::iterator clientIt = channelClients.begin(); clientIt != channelClients.end(); ++clientIt)
                        {
                            Client* clien = clientIt->second;
                            if (cliente->getFd() != clien->getFd())
                                msg = msg + " " + clien->getNickname();
                        }
                        std::string message1 = ": 353 " + client[cliente->getFd()]->getNickname() + " = " + channelName + " :@" + client[cliente->getFd()]->getNickname() + " " + msg + "\r\n";
                        std::string message2 = ": 366 " + client[cliente->getFd()]->getNickname() + " " + channelName + " :End of /NAMES list.\r\n";
                        send(cliente->getFd(), message1.c_str(), message1.size(), 0);
                        send(cliente->getFd(), message2.c_str(), message2.size(), 0);
                    }
                }
            }
            else 
            {
                std::cout << "invite 2\n";
                int modesCount = 0;
                if (channels[channelName].find_mode("k"))
                {
                    if (words[2] == channels[channelName].key)
                        modesCount++;
                    else
                        std::cout << "Incorrect password" << std::endl;
                }
                if (channels[channelName].find_mode("i"))
                {
                    std::cout << "invite 1\n";
                    std::map<int, Client*> inviteList = channels[channelName].getInviteList();
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
                        std::string message = ": 473 " + client[clientSocket]->getNickname() + " " + channelName + " :Cannot join channel (+i)\r\n";
                        send(clientSocket, message.c_str(), message.size(), 0);
                        std::cout << "You Should be invited to join this channel!" << std::endl;
                    }
                }
                if (channels[channelName].find_mode("l"))
                {
                    int clientsCnt = channels[channelName].clients.size();
                    if (clientsCnt < channels[channelName].getLimit())
                        modesCount++;
                    else
                        std::cout << "Channel Full" << std::endl;
                }
                int modes = channels[channelName].getModes().size();
                if (modesCount == modes)
                {
                    std::cout << "modesCount --> " << modesCount << std::endl;
                    std::cout << "modes --> " << modes << std::endl;
                    std::cout << "---------------------------\n";
                    std::vector<std::string> channel_modes = channels[channelName].getModes();
                    std::vector<std::string>::iterator it_modes;
                    for (it_modes = channel_modes.begin(); it_modes != channel_modes.end(); ++it_modes)
                    {
                        std::cout << "***(" << it_modes->c_str() << ")***\n";
                    }
                    std::cout << "---------------------------\n";
                    channels[channelName].addClient(clientObj);

                    //first part
                    std::string message = ":" + client[clientSocket]->getNickname() + "!" + client[clientSocket]->getUsername() + "@localhost JOIN " + channelName + " * :realname\r\n";
                    send(clientSocket, message.c_str(), message.size(), 0);
            
                    //Done by hamza trying to fit it somewhere in coda
                    std::map<std::string, Channel>::iterator it = channels.find(channelName);
                    if (it != channels.end()) 
                    {
                        Channel& channel = it->second;
                        std::map<int, Client*> channelClients = channel.getClients();
                        for (std::map<int, Client*>::iterator clientI = channelClients.begin(); clientI != channelClients.end(); ++clientI)
                        {
                            Client* cliente = clientI->second;

                            std::string msg;
                            for (std::map<int, Client*>::iterator clientIt = channelClients.begin(); clientIt != channelClients.end(); ++clientIt)
                            {
                                Client* clien = clientIt->second;
                                if (cliente->getFd() != clien->getFd())
                                    msg = msg + " " + clien->getNickname();
                            }
                            std::string message1 = ": 353 " + client[cliente->getFd()]->getNickname() + " = " + channelName + " :@" + client[cliente->getFd()]->getNickname() + " " + msg + "\r\n";
                            std::string message2 = ": 366 " + client[cliente->getFd()]->getNickname() + " " + channelName + " :End of /NAMES list.\r\n";
                            send(cliente->getFd(), message1.c_str(), message1.size(), 0);
                            send(cliente->getFd(), message2.c_str(), message2.size(), 0);
                        }
                    }
                }
            }
        }
	}
	std::cout << "---------Operators--------\n";
    std::map<int, Client*> admins = channels[channelName].getOperators();
    std::map<int, Client*>::iterator it_operator;
    for (it_operator = admins.begin(); it_operator != admins.end(); ++it_operator)
    {
        std::cout << "***(" << it_operator->second->getNickname() << ")***\n";
    }
    std::cout << "---------------------------\n\n";
	std::cout << "---------Invite List--------\n";
    std::map<int, Client*> inviteList = channels[channelName].getInviteList();
    std::map<int, Client*>::iterator it_invite;
    for (it_invite = inviteList.begin(); it_invite != inviteList.end(); ++it_invite)
    {
        std::cout << "***(" << it_invite->second->getNickname() << ")***\n";
    }
    std::cout << "---------------------------\n";
	// std::map<std::string, Channel>::iterator it = channels.find(channelName);
	// if (it != channels.end()) 
	// {
	// 	Channel& channel = it->second;
	// 	std::map<int, Client*> channelClients = channel.getClients();
	// 	for (std::map<int, Client*>::iterator clientI = channelClients.begin(); clientI != channelClients.end(); ++clientI)
	// 	{
	// 		Client* cliente = clientI->second;

	// 		std::string msg;
	// 		for (std::map<int, Client*>::iterator clientIt = channelClients.begin(); clientIt != channelClients.end(); ++clientIt)
	// 		{
	// 			Client* clien = clientIt->second;
	// 			if (cliente->getFd() != clien->getFd())
	// 				msg = msg + " " + clien->getNickname();
	// 		}
	// 		std::string message1 = ": 353 " + client[cliente->getFd()]->getNickname() + " = " + channelName + " :@" + client[cliente->getFd()]->getNickname() + " " + msg + "\r\n";
	// 		std::string message2 = ": 366 " + client[cliente->getFd()]->getNickname() + " " + channelName + " :End of /NAMES list.\r\n";
	// 		send(cliente->getFd(), message1.c_str(), message1.size(), 0);
	// 		send(cliente->getFd(), message2.c_str(), message2.size(), 0);
	// 	}
    // }
}





void Server::handleMessage(int clientSocket, const std::string& message)
{
	// std::string input = "this\r\nis::a::test\r\nstring";
	Client* clientObj;

	std::vector<std::string> input;
	if (client.find(clientSocket) != client.end()) {

        clientObj = client[clientSocket];
        // Use clientObj to interact with the client
    }
	if (message.find("\r\n"))
	{
		std::vector<std::string> words;
		std::string delimiter = "\r\n";
		input = splitString(message, delimiter);
		if (input.size() > 1)
		{
			for (size_t i = 0; i < input.size() - 1; ++i) {
				words = splitString1(input[i]);
				authentication(clientObj, clientSocket, words);
			}
		}
		else{
			words = splitString1(input[0]);
			authentication(clientObj, clientSocket, words);
		}

		if (clientObj->correctpass() == 1 && clientObj->getNickname() != "\0" && clientObj->getUsername() != "\0" && clientObj->getStatus() == 0)
		{
			// std::string message = "001 " + clientObj->getNickname() + " :Welcome to the Network\r\n";
			// sendMessage(clientSocket, message); //"<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]"
			clientObj->setFd(clientSocket);
			clientObj->setAuthentication();
			// std::cout << clientObj->getFd()<< " : file discreptor" << std::endl;
		}
		if (clientObj->getAuthentication() == 1)
		{

			// Channel* channelObj;
			if (words[0] == "JOIN" && !words[1].empty() && clientObj->getStatus() == 1)
			{
				std::cout << "Entering join\n";
				// std::cout <<  "Welcome " << clientSocket << std::endl;
				joinChannel(clientSocket, words[1], words);
			}
			else if (words[0] == "KICK")
			{
                std::cout << "makanaklch loz 1\n";
				std::map<std::string, Channel>::iterator channelIt = channels.find(words[1]);
				if (channelIt != channels.end()) 
				{
					Channel* channelPtr = &(channelIt->second);
					bool found = false;
					std::map<int, Client*>::iterator it;
					for (it = channelPtr->clients.begin(); it != channelPtr->clients.end(); it++)
					{
						if (it->second->nickname == words[2])
						{
							found = true;
							break;
						}
					}
					if (found)
						kick(clientSocket, it->first, channelPtr);
					else
						std::cout << "No user found" << std::endl;
				}
			}
			else if (words[0] == "MODE")
			{
                std::cout << "makanaklch loz 2\n";
				std::map<std::string, Channel>::iterator channelIt = channels.find(words[1]);
				if (channelIt != channels.end()) 
				{
                    std::cout << "Entering mode\n";
					Channel* channelPtr = &(channelIt->second);
					mode(clientSocket, channelPtr,words);
				}
			}
			else if (words[0] == "INVITE")
			{
			    std::cout << "here 0\n";
				std::cout << "--> " << words[2] << std::endl;
                std::map<std::string, Channel>::iterator channelIt = channels.find(words[2]);
                if (channelIt != channels.end()) 
                {
                    std::cout << "here 1\n";
                    Channel* channelPtr = &(channelIt->second);
                    bool found = false;
                    std::map<int, Client*>::iterator it;
                    for (it = client.begin(); it != client.end(); it++)
                    {
                        if (it->second->nickname == words[1])
                        {
                            found = true;
                            break;
                        }
                    }
                    if (found)
                    {
                        std::cout << "here 2\n";
                        invite(clientSocket, it->second, channelPtr);
                    }
                    else
                        std::cout << "No user to invite" << std::endl;
                }
			}
			else if (words[0] == "TOPIC")
			{
			    std::map<std::string, Channel>::iterator channelIt = channels.find(words[1]);
                if (channelIt != channels.end()) 
                {
                    Channel* channelPtr = &(channelIt->second);
                    bool found = false;
                    std::map<int, Client*>::iterator it;
                    for (it = channelPtr->clients.begin(); it != channelPtr->clients.end(); it++)
                    {
                        if (it->second->nickname == words[2])
                        {
                            found = true;
                            break;
                        }
                    }
                    if (found)
                        topic(it->first, words[2], channelPtr);
                    else
                        std::cout << "Cannot set the topic" << std::endl;
                }
				
			}
			else if(words[0] != "JOIN" && clientObj->getStatus() == 1)
			{
				if (!message.find("PRIVMSG"))
				{
					broadcastMessage(words[1], message, clientSocket);
				}
				
			}
		}
		if (clientObj->getAuthentication() == 1 && clientObj->getStatus() == 0)
		{
			std::string message = ": 001 " + clientObj->getNickname() + " :Welcome to the IRC Network, " + clientObj->getNickname() + "!" + clientObj->getUsername() + "@localhost\r\n";
			sendMessage(clientSocket, message);
			clientObj->setStatus(1);
		}
	}
}

void    Server::handleClient(int clientSocket)
{
    char    buffer[512];
	
    int     bytesRead = recv(clientSocket, buffer, 512, 0);

    if (bytesRead <= 0)
    {
        if (bytesRead < 0 && (errno != EWOULDBLOCK && errno != EAGAIN))
        {
            std::cerr << "Failed to read from client" << std::endl;
        }
        removeClient(clientSocket);
        return ;
    }
    buffer[bytesRead] = '\0';
    // std::cout << "Received: " << buffer << std::endl;
	handleMessage(clientSocket, buffer);
	// std::string str = "hamza";
	// send(clientSocket, str.c_str(), str.size(), 0);
	// Message(clientSocket, buffer);

    // send(clientSocket, buffer, bytesRead, 0);
	// sendMessage(clientSocket, ":haarab!~hamza@10.11.7.6 JOIN #hh * :realname");
}

void    Server::acceptClient()
{
    int clientSocket = accept(serverSocket, NULL, NULL);

    if (clientSocket < 0)
    {
        if (errno != EWOULDBLOCK && errno != EAGAIN)
        {
            std::cerr << "Failed to accept client." << std::endl;
            close(serverSocket);
            exit(1);
        }
        return ;
    }
	setNonBlocking(clientSocket);
	pollfd clientPollFd = {clientSocket, POLLIN, 0};
	clientSockets.push_back(clientPollFd);
	// clients[clientSocket] = "";

	

	Client* newClient = new Client();
    newClient->setFd(clientSocket);
    client[clientSocket] = newClient;
	// Channel* newChannel = new Channel();

	std::cout << "New client connected " << clientSocket << std::endl;
}

void    Server::start()
{
    std::cout << "Server started on port " << port << " with password " << password << std::endl;
	// Client client;
    while (true)
    {
        int pollCount = poll(clientSockets.data(), clientSockets.size(), -1);
        if (pollCount < 0)
        {
            std::cerr << "Poll failed." << std::endl;
            close(serverSocket);
            exit(1);
        }
        for (size_t i = 0; i < clientSockets.size(); i++)
        {
            if (clientSockets[i].revents & POLLIN)
            {
                if (clientSockets[i].fd == serverSocket)
                {
                    acceptClient();
                }
                else if (clientSockets[i].fd != serverSocket)
                {
                    handleClient(clientSockets[i].fd);
                }
            }
        }
    }
}

void    Server::removeClient(int clientSocket)
{
    close(clientSocket);
    client.erase(clientSocket);
    for (size_t i = 0; i < clientSockets.size(); i++)
    {
        if (clientSockets[i].fd == clientSocket)
        {
            clientSockets.erase(clientSockets.begin() + i);
            break ;
        }
    }
    std::cout << "Client disconnected " << clientSocket << std::endl;
}

