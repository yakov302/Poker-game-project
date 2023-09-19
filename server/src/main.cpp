#include "server_manager.hpp"
#include "cli_cmd.hpp"

using namespace poker;

int main()
{
    ServerManager server;

    std::string cli_cmd;
    while(true)
    {
        std::getline(std::cin, cli_cmd);
        if(get_cmd(cli_cmd))
        {
            server.stop();
            break;
        }
    }

    return 0;
}
