#ifndef SERVER_HPP
#define SERVER_HPP

#include <sstream>      // std::istringstream
// #include <string>
// #include <stdlib.h>
// #include <cerrno>
// #include <errno.h>
// #include <stdio.h>
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <map>

#include "../include/Client.hpp"
#include "../include/Channel.hpp"

class Client;

class Channel;

class Server
{
    public:
        int                         port;
        std::string                 password;


        int                         serverSocket;
        std::vector<pollfd>         clientSockets;
        // std::map<int, std::string>  clients;
		std::map <int, Client *> client;
		std::map<std::string, Channel> channels;

        // std::map<int, std::string>  nicknames;
        // std::map<std::string, std::vector<int> > channels;


        void        handleClient(Server* serverObj, int clientSocket);
        void        acceptClient();
        void        removeClient(int clientSocket);
        void        setNonBlocking(int socket);
    public:
		Server();
		Server(Server const &src);
		Server & operator=(Server const &src);
		std::string	getPassword();
		// void 		sendList(Server *serverObj , int clientSocket, const std::string& channelName);
        Server(int port, const std::string& password);
		// void 		handleMessage(int clientSocket, const std::string& message);
		// void		joinChannel(int clientSocket, const std::string& channel);
		// void		leaveChannel(int clientSocket, const std::string& channel);
		// void		broadcastMessage(const std::string& channelName, const std::string& message, int clientSocket);
		void		authentication(Client* clientObj, int clientSocket, std::vector<std::string> & words);
		// void		Message(int clientSocket, const std::string& message);
        void start(Server*	serverObj);
		
		Channel* findChannelByName(const std::string& channelName);

};

#endif
