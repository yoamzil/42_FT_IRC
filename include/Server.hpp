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
		std::map <int, Client *> client;
		std::map<std::string, Channel> channels;
    public:
        // void addChannel(Channel channelName);
        // void removeChannel(Channel channelName);
        
        std::map<int, Client *>           getClient();
		std::map<std::string, Channel>    getChannels();
		std::string                       getPassword();
		
        void        handleClient(Server* serverObj, int clientSocket);
        void        acceptClient();
        void        removeClient(int clientSocket);
        void        setNonBlocking(int socket);
		Server();
		~Server();
		Server(Server const &src);
		Server & operator=(Server const &src);
        Server(int port, const std::string& password);
        void start(Server*	serverObj);
		Channel* findChannelByName(const std::string& channelName);

};

#endif
