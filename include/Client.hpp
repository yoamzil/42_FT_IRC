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
		std::string username;
		std::string nickname;
		std::string pass;
		// std::string _channelName;
		std::vector<std::string> _channelName;
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
		void	setPassword(std::string Pass);
		void	setFd(int fd);
		int 	getFd();
		void	setCorrectPass();
		bool 	correctpass();
		bool 	getAuthentication();
		void	setAuthentication();
		std::string getChannelName(const std::string & message) const;
		void setChannelName(const std::string& channelName);
		~Client();
		void start();
};

#endif
