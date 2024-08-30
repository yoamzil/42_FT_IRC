#include "../../include/client_channel/ModeUser.hpp"

ModeUser::ModeUser() {
}

ModeUser::~ModeUser() {
}

void ModeUser::Join(Server* serverObj, Client* clientObj, int clientSocket, std::vector<std::string> & words) {
	if (words[0] == "JOIN" && !words[1].empty() && clientObj->getStatus() == 1)
	{
		if (words[1][0] == '#')
		{
			clientObj->joinChannel(serverObj, clientSocket, words[1], words);
		} else if (words[1][0] != '#')
		{
			std::string message = ": 403 * " + clientObj->getNickname() + " " + words[1] + " :No such channel\r\n";
			clientObj->sendMessage(clientSocket, message);
		}
	}
	else if (words[0] == "JOIN" && words[1].empty())
	{
		std::string message = ": 461 * JOIN :Not enough parameters\r\n";
		clientObj->sendMessage(clientSocket, message);
	}
}

void ModeUser::Part(Server* serverObj, Client* clientObj, int clientSocket, std::vector<std::string> & words) {
	int j = 0;
	if (words[0] == "PART" && !words[1].empty() && clientObj->getStatus() == 1)
	{
		std::vector<std::string>::iterator it = std::find(clientObj->getChannelName().begin(), clientObj->getChannelName().end(), words[1]);
		if (it != clientObj->getChannelName().end())
		{
			j = 1;
		}
		if (words[1][0] == '#' && j == 1)
		{
			clientObj->leaveChannel(serverObj, clientSocket, words[1]);
		} else if (words[1][0] != '#')
		{
			std::string message = ": 403 * " + clientObj->getNickname() + " " + words[1] + " :No such channel\r\n";
			clientObj->sendMessage(clientSocket, message);
		}
		else if (j == 0)
		{
			std::string message = ": 442 * " + clientObj->getNickname() + " " + words[1] + " :You're not on that channel\r\n";
			clientObj->sendMessage(clientSocket, message);
		}
	}
	else if (words[0] == "PART" && words[1].empty())
	{
		std::string message = ": 461 * PART :Not enough parameters\r\n";
		clientObj->sendMessage(clientSocket, message);
	}
}

void ModeUser::Commande(Server* serverObj, Client* clientObj, int clientSocket, std::vector<std::string> & words) {
	// std::cout << words[0] << std::endl;
	
	if (words.empty())
		return; // Check for empty command

    typedef void (ModeUser::*CommandFunc)(Server*, Client*, int, std::vector<std::string>&);

    std::map<std::string, CommandFunc> commands;
    commands["JOIN"] = &ModeUser::Join;
    commands["PART"] = &ModeUser::Part;

    std::map<std::string, CommandFunc>::iterator it = commands.find(words[0]);
    if (it != commands.end()) {
        CommandFunc Func = it->second;
        (this->*Func)(serverObj, clientObj, clientSocket, words);
    }
	// else {
    //     std::cout << "Unknown command: " << words[0] << std::endl;
    // }
}

bool ModeUser::Check_Join() {
	return true;
}

bool ModeUser::Check_Part() {
	return true;
}

bool ModeUser::Check_Topic() {
	return true;
}

bool ModeUser::Check_Invite() {
	return true;
}

bool ModeUser::Check_Mode() {
	return true;
}

bool ModeUser::Check_Kick() {
	return true;
}



bool ModeUser::check_Comande(std::vector<std::string> & words) {
	std::cout << "hello : " <<  words[0] << std::endl;
	bool (ModeUser::*checkCommand[6])() = {&ModeUser::Check_Join, &ModeUser::Check_Part,
											&ModeUser::Check_Topic, &ModeUser::Check_Invite,
												&ModeUser::Check_Mode, &ModeUser::Check_Kick};
	
	std::string command[6] = {"JOIN", "PART", "TOPIC", "INVITE", "MODE", "KICK"};
	for (int i = 0; i < 6; i++)
	{
		if (words[0] == command[i])
		{
			return (this->*checkCommand[i])();
		}
	}
	return false;
}