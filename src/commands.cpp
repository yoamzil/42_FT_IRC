#include "../include/Commands.hpp"
#include "../include/client_channel/Channel.hpp"
#include "../include/client_channel/Client.hpp"

Commands::Commands() {
    // std::cout << "Default commands constructor called";
}

void Commands::kick(int isAdmin, int toKick, Channel* channel) {
    std::map<int, Client*>::iterator it = channel->operators.find(isAdmin);
    if (it != channel->operators.end())
    {
        channel->clients.erase(toKick);
        // :haarab!hamza@88ABE6.25BF1D.D03F86.88C9BD.IP KICK #1 rouali :haarab
        // :<admin_nick>!<admin_user>@<admin_host> KICK <channel> <kicked_nick> :<reason>
        // std::string message = ":" + channel->clients[isAdmin]->getNickname() + "!" + channel->clients[isAdmin]->getUsername() + "@localhost KICK " + channel->getName() + " " + channel->clients[toKick]->getNickname() + " :" + channel->clients[isAdmin]->getNickname() + " \r\n";
        // send(isAdmin, message.c_str(), message.size(), 0);
        // std::cout << "Bug removed succefully" << std::endl;
    }
    else
    {
        std::string message = ": 482 " + channel->clients[isAdmin]->getNickname() + " #1 :You're not channel operator \r\n";
        send(isAdmin, message.c_str(), message.size(), 0);
        // :luna.AfterNET.Org 482 rouali #1 :You're not channel operator
        // std::cout << "You have to be an Operator to proceed this operation" << std::endl;
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
            std::string message;
            // message = ":" + newMember->getNickname() + "!" + newMember->getUsername() + "@localhost MODE " + channel->getName() + " +i \r\n";
	        // send(isAdmin, message.c_str(), message.size(), 0);
            // message = ":" + channel->clients[isAdmin]->getNickname() + "!" + channel->clients[isAdmin]->getUsername() + "@localhost INVITE " + newMember->getNickname() + " :" + channel->getName() + " \r\n";
            // send(isAdmin, message.c_str(), message.size(), 0);
            // :irc.example.com 341 john alice #examplechannel
            message = ": 341 " + channel->clients[isAdmin]->getNickname() + " " + newMember->getNickname() + " :" + channel->getName() + " \r\n";
            send(isAdmin, message.c_str(), message.size(), 0);
        }
        else
        {
            std::string message = ": 482 " + newMember->getNickname() + " " + channel->getName() + " :You're not channel operator\r\n";
	        send(isAdmin, message.c_str(), message.size(), 0);
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
                    std::string modeMessage = ":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + channel->getName() + " +k " + words[3].c_str() + "\r\n";
                    send(isAdmin, modeMessage.c_str(), modeMessage.size(), 0);
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
                    // else
                    // {

                    //     std::cout << "New operator added --> " << clients[clientFd]->nickname << std::endl;
                    // }
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
    // std::cout << "Default commands Destructor called" << std::endl;
}
