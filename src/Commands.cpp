#include "../include/Commands.hpp"
#include "../include/Channel.hpp"
#include "../include/Client.hpp"

Commands::Commands() {
}

void Commands::kick(Server* serverObj, int isAdmin, std::vector<std::string> words)
{
        std::map<std::string, Channel>::iterator channelIt = serverObj->channels.find(words[1]);
    	if (channelIt != serverObj->channels.end()) 
    	{
    	std::map<std::string, Channel> channels = serverObj->channels;
    	bool found = false;
    	std::map<int, Client*> channelClients = channels[words[1]].getClients();
    	std::map<int, Client*>::iterator toKickIt;
    	for (toKickIt = channelClients.begin(); toKickIt != channelClients.end(); toKickIt++)
    	{
                    std::cout << toKickIt->second->getNickname() << std::endl;
    		std::cout << words.size() << std::endl;
    	    std::cout << "-->" + words[2] << std::endl;
    		if (toKickIt->second->getNickname() == words[2])
    		{
                    std::cout << "entering kick in if \n";
    			found = true;
    			break;
    		}
    	}
    	if (found)
    	{
    	    std::cout << "kick in found \n";
            std::string channelName = serverObj->channels[words[1]].getName();
            std::map<std::string, Channel>::iterator channelIt = serverObj->channels.find(channelName);
            if (channelIt != serverObj->channels.end())
            {
                std::map<int, Client*>::iterator adminIt = serverObj->channels[channelName].operators.find(isAdmin);
                if (adminIt != serverObj->channels[channelName].operators.end())
                {
                    serverObj->channels[channelName].clients.erase(toKickIt->first);
                    std::map<int, Client*>::iterator isInvIt = serverObj->channels[channelName].inviteList.find(toKickIt->first);
                    if (isInvIt != serverObj->channels[channelName].inviteList.end())
                        serverObj->channels[channelName].inviteList.erase(toKickIt->first);
                        std::map<int, Client*>::iterator isOperIt = serverObj->channels[channelName].operators.find(toKickIt->first);
                    if (isOperIt != serverObj->channels[channelName].operators.end())
                        serverObj->channels[channelName].operators.erase(toKickIt->first);
                        serverObj->client[toKickIt->first]->eraseClientChannel(channelName);
                    std::map<int, Client*> members = serverObj->channels[channelName].getClients();
                    for (std::map<int, Client*>::iterator it = members.begin(); it != members.end(); ++it) 
                    {
                        std::string message = ":" + serverObj->channels[channelName].operators[isAdmin]->getNickname() + "!" + serverObj->channels[channelName].operators[isAdmin]->getUsername() + "@" + serverObj->client[isAdmin]->getLocation() + " KICK #1 " + serverObj->client[toKickIt->first]->getNickname() + " :" + serverObj->channels[channelName].operators[isAdmin]->getNickname() + "\r\n";
                        send(it->first, message.c_str(), message.size(), 0);
                    }
                    std::string message = ":" + serverObj->client[isAdmin]->getNickname() + "!" + serverObj->client[isAdmin]->getUsername() + "@" + serverObj->client[isAdmin]->getLocation() + " KICK #1 " + serverObj->client[toKickIt->first]->getNickname() + " :" + serverObj->client[isAdmin]->getNickname() + "\r\n";
                    send(toKickIt->first, message.c_str(), message.size(), 0);
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
        else
        {
            std::string message = ":  401 " + serverObj->client[isAdmin]->getNickname() + " " + words[2] + " :No such nick\r\n";
       	    send(isAdmin, message.c_str(), message.size(), 0);
        }   
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

void Commands::mode(Server* serverObj, int isAdmin, std::vector<std::string> words)
{
    std::map<std::string, Channel>::iterator channelIt = serverObj->channels.find(words[1]);
	if (channelIt != serverObj->channels.end()) 
	{
        std::map<int, Client*>::iterator it = serverObj->channels[words[1]].operators.find(isAdmin);
        if (it != serverObj->channels[words[1]].operators.end())
        {
            if (words.size() == 2)
            {
                std::vector<std::string> channel_modes = serverObj->channels[words[1]].getModes();
                std::vector<std::string>::iterator it_modes = channel_modes.begin();
                std::string modes_str = "+";
                for (long unsigned i = 0; i < channel_modes.size(); i++)
                    modes_str += it_modes->c_str();
                std::string modeMessage = ": 324 " + it->second->getNickname() + "!" + it->second->getUsername() + " " + serverObj->channels[words[1]].getName() + " +n\r\n";
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
                            serverObj->channels[words[1]].key = words[3];
                            serverObj->channels[words[1]].modes.push_back("k");
                            std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + serverObj->channels[words[1]].getName() + " +k " + words[3].c_str() + "\r\n";
                            send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
                        }
                        else if (words[2][i] == 't')
                        {
                            std::cout << "Inside topic in for in mode \n";
                            serverObj->channels[words[1]].modes.push_back("t");
                            std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + serverObj->channels[words[1]].getName() + " +t\r\n";
                            send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
                        }
                        else if (words[2][i] == 'i')
                        {
                            serverObj->channels[words[1]].modes.push_back("i");
        
                            std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + serverObj->channels[words[1]].getName() + " +i\r\n";
                            send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
                        }
                        else if (words[2][i] == 'l')
                        {
                            serverObj->channels[words[1]].limit = atoi(words[3].c_str());
                            serverObj->channels[words[1]].modes.push_back("l");
                            std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + serverObj->channels[words[1]].getName() + " +l " + words[3].c_str() + "\r\n";
                            send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
                        }
                        else if (words[2][i] == 'o')
                        {
                            std::map<int, Client*> clients = serverObj->channels[words[1]].getClients();
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
                                std::map<int, Client*> operators = serverObj->channels[words[1]].getOperators();
                                std::map<int, Client*>::iterator isAlreadyOper = operators.find(clientFd);
                                if (isAlreadyOper == operators.end())
                                {
                                    std::cout << "******** fuck off ********\n";
                                    serverObj->channels[words[1]].setOperator(clientFd, clients[clientFd]);
                                    std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + serverObj->channels[words[1]].getName() + " +o " + words[3].c_str() + "\r\n";
                                    send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
                                }
                                std::cout << "---------Operators-IN-Kick--------\n";
                                std::map<int, Client*> admins = serverObj->channels[words[1]].getOperators();
                                std::map<int, Client*>::iterator it_operator;
                                for (it_operator = admins.begin(); it_operator != admins.end(); ++it_operator)
                                {
                                    std::cout << "*** kick operators now (" << it_operator->second->getNickname() << ")***\n";
                                }
                            }
                        }
                    }
                    else if (sign == 2) ////////////////
                    {
                        if (words[2][i] == 'k')
                        {
                            std::vector<std::string>::iterator iter;
                            for (iter = serverObj->channels[words[1]].modes.begin(); iter != serverObj->channels[words[1]].modes.end(); ++iter)
                            {
                                if (*iter == "k")
                                {
                                    serverObj->channels[words[1]].deleteMode("k");
                                    break;
                                }
                            } 
                            serverObj->channels[words[1]].key.clear();
                            std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + serverObj->channels[words[1]].getName() + " -k " + words[3].c_str() + "\r\n";
                            send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
                        }
                        else if (words[2][i] == 'i')
                        {
                            std::vector<std::string> mods = serverObj->channels[words[1]].getModes();
                            std::vector<std::string>::iterator iter;
                            for (iter = mods.begin(); iter != mods.end(); ++iter)
                            {
                                if (*iter == "i")
                                {
                                    serverObj->channels[words[1]].deleteMode("i");
                                    break;
                                }
                            }
                            std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + serverObj->channels[words[1]].getName() + " -i\r\n";
                            send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
                        }
                        else if (words[2][i] == 'l')
                        {
                            std::vector<std::string>::iterator iter;
                            for (iter = serverObj->channels[words[1]].modes.begin(); iter != serverObj->channels[words[1]].modes.end(); ++iter)
                            {
                                if (*iter == "l")
                                {
                                    serverObj->channels[words[1]].deleteMode("l");
                                    break;
                                }
                            } 
                            serverObj->channels[words[1]].setLimit(0);
                            std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + serverObj->channels[words[1]].getName() + " -l \r\n";
                            send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
                        }
                        else if (words[2][i] == 'o')
                        {
                            std::map<int, Client*> clients = serverObj->channels[words[1]].getClients();
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
                            std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + serverObj->channels[words[1]].getName() + " -o " + words[3].c_str() + "\r\n";
                            send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
                            serverObj->channels[words[1]].operators.erase(clientFd);
                        }
                    }//end of else if -
                }
            }//end if words[2].empty
            
        }
        else
        {
            if (words.size() != 2)
            {
                std::string message = ": 482 " + serverObj->channels[words[1]].clients[isAdmin]->getNickname() + " #1 :You're not channel operator \r\n";
                send(isAdmin, message.c_str(), message.size(), 0);
            }
        }
	}
}

Commands::~Commands() {
}
