#include "../include/Commands.hpp"
#include "../include/client_channel/Channel.hpp"
#include "../include/client_channel/Client.hpp"

Commands::Commands() {
}


int userFd(Server* serverObj, std::vector<std::string> words)
{
   	std::map<int, Client*>::iterator toKickIt;
   	for (toKickIt = serverObj->channels[words[1]].clients.begin(); toKickIt != serverObj->channels[words[1]].clients.end(); toKickIt++)
   	{
  		if (toKickIt->second->getNickname() == words[2])
  		{
 			return (toKickIt->first);
 			break;
  		}
   	}
    return (0);
}

void sendKickMessage(Server* serverObj, int isAdmin, std::vector<std::string> words, int tokick)
{
    std::string reason = ":Kicked for no reason *_*";
    if (words.size() == 4 && !words[3].empty())
        reason = words[3].c_str();
    for (std::map<int, Client*>::iterator it = serverObj->channels[words[1]].clients.begin(); it != serverObj->channels[words[1]].clients.end(); ++it) 
    {
        std::string message = ":" + serverObj->mapClient[isAdmin]->getNickname() + "!" + serverObj->mapClient[isAdmin]->getUsername() + "@" + serverObj->mapClient[isAdmin]->getLocation() + " KICK " + words[1] + " " + serverObj->mapClient[tokick]->getNickname() + " " + reason + "\r\n";
        send(it->first, message.c_str(), message.size(), 0);// :e!eeee@88ABE6.25BF1D.D03F86.88C9BD.IP KICK #3 d :test
    }
    std::string message = ":" + serverObj->mapClient[isAdmin]->getNickname() + "!" + serverObj->mapClient[isAdmin]->getUsername() + "@" + serverObj->mapClient[isAdmin]->getLocation() + " KICK " + words[1] + " " + serverObj->mapClient[tokick]->getNickname() + " " + reason + "\r\n";
    send(tokick, message.c_str(), message.size(), 0);
}

