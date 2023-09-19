#include "client_manager.hpp"
#include "cli_cmd.hpp"

using namespace poker;

int main()
{
    poker::ClientManager client;

    std::string cli_cmd;
    while(true)
    {
        std::getline(std::cin, cli_cmd);
        if(get_cmd(cli_cmd))
        {
            client.stop();
            break;
        }
    }

    return 0;
}


