#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../include/Server.hpp"

// class Server;

class Client
{
    private:
		int			_fd;
		std::string username;
		std::string nickname;
		std::string pass;
		bool		correctPassword;

    public:
		Client();
		void setUsername(std::string userName);
		void setNikename(std::string nikeName);
		std::string getUsername();
		std::string getNickname();
		bool 	check_password(std::string str);
		void	setPassword(std::string Pass);
		void	setFd(int fd);
		int 	getFd();
		void	setCorrectPass();
		bool 	correctpass();
		~Client();
		void start();
};

#endif
