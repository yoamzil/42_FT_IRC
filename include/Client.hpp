#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../include/Server.hpp"
#include "../include/Channel.hpp"

class Server;
class Channel;

class Client
{
    public:
		int			_fd;
		bool		_authentication;
		std::string		_Location;
		std::string username;
		std::string nickname;
		std::string pass;
		// std::string _channelName;
		std::vector<std::string>  _channelNames;
		bool		correctPassword;
		bool		status;

    public:
		Client();
		// Client(Client const &src);
		// Client & operator=(Client const &src);

		void setUsername(std::string userName);
		void setNikename(std::string nikeName);
		std::string getUsername();
		std::string getNickname();
		bool		getStatus();
		void		setStatus(int status);
		bool 	check_password(std::string str);
		// void	setPassword(std::string Pass);
		// std::string	getPassword();
		void	setFd(int fd);
		int 	getFd();
		void	setCorrectPass();
		bool 	correctpass();
		bool 	getAuthentication();
		void	setAuthentication();
		void setChannelName(const std::string& channelName);
		std::vector<std::string> getChannelName() const;
		void eraseClientChannel(const std::string& channelName);
		~Client();
		std::string  getLocation() const;
		void start();
		void setLocation(const std::string& Location);
		std::vector<std::string> splitString(const std::string& str, const std::string& delimiter);
		std::vector<std::string> splitString1(const std::string& str);
		void sendMessage(int clientSocket, const std::string& message);
		void sendList(Server *serverObj , int clientSocket, const std::string& channelName);
		void joinChannel(Server *serverObj, int clientSocket, const std::string& channelName);
		void		leaveChannel(Server *serverObj, int clientSocket, const std::string& channel);
		void		broadcastMessage(Server *serverObj, const std::string& channelName, const std::string& message, int clientSocket);
		void		authentication(Server* serverObj, Client* clientObj, int clientSocket, std::vector<std::string> & words);
		void 		handleMessage(Server* serverObj, int clientSocket, const std::string& message);

};

#endif
