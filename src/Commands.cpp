#include "../include/Commands.hpp"
#include "../include/Channel.hpp"

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

// void Commands::invite(int isAdmin, int newMember, Channel* channel) {

// }

// void Commands::topic(int isAdmin, std::string newTopic, Channel* channel) {

// }

// void Commands::mode(int isAdmin, std::string newMode, Channel* channel) {

// }

Commands::~Commands() {
    std::cout << "Default commands Destructor called";
}
