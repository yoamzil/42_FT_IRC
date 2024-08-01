#include "../include/Commands.hpp"
#include "../include/Channel.hpp"
#include "../include/Client.hpp"

Commands::Commands() {
    std::cout << "Default commands constructor called";
}

void Commands::kick(int isAdmin, int toKick, Channel* channel) {
    std::map<int, Client*>::iterator it = channel->operators.find(isAdmin);
    if (it != channel->operators.end())
    {
        channel->clients.erase(toKick);
        std::cout << "Bug removed succefully" << std::endl;
    }
    else
    {
        std::cout << "You have to be an Operator to proceed this operation" << std::endl;
        return ;
    }
}

void Commands::invite(int isAdmin, Client* newMember, Channel* channel)
{
    if (channel->mode == "INVITE")
    {
        std::map<int, Client*>::iterator it = channel->operators.find(isAdmin);
        if (it != channel->operators.end())
        {
            channel->addToInviteList(newMember);
        }
        else
        {
            std::cout << "You have to be an Operator to proceed this operation" << std::endl;
            return ;
        }
    }
    else 
    {
        channel->addToInviteList(newMember);
        std::cout << "User added to invite list" << std::endl;
    }
}

void Commands::topic(int isAdmin, std::string newTopic, Channel* channel)
{
    if (newTopic != "")
    {
        if (channel->mode == "TOPIC")
        {
            std::map<int, Client*>::iterator it = channel->operators.find(isAdmin);
            if (it != channel->operators.end())
            {
                channel->setTopic(newTopic);
                std::cout << "Topic changed successfully list" << std::endl;
            }
            else
            {
                std::cout << "You have to be an Operator to proceed this operation" << std::endl;
                return ;
            }
        }
        else 
        {
            channel->setTopic(newTopic);
            std::cout << "Topic changed successfully list" << std::endl;
        }
    }
    else
    {
        // Just view the topic
        std::cout << channel->getTopic() << std::endl;
    }
}

void Commands::mode(Channel* channel, std::vector<std::string> words)
{
    
}

Commands::~Commands() {
    std::cout << "Default commands Destructor called";
}
