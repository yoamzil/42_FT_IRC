#include "../include/Commands.hpp"
#include "../include/Channel.hpp"
#include "../include/Client.hpp"

Commands::Commands() {
    std::cout << "Default commands constructor called";
}

void Commands::kick(Server* serverObj, int isAdmin, int toKick, Channel* channel)
{
    std::string channelName = channel->getName();
    std::map<std::string, Channel>::iterator channelIt = serverObj->channels.find(channelName);
    if (channelIt != serverObj->channels.end())
    {
        std::map<int, Client*>::iterator adminIt = channel->operators.find(isAdmin);
        if (adminIt != channel->operators.end())
        {
            channel->clients.erase(toKick);
            std::string channelName = channel->getName();
            serverObj->client[toKick]->eraseClientChannel(channelName);
            std::map<int, Client*> members = channel->getClients();
            for (std::map<int, Client*>::iterator it = members.begin(); it != members.end(); ++it) 
            {
                std::string message = ":" + channel->operators[isAdmin]->getNickname() + "!" + channel->operators[isAdmin]->getUsername() + "@" + serverObj->client[isAdmin]->getLocation() + " KICK #1 " + serverObj->client[toKick]->getNickname() + " :" + channel->operators[isAdmin]->getNickname() + "\r\n";
                send(it->first, message.c_str(), message.size(), 0);
            }
            std::string message = ":" + channel->operators[isAdmin]->getNickname() + "!" + channel->operators[isAdmin]->getUsername() + "@" + serverObj->client[isAdmin]->getLocation() + " KICK #1 " + serverObj->client[toKick]->getNickname() + " :" + channel->operators[isAdmin]->getNickname() + "\r\n";
            send(toKick, message.c_str(), message.size(), 0);
        }
    }
    else
    {
        std::string message = ": 482 " + channel->clients[isAdmin]->getNickname() + " #1 :You're not channel operator \r\n";
        send(isAdmin, message.c_str(), message.size(), 0);
        return ;
    }
}

void Commands::invite(Server* serverObj, int isAdmin, std::vector<std::string> words)
{
    std::map<std::string, Channel>::iterator channelIt = serverObj->channels.find(words[2]);
    if (channelIt != serverObj->channels.end()) 
    {
        bool found = false;
        std::map<int, Client*>::iterator newMemberIt;
        for (newMemberIt = serverObj->client.begin(); newMemberIt != serverObj->client.end(); newMemberIt++)
        {
            if (newMemberIt->second->nickname == words[1])
            {
                found = true;
                break;
            }
        }
        if (found)
        {
            if (channelIt->second.find_mode("i"))
            {
                std::map<int, Client*>::iterator it = channelIt->second.operators.find(isAdmin);
                if (it != channelIt->second.operators.end())
                {
                    channelIt->second.addToInviteList(newMemberIt->second);
                    std::string message = ": 341 " + channelIt->second.clients[isAdmin]->getNickname() + " " + newMemberIt->second->getNickname() + " :" + channelIt->second.getName() + " \r\n";
                    send(isAdmin, message.c_str(), message.size(), 0);
                    message = ":" + channelIt->second.clients[isAdmin]->getNickname() + "!" + channelIt->second.clients[isAdmin]->getUsername() + "@" + newMemberIt->second->getLocation() + " INVITE " + newMemberIt->second->getNickname() + " " + channelIt->second.getName() + "\r\n";
                    send(newMemberIt->second->getFd(), message.c_str(), message.size(), 0);
                }
                else
                {
                    std::string message = ": 482 " + newMemberIt->second->getNickname() + " " + channelIt->second.getName() + " :You're not channel operator\r\n";
           	        send(isAdmin, message.c_str(), message.size(), 0);
                }
            }
            else 
            {
                channelIt->second.addToInviteList(newMemberIt->second);
                std::string message = ": 341 " + channelIt->second.clients[isAdmin]->getNickname() + " " + newMemberIt->second->getNickname() + " :" + channelIt->second.getName() + " \r\n";
                send(isAdmin, message.c_str(), message.size(), 0);
                message = ":" + channelIt->second.clients[isAdmin]->getNickname() + "!" + channelIt->second.clients[isAdmin]->getUsername() + "@" + newMemberIt->second->getLocation() + " INVITE " + newMemberIt->second->getNickname() + " " + channelIt->second.getName() + "\r\n";
                send(newMemberIt->second->getFd(), message.c_str(), message.size(), 0);
            }
        }
        else
        {
            std::string message = ": " + words[1] + " " + channelIt->second.getName() + " :No such nick\r\n";
       	    send(isAdmin, message.c_str(), message.size(), 0);
        }
    }
}

