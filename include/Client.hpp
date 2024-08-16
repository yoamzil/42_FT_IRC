#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../include/Commands.hpp"
#include "../include/Channel.hpp"
#include "../include/Client.hpp"

class Server;
class Channel;

class Client : public Commands
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
		int 	getFd();
		bool 	correctpass();
		bool 	getAuthentication();
		std::vector<std::string> getChannelName() const;
		~Client();
		std::string  getLocation() const;
		std::vector<std::string> splitString(const std::string& str, const std::string& delimiter);
		std::vector<std::string> splitString1(const std::string& str);
		void		setFd(int fd);
		void		setCorrectPass();
		void		setAuthentication();
		void 		setChannelName(const std::string& channelName);
		void 		eraseClientChannel(const std::string& channelName);
		void 		start();
		void 		setLocation(const std::string& Location);
		void 		sendMessage(int clientSocket, const std::string& message);
		void 		sendList(Server *serverObj , int clientSocket, const std::string& channelName);
		void 		joinChannel(Server *serverObj, int clientSocket, const std::string& channelName, std::vector<std::string> words);
		void		leaveChannel(Server *serverObj, int clientSocket, const std::string& channel);
		void		broadcastMessage(Server *serverObj, const std::string& channelName, const std::string& message, int clientSocket);
		void		authentication(Server* serverObj, Client* clientObj, int clientSocket, std::vector<std::string> & words);
		void 		handleMessage(Server* serverObj, int clientSocket, const std::string& message);
		void 		LeaveSendList(Server *serverObj , __unused int clientSocket, const std::string& channelName);

};

#endif
