#include "../include/Server.hpp"

Server::Server(Server const &src)
{
	*this = src;
}

Server & Server::operator=(Server const &src)
{
	if (this != & src)
	{
		*this = src;
	}
    return (*this);
}



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

Server::~Server()
{
	close(serverSocket);
}

std::string Server::getPassword()
{
	return (password);
}


void    Server::handleClient(Server* serverObj, int clientSocket)
{
	
	Client clientObj;
    char    buffer[512];
	// std::cout << "Received" << std::endl;
	
    int     bytesRead = recv(clientSocket, buffer, 512, 0);

    if (bytesRead <= 0)
    {
        if (bytesRead < 0 && (errno != EWOULDBLOCK && errno != EAGAIN))
        {
            std::cerr << "Failed to read from client" << std::endl;
        }
		std::vector<std::string> channelNames = client[clientSocket]->getChannelName();
		for (std::vector<std::string>::iterator it = channelNames.begin(); it != channelNames.end(); ++it)
		{
			client[clientSocket]->leaveChannel(serverObj, clientSocket, *it);
		}
        removeClient(clientSocket);
        return ;
    }
    buffer[bytesRead] = '\0';
    // std::cout << "Received: " << buffer << std::endl;
	clientObj.handleMessage(serverObj, clientSocket, buffer);
	// send(clientSocket, str.c_str(), str.size(), 0);
	// Message(clientSocket, buffer);

    // send(clientSocket, buffer, bytesRead, 0);
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

void    Server::start(Server*	serverObj)
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
                if (clientSockets[i].fd != serverSocket)
                {
                    handleClient(serverObj, clientSockets[i].fd);
                }
            }
        }
		// system("leaks ircserv");
    }
}

void    Server::removeClient(int clientSocket)
{
    close(clientSocket);
	delete client[clientSocket];
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
