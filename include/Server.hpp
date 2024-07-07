#ifndef SERVER_HPP
#define SERVER_HPP

#include <sstream>      // std::istringstream
#include <string>
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <map>

#include "../include/Client.hpp"

class Client;

class Server
{
    private:
        int                         port;
        std::string                 password;


        int                         serverSocket;
        std::vector<pollfd>         clientSockets;
        std::map<int, std::string>  clients;
		int status;
		std::map <int, Client *> client;
        // std::map<int, std::string>  nicknames;
        // std::map<std::string, std::vector<int> > channels;

        void        handleClient(int clientSocket);
        void        acceptClient();
        void        removeClient(int clientSocket);
        void        setNonBlocking(int socket);
    public:
        Server(int port, const std::string& password);
		void 		handleMessage(int clientSocket, const std::string& message);
		void		joinChannel(int clientSocket, const std::string& channel);
		void		leaveChannel(int clientSocket, const std::string& channel);
		void		broadcastMessage(const std::string& channel, const std::string& message);
        void start();

};

#endif
