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
            std::string message = ":" + newMember->getNickname() + "!" + newMember->getUsername() + "@localhost MODE " + channel->getName() + " +i \r\n";
	        send(isAdmin, message.c_str(), message.size(), 0);
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
        std::string message = ":" + newMember->getNickname() + "!" + newMember->getUsername() + "@localhost MODE " + channel->getName() + " +i \r\n";
	    send(isAdmin, message.c_str(), message.size(), 0);
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
        std::cout << "In mode function\n";
        if (words.size() == 2)
        {
            std::vector<std::string> channel_modes = channel->getModes();
            std::vector<std::string>::iterator it_modes = channel_modes.begin();
            std::string modes_str = "+";
            for (long unsigned i = 0; i < channel_modes.size(); i++)
            {
                // std::cout << it_modes->c_str() << std::endl;
                modes_str += it_modes->c_str();
            }
            // std::cout << "----" + modes_str + "----\n";
            // std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + channel->getName() + " +i\r\n";
            std::string modeMessage = ": 324 " + it->second->getNickname() + "!" + it->second->getUsername() + " " + channel->getName() + " +n\r\n";
            send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
        }
        else if (!words[2].empty() && words[2][0] == '+')
        {
            std::cout << "In mode function in + \n";
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
                    std::cout << "In mode function in invite condition \n";
                    channel->modes.push_back("i");
                    std::cout << "Channel is now invite only " << std::endl;

                    std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + channel->getName() + " +i\r\n";
                    send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
                }
                else if (modes[i] == 'l')
                {
                    channel->limit = atoi(words[3].c_str());
                    std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + channel->getName() + " +l " + words[3].c_str() + "\r\n";
                    send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
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
                    if (clientFd != 0)
                    {
                        channel->setOperator(clientFd, clients[clientFd]);
                        std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + channel->getName() + " +o " + words[3].c_str() + "\r\n";
                        send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
                    }
                    else
                    {

                        std::cout << "New operator added --> " << clients[clientFd]->nickname << std::endl;
                    }
                }
            }
        }
        else if (!words[2].empty() && words[2][0] == '-')
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
        std::cout << "You have to be an Operator to proceed this operation 5" << std::endl;
        return ;
    }
}

Commands::~Commands() {
    std::cout << "Default commands Destructor called";
}
