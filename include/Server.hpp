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
        std::map<int, std::string>  clients;
		std::map <int, Client *> client;
		// std::map <int, Channel *> channel;
		 std::map<std::string, Channel> channels;

        // std::map<int, std::string>  nicknames;
        // std::map<std::string, std::vector<int> > channels;

		// Server(Server const &src);
		// Server & operator=(Server const &src);

        void        handleClient(int clientSocket);
        void        acceptClient();
        void        removeClient(int clientSocket);
        void        setNonBlocking(int socket);
    public:

        Server(int port, const std::string& password);
		void 		handleMessage(int clientSocket, const std::string& message);
		void		joinChannel(int clientSocket, std::string& channel);
		void		leaveChannel(int clientSocket, const std::string& channel);
		void		broadcastMessage(const std::string& channelName, const std::string& message, int clientSocket);
		void		authentication(Client* clientObj, __unused int clientSocket, std::vector<std::string> & words);
		// void		Message(int clientSocket, const std::string& message);
        void start();
		void sendMessage(int clientSocket, const std::string& message);

};

#endif
