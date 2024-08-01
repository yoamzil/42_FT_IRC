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
#include <cstdlib>
#include <cstdio>

#ifndef __unused
#define __unused
#endif

// #include "../include/Client.hpp"
#include "../include/Commands.hpp"
#include "../include/Channel.hpp"
#include "../include/Client.hpp"

class Server : public Commands
{
    public:
        int                            port;
        std::string                    password;
        int                            serverSocket;
        std::vector<pollfd>            clientSockets;
        std::map<int, std::string>     clients;
		std::map<int, Client *>        client;
		std::map<std::string, Channel> channels;
		int status;

        void        handleClient(int clientSocket);
        void        acceptClient();
        void        removeClient(int clientSocket);
        void        setNonBlocking(int socket);
    public:
        Server(int port, const std::string& password);
        void        handleMessage(__unused int clientSocket, const std::string& message);
		void        joinChannel(int clientSocket, __unused std::string& channelName, std::vector<std::string> words);
		void		leaveChannel(int clientSocket, const std::string& channel);
		void		broadcastMessage(const std::string& channelName, const std::string& message, int clientSocket);
		void		authentication(Client* clientObj, __unused int clientSocket, std::vector<std::string> & words);
		// void		Message(int clientSocket, const std::string& message);
        void start();
		void sendMessage(int clientSocket, const std::string& message);

};

#endif
