#include "../include/Channel.hpp"

Channel::Channel(const std::string& name) : name(name) {}

void Channel::addClient(Client* client) {
    clients[client->getFd()] = client;
}

Channel::~Channel()
{

}

Channel::Channel()
{

}
std::string Channel::getName(){
    return name;
}

std::map<int, Client*> Channel::getClients() {
    return clients;
}


// Channel::Channel(Channel const &src)
// {
// 	std::cout << "Channel : copy constructor called" << std::endl;
// 	*this = src;
// }
// Channel & Channel::operator=(Channel const &src)
// {
// 	std::cout << "Channel : assignation operator called" << std::endl;
// 	if (this != & src)
// 	{
// 		*this = src;
// 	}
//     return (*this);
// }


void Channel::removeClient(Client* client) {
	clients.erase(client->getFd());
}