void Commands::topic(Server* serverObj, int isAdmin, std::vector<std::string> words)
{
    std::map<std::string, Channel>::iterator channelIt = serverObj->channels.find(words[1]);
    if (channelIt != serverObj->channels.end()) 
    {
        if (words.size() == 2)
        {
            std::string topic = channelIt->second.getTopic();
            if (topic.empty())
            {
                std::string message = ": 331 " + serverObj->client[isAdmin]->getNickname() + " " + words[1] + " :No topic is set.\r\n";
     	        send(isAdmin, message.c_str(), message.size(), 0);
            }
            else
            {
                std::string message = ": 332 " + serverObj->client[isAdmin]->getNickname() + " " + words[1] + " :" + topic + "\r\n";
    	        send(isAdmin, message.c_str(), message.size(), 0);
            }
        }
        else if (channelIt->second.find_mode("t"))
        {
            std::map<int, Client*>::iterator it = channelIt->second.operators.find(isAdmin);
            if (it != channelIt->second.operators.end())
            {
                std::string newTopic = words[2].substr(1); 
                channelIt->second.setTopic(newTopic);
                std::string message = ":" + serverObj->client[isAdmin]->getNickname() + "!" + serverObj->client[isAdmin]->getUsername() + "@" + serverObj->client[isAdmin]->getLocation() + " TOPIC " + words[1] + " :" + newTopic + "\r\n";
       	        send(isAdmin, message.c_str(), message.size(), 0);
            }
            else
            {
                std::string message = ": 482 " + serverObj->client[isAdmin]->getNickname() + " " + words[1] + " :You're not channel operator \r\n";
                send(isAdmin, message.c_str(), message.size(), 0);
            }
        }
        else 
        {
            std::string newTopic = words[2].substr(1); 
            channelIt->second.setTopic(newTopic);
            std::string message = ":" + serverObj->client[isAdmin]->getNickname() + "!" + serverObj->client[isAdmin]->getUsername() + "@" + serverObj->client[isAdmin]->getLocation() + " TOPIC " + words[1] + " :" + newTopic + "\r\n";
   	        send(isAdmin, message.c_str(), message.size(), 0);
        }
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
                    std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + channel->getName() + " +k " + words[3].c_str() + "\r\n";
                    send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
                    std::cout << "Channel password set to " << channel->key << std::endl;
                }
                else if (modes[i] == 't')
                {
                    std::cout << "In mode function in topic condition \n";
                    channel->modes.push_back("t");
                    std::cout << "Channel is now topic only " << std::endl;

                    std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + channel->getName() + " +t\r\n";
                    send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
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
                    std::map<int, Client*>::const_iterator iter;
                    int clientFd = 0;
                    for (iter = clients.begin(); iter != clients.end(); ++iter) 
                    {
                        std::cout << "teeeeest\n";
                        std::string userName = iter->second->getNickname();
                        std::cout << "username --> " << userName << std::endl;
                        if (userName == nickName)
                        {
                            std::cout << "+++" << clientFd << std::endl;
                            clientFd = iter->first;
                            break;
                        }
                    }
                    if (clientFd != 0)
                    {
                        std::cout << "................. " << words[3].c_str() << std::endl;
                        channel->setOperator(clientFd, clients[clientFd]);
                        std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + channel->getName() + " +o " + words[3].c_str() + "\r\n";
                        send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
                    }
                }
            }
        }
        else if (!words[2].empty() && words[2][0] == '-') ///////////////////////////////////////////////////////////////
        {
            std::string modes = words[2];
            for (int i = 1; modes[i] != '\0'; i++)
            {
                if (modes[i] == 'k')
                {
                    std::vector<std::string>::iterator iter;
                    for (iter = channel->modes.begin(); iter != channel->modes.end(); ++iter)
                    {
                        if (*iter == "k")
                        {
                            channel->deleteMode("k");
                            break;
                        }
                    } 
                    channel->key.clear();
                    std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + channel->getName() + " -k " + words[3].c_str() + "\r\n";
                    send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
                    std::cout << "Channel password removed"<< std::endl;
                }
                else if (modes[i] == 'i')
                {
                    std::vector<std::string> mods = channel->getModes();
                    std::vector<std::string>::iterator iter;
                    for (iter = mods.begin(); iter != mods.end(); ++iter)
                    {
                        if (*iter == "i")
                        {
                            channel->deleteMode("i");
                            break;
                        }
                    }
                    std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + channel->getName() + " -i\r\n";
                    send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
                    std::cout << "Invite only mode removed"<< std::endl;
                }
                else if (modes[i] == 'l')
                {
                    std::vector<std::string>::iterator iter;
                    for (iter = channel->modes.begin(); iter != channel->modes.end(); ++iter)
                    {
                        if (*iter == "l")
                        {
                            channel->deleteMode("l");
                            break;
                        }
                    } 
                    channel->setLimit(0);
                    std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + channel->getName() + " -l \r\n";
                    send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
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
                        if (it->second->nickname == nickName)
                        {
                            clientFd = it->first;
                            break;
                        }
                    }
                    std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + channel->getName() + " -o " + words[3].c_str() + "\r\n";
                    send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
                    channel->operators.erase(clientFd);
                    std::cout << "Operator removed from --> " << clients[clientFd]->nickname << std::endl;
                }
            }
        }
    }
    else
    {
        std::cout << "You have to be an Operator to proceed this operation 5" << std::endl;
        std::string message = ": 482 " + channel->clients[isAdmin]->getNickname() + " #1 :You're not channel operator \r\n";
        send(isAdmin, message.c_str(), message.size(), 0);
        // return ;
    }
}

Commands::~Commands() {
    std::cout << "Default commands Destructor called" << std::endl;
}
