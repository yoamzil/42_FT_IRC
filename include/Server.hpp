#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

class Server
{
    private:
        int                 port;
        std::string         password;

        int                 serverSocket;
        std::vector<int>    clientSockets;

        void        handleClient(int clientSocket);
        void        acceptClient();
        void        removeClient(int clientSocket);
        void        setNonBlocking(int socket);

    public:
        Server(int port, const std::string& password);
        void start();

};

#endif
