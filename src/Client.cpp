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

std::string  Client::getLocation() const
{
	return (this->_Location);
}
std::vector<std::string>  Client::getChannelName() const
{
	return (_channelNames);
}

void Client::setChannelName(const std::string& channelName)
{
	_channelNames.push_back(channelName);
}

void Client::setLocation(const std::string& Location)
{
	this->_Location = Location;
}

bool Client::getStatus()
{
	return status;
}

void Client::setStatus(int status)
{
	this->status = status;
}

void Client::eraseClientChannel(const std::string& channelName)
{
	std::vector<std::string>::iterator it = std::find(_channelNames.begin(), _channelNames.end(), channelName);
	if (it != _channelNames.end())
		_channelNames.erase(it);
}
