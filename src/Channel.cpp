#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include <utility>
#include "../include/Channel.hpp"

Channel::Channel(std::string& name) : name(name) {}

void Channel::addClient(Client* client) {
    clients[client->getFd()] = client;
}

Channel::~Channel()
{
    // std::cout << "hello" << std::endl;
    std::cout << "hello" << std::endl;
}

Channel::Channel()
{

}
std::string Channel::getName(){
    return name;
}

std::map<int, Client*> Channel::getClients() 
{
    return clients;
}

void Channel::setOperator(int clientSocket, Client* newOperator)
{
    operators.insert(std::make_pair(clientSocket, newOperator));
}

void Channel::addToInviteList(Client* newMember)
{
    inviteList[newMember->getFd()] = newMember;
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
