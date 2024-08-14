#include "../include/Server.hpp"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return (1);
    }

    int             port        = std::atoi(argv[1]);
    std::string     password    = argv[2];

    Server      ircServer(port, password);
	Server*		serverObj = &ircServer;
    ircServer.start(serverObj);


    return (0);
}
