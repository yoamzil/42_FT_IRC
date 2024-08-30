#include "../include/Commands.hpp"
#include "../include/Channel.hpp"
#include "../include/Client.hpp"

Commands::Commands() {
}

void Commands::kick(Server* serverObj, int isAdmin, int toKick, Channel* channel)
{
    std::string channelName = channel->getName();
    std::map<std::string, Channel>::iterator channelIt = serverObj->channels.find(channelName);
    if (channelIt != serverObj->channels.end())
    {
        std::map<int, Client*>::iterator adminIt = serverObj->channels[channelName].operators.find(isAdmin);
        if (adminIt != serverObj->channels[channelName].operators.end())
        {
            serverObj->channels[channelName].clients.erase(toKick);
            std::map<int, Client*>::iterator isInvIt = serverObj->channels[channelName].inviteList.find(toKick);
            if (isInvIt != serverObj->channels[channelName].inviteList.end())
                serverObj->channels[channelName].inviteList.erase(toKick);
            std::map<int, Client*>::iterator isOperIt = serverObj->channels[channelName].operators.find(toKick);
            if (isOperIt != serverObj->channels[channelName].operators.end())
                serverObj->channels[channelName].operators.erase(toKick);
            serverObj->client[toKick]->eraseClientChannel(channelName);
            std::map<int, Client*> members = serverObj->channels[channelName].getClients();
            for (std::map<int, Client*>::iterator it = members.begin(); it != members.end(); ++it) 
            {
                std::string message = ":" + serverObj->channels[channelName].operators[isAdmin]->getNickname() + "!" + serverObj->channels[channelName].operators[isAdmin]->getUsername() + "@" + serverObj->client[isAdmin]->getLocation() + " KICK #1 " + serverObj->client[toKick]->getNickname() + " :" + serverObj->channels[channelName].operators[isAdmin]->getNickname() + "\r\n";
                send(it->first, message.c_str(), message.size(), 0);
            }
            std::string message = ":" + serverObj->client[isAdmin]->getNickname() + "!" + serverObj->client[isAdmin]->getUsername() + "@" + serverObj->client[isAdmin]->getLocation() + " KICK #1 " + serverObj->client[toKick]->getNickname() + " :" + serverObj->client[isAdmin]->getNickname() + "\r\n";
            send(toKick, message.c_str(), message.size(), 0);
        }
        else
        {
            std::string message = ": 482 " + serverObj->channels[channelName].clients[isAdmin]->getNickname() + " #1 :You're not channel operator \r\n";
            send(isAdmin, message.c_str(), message.size(), 0);
            return ;
        }
    }
        std::cout << "---------Operators-IN-Kick--------\n";
        std::map<int, Client*> admins = serverObj->channels[channelName].getOperators();
        std::map<int, Client*>::iterator it_operator;
        for (it_operator = admins.begin(); it_operator != admins.end(); ++it_operator)
        {
            std::cout << "*** kick operators now (" << it_operator->second->getNickname() << ")***\n";
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
        if (words.size() == 2)
        {
            std::vector<std::string> channel_modes = channel->getModes();
            std::vector<std::string>::iterator it_modes = channel_modes.begin();
            std::string modes_str = "+";
            for (long unsigned i = 0; i < channel_modes.size(); i++)
                modes_str += it_modes->c_str();
            std::string modeMessage = ": 324 " + it->second->getNickname() + "!" + it->second->getUsername() + " " + channel->getName() + " +n\r\n";
            send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
        }
        else if (!words[2].empty())
        {
            int sign = 0;
            for (int i = 0; words[2][i] != '\0'; i++)
            {
                std::cout << "Dekhla Inside for in mode " << words[2][i] << "iiii --> " << i << "\n";
                if (words[2][i] == '+')
                {
                    sign = 1;
                    i++;
                }
                else if (words[2][i] == '-')
                {
                    sign = 2;
                    i++;
                }
                std::cout << "Mor Sign Inside for in mode " << words[2][i] << "iiii --> " << i << "sign --> " << sign << "\n";
                if (sign == 1) ////////////////
                {
                    if (words[2][i] == 'k')
                    {
                        channel->key = words[3];
                        channel->modes.push_back("k");
                        std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + channel->getName() + " +k " + words[3].c_str() + "\r\n";
                        send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
                    }
                    else if (words[2][i] == 't')
                    {
                        std::cout << "Inside topic in for in mode \n";
                        channel->modes.push_back("t");
                        std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + channel->getName() + " +t\r\n";
                        send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
                    }
                    else if (words[2][i] == 'i')
                    {
                        channel->modes.push_back("i");
    
                        std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + channel->getName() + " +i\r\n";
                        send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
                    }
                    else if (words[2][i] == 'l')
                    {
                        channel->limit = atoi(words[3].c_str());
                        channel->modes.push_back("l");
                        std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + channel->getName() + " +l " + words[3].c_str() + "\r\n";
                        send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
                    }
                    else if (words[2][i] == 'o')
                    {
                        std::map<int, Client*> clients = channel->getClients();
                        std::string nickName = words[3];
                        std::map<int, Client*>::const_iterator iter;
                        int clientFd = 0;
                        for (iter = clients.begin(); iter != clients.end(); ++iter) 
                        {
                            std::string userName = iter->second->getNickname();
                            if (userName == nickName)
                            {
                                clientFd = iter->first;
                                break;
                            }
                        }
                        if (clientFd != 0)
                        {
                            std::map<int, Client*> operators = channel->getOperators();
                            std::map<int, Client*>::iterator isAlreadyOper = operators.find(clientFd);
                            if (isAlreadyOper == operators.end())
                            {
                                channel->setOperator(clientFd, clients[clientFd]);
                                std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + channel->getName() + " +o " + words[3].c_str() + "\r\n";
                                send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
                            }
                        }
                    }
                }
                else if (sign == 2) ////////////////
                {
                    if (words[2][i] == 'k')
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
                    }
                    else if (words[2][i] == 'i')
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
                    }
                    else if (words[2][i] == 'l')
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
                    }
                    else if (words[2][i] == 'o')
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
                    }
                }//end of else if -
            }
        }//end if words[2].empty
        
    }
    else
    {
        if (words.size() != 2)
        {
            std::string message = ": 482 " + channel->clients[isAdmin]->getNickname() + " #1 :You're not channel operator \r\n";
            send(isAdmin, message.c_str(), message.size(), 0);
        }
    }
}

Commands::~Commands() {
}
