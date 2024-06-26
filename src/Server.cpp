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
    std::cout << "Received: " << buffer << std::endl;
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
    clients[clientSocket] = "";
    std::cout << "New client connected " << clientSocket << std::endl;
}

void    Server::start()
{
    std::cout << "Server started on port " << port << " with password " << password << std::endl;
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