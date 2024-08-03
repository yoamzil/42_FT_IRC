#include "../include/Client.hpp"

Client::Client()
{
	this->_fd = 0;
	this->correctPassword = 0;
	this->_authentication = 0;
	this->status = 0;
}

void	Client::setPassword(std::string Pass)
{
	this->pass = Pass;
}

void	Client::setFd(int fd)
{
	this->_fd = fd;
}

void Client::setUsername(std::string userName) 
{
	this->username = userName;
}

void Client::setNikename(std::string nikeName) 
{
	this->nickname = nikeName;
}

std::string Client::getUsername()
{
	return (this->username);
}

std::string Client::getNickname()
{
	return (this->nickname);
}

Client::~Client()
{

}

int Client::getFd()
{
	return (this->_fd);
}

void	Client::setCorrectPass()
{
	this->correctPassword = 1;
}

bool 	Client::correctpass()
{
	return (this->correctPassword);
}

void	Client::setAuthentication()
{
	this->_authentication = 1;
}

bool 	Client::getAuthentication()
{
	return (this->_authentication);
}


// Client::Client(Client const &src)
// {
// 	std::cout << "Client : copy constructor called" << std::endl;
// 	*this = src;
// }
// Client & Client::operator=(Client const &src)
// {
// 	std::cout << "Client : assignation operator called" << std::endl;
// 	if (this != & src)
// 	{
// 		*this = src;
// 	}
//     return (*this);
// }


std::string  Client::getChannelName(const std::string & message) const
{

	return (message);
}

void Client::setChannelName(const std::string& channelName)
{
	this->_channelName.push_back(channelName);
}

bool Client::getStatus()
{
	return status;
}

void Client::setStatus(int status)
{
	this->status = status;
}