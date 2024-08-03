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
    std::cout << "here 3\n";
    if (channel->find_mode("i"))
    {
        std::cout << "here 4\n";
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
        std::cout << "here 5\n";
        channel->addToInviteList(newMember);
        std::cout << "User added to invite list" << std::endl;
    }
}

void Commands::topic(int isAdmin, std::string newTopic, Channel* channel)
{
    if (newTopic != "")
    {
        if (channel->find_mode("t"))
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

void Commands::mode(int isAdmin, Channel* channel, std::vector<std::string> words)
{
    std::map<int, Client*>::iterator it = channel->operators.find(isAdmin);
    if (it != channel->operators.end())
    {
        if (words[2][0] == '+')
        {
            std::string modes = words[2];
            for (int i = 1; modes[i] != '\0'; i++)
            {
                if (modes[i] == 'k')
                {
                    channel->key = words[3];
                    channel->modes.push_back("k");
                    std::cout << "Channel password set to " << channel->key << std::endl;
                }
                else if (modes[i] == 'i')
                {
                    channel->modes.push_back("i");
                    std::cout << "Channel is now invite only " << std::endl;
                }
                else if (modes[i] == 'l')
                {
                    channel->limit = atoi(words[3].c_str());
                    std::cout << "Channel limit is set to " << channel->limit << std::endl;
                }
                else if (modes[i] == 'o')
                {
                    std::map<int, Client*> clients = channel->getClients();
                    std::string nickName = words[3];
                    std::map<int, Client*>::const_iterator it;
                    int clientFd = 0;
                    for (it = clients.begin(); it != clients.end(); ++it) 
                    {
                        if (it->second->username == nickName)
                        {
                            clientFd = it->first;
                            break;
                        }
                    }
                    channel->setOperator(clientFd, clients[clientFd]);
                    std::cout << "New operator added --> " << clients[clientFd]->nickname << std::endl;
                }
            }
        }
        else if (words[2][0] == '-')
        {
            std::string modes = words[2];
            for (int i = 1; modes[i] != '\0'; i++)
            {
                if (modes[i] == 'k')
                {
                    std::vector<std::string>::iterator it;
                    for (it = channel->modes.begin(); it != channel->modes.end(); ++it)
                    {
                        if (*it == "k")
                            it = channel->modes.erase(it);
                    } 
                    channel->key.clear();
                    std::cout << "Channel password removed"<< std::endl;
                }
                else if (modes[i] == 'i')
                {
                    std::vector<std::string>::iterator it;
                    for (it = channel->modes.begin(); it != channel->modes.end(); ++it)
                    {
                        if (*it == "i")
                            it = channel->modes.erase(it);
                    }
                    std::cout << "Invite only mode removed"<< std::endl;
                }
                else if (modes[i] == 'l')
                {
                    std::vector<std::string>::iterator it;
                    for (it = channel->modes.begin(); it != channel->modes.end(); ++it)
                    {
                        if (*it == "l")
                            it = channel->modes.erase(it);
                    } 
                    channel->setLimit(0);
                    std::cout << "Channel limit removed"<< std::endl;
                }
                else if (modes[i] == 'o')
                {
                    std::map<int, Client*> clients = channel->getClients();
                    std::string nickName = words[3];
                    std::map<int, Client*>::const_iterator it;
                    int clientFd;
                    for (it = clients.begin(); it != clients.end(); ++it) 
                    {
                        if (it->second->username == nickName)
                        {
                            clientFd = it->first;
                            break;
                        }
                    }
                    channel->operators.erase(clientFd);
                    std::cout << "Operator removed from --> " << clients[clientFd]->nickname << std::endl;
                }
            }
        }
    }
    else
    {
        std::cout << "You have to be an Operator to proceed this operation" << std::endl;
        return ;
    }
}

Commands::~Commands() {
    std::cout << "Default commands Destructor called";
}
