#ifndef SERVER_HPP
#define SERVER_HPP

#include <sstream>
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <map>

#include "./client_channel/Client.hpp"
#include "./client_channel/Channel.hpp"

class Client;

class Channel;

class Server
{
    private :
        int                         port;
        std::string                 password;

        int                         serverSocket;
        std::vector<pollfd>         clientSockets;

    public:
		std::map <int, Client *> 	mapClient;
		std::map<std::string, Channel> channels;
		std::map <int, Client *> getClient();
		void	setChannels(const std::map<std::string, Channel>& newChannels);
		std::map<std::string, Channel> getChannels();
        void        handleClient(Server* serverObj, int clientSocket);
        void        acceptClient();
        void        removeClient(int clientSocket);
        void        setNonBlocking(int socket);
		Server();
		~Server();
		Server(Server const &src);
		Server & operator=(Server const &src);
		std::string	getPassword();
        Server(int port, const std::string& password);
        void start(Server*	serverObj);
		Channel* findChannelByName(const std::string& channelName);

};

#endif
