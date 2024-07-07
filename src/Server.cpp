#include "../include/Server.hpp"

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


void Server::handleMessage(__unused int clientSocket, const std::string& message)
{
	// std::string input = "this\r\nis::a::test\r\nstring";
	std::vector<std::string> input;
	if (client.find(clientSocket) != client.end()) {
        Client* clientObj = client[clientSocket];
        // Use clientObj to interact with the client
		if (message.find("\r\n"))
		{
			std::string delimiter = "\r\n";
			input = splitString(message, delimiter);
			for (size_t i = 0; i < input.size() - 1; ++i) {
				std::vector<std::string> words = splitString1(input[i]);
				if (words[0] == "PASS")
				{
					if (words[1] == password)
					{
						clientObj->setCorrectPass();
						// client.setPassword(password);
						std::cout << clientObj->correctpass() << "password is correct" << std::endl;
					}
					else if (words[1] != password)
					{
						std::cout << clientObj->correctpass() << "password  incorrect" << std::endl;
					}
				}
				else if (words[0] == "NICK" && clientObj->correctpass() == 1 && words[1] != "\0")
				{
					clientObj->setNikename(words[1]);
					std::cout <<  "Create a Nick : " << words[1] << " , Id : " << clientSocket << " correct ID : " << clientObj->correctpass()  << std::endl;
				}
				else if (words[0] == "USER" && clientObj->correctpass() == 1 && words[1] != "\0")
				{
					clientObj->setUsername(words[1]);
					std::cout <<  "Create a USER : " << words[1] << " , Id : " << clientSocket << " correct ID : " << clientObj->correctpass() << std::endl;
				}
			}
		}
		std::vector<std::string> words = splitString1(message);
		// std::cout << "aarab : " << message << std::endl;
		if (words[0] == "PASS")
		{
			if (words[1] == password)
			{
				clientObj->setCorrectPass();
				// client.setPassword(password);
				std::cout << clientObj->correctpass() << "password is correct" << std::endl;
			}
			else if (words[1] != password)
			{
				std::cout << clientObj->correctpass() << "password  incorrect" << std::endl;
			}
		}
		else if (words[0] == "NICK" && clientObj->correctpass() == 1 && words[1] != "\0")
		{
			clientObj->setNikename(words[1]);
			std::cout <<  "Create a Nick : " << words[1] << " , Id : " << clientSocket << " correct ID : " << clientObj->correctpass()  << std::endl;
		}
		else if (words[0] == "USER" && clientObj->correctpass() == 1 && words[1] != "\0")
		{
			clientObj->setUsername(words[1]);
			std::cout <<  "Create a USER : " << words[1] << " , Id : " << clientSocket << " correct ID : " << clientObj->correctpass() << std::endl;
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
	status = 0;
	setNonBlocking(clientSocket);
	pollfd clientPollFd = {clientSocket, POLLIN, 0};
	clientSockets.push_back(clientPollFd);
	clients[clientSocket] = "";
	Client* newClient = new Client();
    newClient->setFd(clientSocket);
    client[clientSocket] = newClient;

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
                else
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
    clients.erase(clientSocket);
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