void Commands::kick(Server* serverObj, int isAdmin, std::vector<std::string> words)
{
    std::map<std::string, Channel>::iterator channelIt = serverObj->channels.find(words[1]);
   	if (channelIt != serverObj->channels.end()) 
   	{
        int tokick = userFd(serverObj, words);
       	if (tokick != 0)
       	{
            std::map<std::string, Channel>::iterator channelIt = serverObj->channels.find(words[1]);
            if (channelIt != serverObj->channels.end())
            {
                std::map<int, Client*>::iterator adminIt = serverObj->channels[words[1]].operators.find(isAdmin);
                if (adminIt != serverObj->channels[words[1]].operators.end())
                {
                    std::map<int, Client*>::iterator isInvIt = serverObj->channels[words[1]].inviteList.find(tokick);
                    if (isInvIt != serverObj->channels[words[1]].inviteList.end())
                        serverObj->channels[words[1]].inviteList.erase(tokick);
                    std::map<int, Client*>::iterator isOperIt = serverObj->channels[words[1]].operators.find(tokick);
                    if (isOperIt != serverObj->channels[words[1]].operators.end())
                        serverObj->channels[words[1]].operators.erase(tokick);
                    serverObj->channels[words[1]].clients.erase(tokick);
                    serverObj->mapClient[tokick]->eraseClientChannel(words[1]);
                    sendKickMessage(serverObj, isAdmin, words, tokick);
                }
                else
                {
                    std::string message = ": 482 " + serverObj->mapClient[isAdmin]->getNickname() + " #1 :You're not channel operator \r\n";
                    send(isAdmin, message.c_str(), message.size(), 0);
                    return ;
                }
            }
        }
        else
        {
            std::string message = ":  401 " + serverObj->mapClient[isAdmin]->getNickname() + " " + words[2] + " :No such nick\r\n";
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
        for (newMemberIt = serverObj->mapClient.begin(); newMemberIt != serverObj->mapClient.end(); newMemberIt++)
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
				for (std::map<int, Client*>::iterator it = serverObj->channels[words[1]].clients.begin(); it != serverObj->channels[words[1]].clients.end(); ++it) 
				{
					std::string message = ": 331 " + it->second->getNickname() + " " + words[1] + " :No topic is set.\r\n";
					send(it->first, message.c_str(), message.size(), 0);
				}
            }
            else
            {
				for (std::map<int, Client*>::iterator it = serverObj->channels[words[1]].clients.begin(); it != serverObj->channels[words[1]].clients.end(); ++it) 
				{
					std::string message = ": 332 " + it->second->getNickname() + " " + words[1] + " :" + topic + "\r\n";
					send(it->first, message.c_str(), message.size(), 0);
				}
            }
        }
        else if (channelIt->second.find_mode("t"))
        {
            std::map<int, Client*>::iterator it = channelIt->second.operators.find(isAdmin);
            if (it != channelIt->second.operators.end())
            {
                std::string newTopic = words[2].substr(1); 
                channelIt->second.setTopic(newTopic);
				for (std::map<int, Client*>::iterator it = serverObj->channels[words[1]].clients.begin(); it != serverObj->channels[words[1]].clients.end(); ++it) 
				{
					std::string message = ":" + it->second->getNickname() + "!" + it->second->getUsername() + "@" + it->second->getLocation() + " TOPIC " + words[1] + " :" + newTopic + "\r\n";
					send(it->first, message.c_str(), message.size(), 0);
				}
            }
            else
            {
				for (std::map<int, Client*>::iterator it = serverObj->channels[words[1]].clients.begin(); it != serverObj->channels[words[1]].clients.end(); ++it) 
				{
					std::string message = ": 482 " + it->second->getNickname() + " " + words[1] + " :You're not channel operator \r\n";
					send(it->first, message.c_str(), message.size(), 0);
				}
            }
        }
        else 
        {
            std::string newTopic = words[2].substr(1); 
            channelIt->second.setTopic(newTopic);
			for (std::map<int, Client*>::iterator it = serverObj->channels[words[1]].clients.begin(); it != serverObj->channels[words[1]].clients.end(); ++it) 
			{
				std::string message = ":" + it->second->getNickname() + "!" + it->second->getUsername() + "@" + it->second->getLocation() + " TOPIC " + words[1] + " :" + newTopic + "\r\n";
				send(it->first, message.c_str(), message.size(), 0);
			}
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
                    if (sign == 1)
                    {
                        if (words[2][i] == 'k')
                        {
                            serverObj->channels[words[1]].key = words[3];
                            serverObj->channels[words[1]].modes.push_back("k");
                            for (std::map<int, Client*>::iterator it = serverObj->channels[words[1]].clients.begin(); it != serverObj->channels[words[1]].clients.end(); ++it) 
                            {
                                std::string message = ":" + it->second->getNickname() + "!" + it->second->getUsername() + "@" + serverObj->mapClient[isAdmin]->getLocation() + " MODE " + serverObj->channels[words[1]].getName() + " +k " + words[3].c_str() + "\r\n";
                                send(it->first, message.c_str(), message.size(), 0);
                            }
                        }
                        else if (words[2][i] == 't')
                        {
                            serverObj->channels[words[1]].modes.push_back("t");
                            for (std::map<int, Client*>::iterator it = serverObj->channels[words[1]].clients.begin(); it != serverObj->channels[words[1]].clients.end(); ++it) 
                            {
                                std::string message = ":" + it->second->getNickname() + "!" + it->second->getUsername() + "@" + serverObj->mapClient[isAdmin]->getLocation() + " MODE " + serverObj->channels[words[1]].getName() + " +t\r\n";
                                send(it->first, message.c_str(), message.size(), 0);
                            }
                        }
                        else if (words[2][i] == 'i')
                        {
                            serverObj->channels[words[1]].modes.push_back("i");
                            for (std::map<int, Client*>::iterator it = serverObj->channels[words[1]].clients.begin(); it != serverObj->channels[words[1]].clients.end(); ++it) 
                            {
                                std::string message = ":" + it->second->getNickname() + "!" + it->second->getUsername() + "@" + serverObj->mapClient[isAdmin]->getLocation() + " MODE " + serverObj->channels[words[1]].getName() + " +i\r\n";
                                send(it->first, message.c_str(), message.size(), 0);
                            }
                        }
                        else if (words[2][i] == 'l')
                        {
                            serverObj->channels[words[1]].limit = atoi(words[3].c_str());
                            serverObj->channels[words[1]].modes.push_back("l");
                            for (std::map<int, Client*>::iterator it = serverObj->channels[words[1]].clients.begin(); it != serverObj->channels[words[1]].clients.end(); ++it) 
                            {
                                std::string message = ":" + it->second->getNickname() + "!" + it->second->getUsername() + "@" + serverObj->mapClient[isAdmin]->getLocation() + " MODE " + serverObj->channels[words[1]].getName() + " +l " + words[3] + "\r\n";
                                send(it->first, message.c_str(), message.size(), 0);
                            }
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
                                std::map<int, Client*>::iterator isAlreadyOper = serverObj->channels[words[1]].operators.find(clientFd);
                                if (isAlreadyOper == serverObj->channels[words[1]].operators.end())
                                {
                                    serverObj->channels[words[1]].setOperator(clientFd, serverObj->channels[words[1]].clients[clientFd]);
                                    for (std::map<int, Client*>::iterator it = serverObj->channels[words[1]].clients.begin(); it != serverObj->channels[words[1]].clients.end(); ++it) 
                                    {
                                        std::string message = ":" + it->second->getNickname() + "!" + it->second->getUsername() + "@" + serverObj->mapClient[isAdmin]->getLocation() + " MODE " + serverObj->channels[words[1]].getName() + " +o " + words[3] + "\r\n";
                                        send(it->first, message.c_str(), message.size(), 0);
                                    }
                                }
                                else 
                                {
                                    for (std::map<int, Client*>::iterator it = serverObj->channels[words[1]].clients.begin(); it != serverObj->channels[words[1]].clients.end(); ++it) 
                                    {
                                        std::string message = ":" + it->second->getNickname() + "!" + it->second->getUsername() + "@" + serverObj->mapClient[isAdmin]->getLocation() + " MODE " + serverObj->channels[words[1]].getName() + " +o " + words[3] + "\r\n";
                                        send(it->first, message.c_str(), message.size(), 0);
                                    }
                                }
                            }
                        }
                    }
                    else if (sign == 2)
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
                            for (std::map<int, Client*>::iterator it = serverObj->channels[words[1]].clients.begin(); it != serverObj->channels[words[1]].clients.end(); ++it) 
                            {
                                std::string message = ":" + it->second->getNickname() + "!" + it->second->getUsername() + "@" + serverObj->mapClient[isAdmin]->getLocation() + " MODE " + serverObj->channels[words[1]].getName() + " -k " + words[3].c_str() + "\r\n";
                                send(it->first, message.c_str(), message.size(), 0);
                            }
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
                            for (std::map<int, Client*>::iterator it = serverObj->channels[words[1]].clients.begin(); it != serverObj->channels[words[1]].clients.end(); ++it) 
                            {
                                std::string message = ":" + it->second->getNickname() + "!" + it->second->getUsername() + "@" + serverObj->mapClient[isAdmin]->getLocation() + " MODE " + serverObj->channels[words[1]].getName() + " -i\r\n";
                                send(it->first, message.c_str(), message.size(), 0);
                            }
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
                            for (std::map<int, Client*>::iterator it = serverObj->channels[words[1]].clients.begin(); it != serverObj->channels[words[1]].clients.end(); ++it) 
                            {
                                std::string message = ":" + it->second->getNickname() + "!" + it->second->getUsername() + "@" + serverObj->mapClient[isAdmin]->getLocation() + " MODE " + serverObj->channels[words[1]].getName() + " -l \r\n";
                                send(it->first, message.c_str(), message.size(), 0);
                            }
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
                            for (std::map<int, Client*>::iterator it = serverObj->channels[words[1]].clients.begin(); it != serverObj->channels[words[1]].clients.end(); ++it) 
                            {
                                std::string message = ":" + it->second->getNickname() + "!" + it->second->getUsername() + "@" + serverObj->mapClient[isAdmin]->getLocation() + " MODE " + serverObj->channels[words[1]].getName() + " -o " + words[3].c_str() + "\r\n";
                                send(it->first, message.c_str(), message.size(), 0);
                            }
                            serverObj->channels[words[1]].operators.erase(clientFd);
                        }
						else if (words[2][i] == 't')
                        {
							for (std::vector<std::string>::iterator iter = serverObj->channels[words[1]].modes.begin(); iter != serverObj->channels[words[1]].modes.end(); ++iter)
                            {
                                if (*iter == "t")
                                {
                                    serverObj->channels[words[1]].deleteMode("t");
                                    break;
                                }
                            } 
                            for (std::map<int, Client*>::iterator it = serverObj->channels[words[1]].clients.begin(); it != serverObj->channels[words[1]].clients.end(); ++it) 
                            {
                                std::string message = ":" + it->second->getNickname() + "!" + it->second->getUsername() + "@" + serverObj->mapClient[isAdmin]->getLocation() + " MODE " + serverObj->channels[words[1]].getName() + " -t\r\n";
                                send(it->first, message.c_str(), message.size(), 0);
                            }
                        }
                    }
                }
            }
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