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
}

void    Server::handleClient(int clientSocket)
{
    std::cout << "Handling client" << std::endl;
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
    send(clientSocket, buffer, bytesRead, 0);
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
    clientSockets.push_back(clientSocket);
    std::cout << "New client connected" << clientSocket << std::endl;
}

void    Server::start()
{
    std::cout << "Server started on port " << port << " with password " << password << std::endl;
    while (true)
    {
        acceptClient();
        for (size_t i = 0; i < clientSockets.size(); i++)
        {
            handleClient(clientSockets[i]);
        }
    }
}

void    Server::removeClient(int clientSocket)
{
    close(clientSocket);
    clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), clientSocket), clientSockets.end());
    std::cout << "Client disconnected" << clientSocket << std::endl;
}