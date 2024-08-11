#include "../include/Channel.hpp"
#include <iostream>

Channel::Channel(std::string& name) : name(name) {}

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

void Channel::setOperator(int clientSocket, Client* newOperator)
{
    std::cout << "3iiiw\n";
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

void Channel::deleteMode(std::string toDelete)
{
    std::vector<std::string>::iterator iter;
    for (iter = modes.begin(); iter != modes.end(); ++iter)
    {
        if (*iter == toDelete)
        {
            iter = modes.erase(iter);
            break;
        }
    }
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

int	Channel::find_mode(std::string mode) 
{
    for (std::vector<std::string>::iterator it = modes.begin(); it != modes.end(); ++it) {
        if (*it == mode) {
            return 1;
        }
    }
    return 0;
}

int Channel::getLimit()
{
    return (limit);
}
void Channel::setLimit(int newLimit)
{
    limit = newLimit;
}

std::map<int, Client*> Channel::getOperators()
{
    return (operators);
}

std::map<int, Client*> Channel::getInviteList()
{
    return (inviteList);
}
