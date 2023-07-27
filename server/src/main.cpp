#include <ctime>
#include <time.h>
#include <iostream>

#include "server_manager.hpp"
#include "tcp_server.hpp"
#include "subscribs.hpp"
#include "protocol.hpp"
#include "cli_cmd.hpp"
#include "sha256.hpp"
#include "deck.hpp"

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
