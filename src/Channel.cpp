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
