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

void Channel::setModes(std::string newMode)
{
    modes.push_back(newMode);
}

std::vector<std::string> Channel::getModes()
{
    return (modes);
}

void Channel::setTopic(std::string newTopic)
{
    topic = newTopic;
}

std::string Channel::getTopic()
{
    return (topic);
}

std::string Channel::getKey() 
{
    return (key);
}

void Channel::setKey(std::string newKey)
{
    key = newKey;
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
