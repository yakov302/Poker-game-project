#include <ctime>
#include <time.h>
#include <iostream>

#include "server_manager.hpp"
#include "tcp_server.hpp"
#include "subscribs.hpp"
#include "protocol.hpp"
#include "sha256.hpp"
#include "deck.hpp"

using namespace poker;

int main()
{
    ServerManager server;

    char c;
    while(std::cin >> c)
    {
        if((int)c == 27)
        {
            server.stop();
            break;
        }
    }

    return 0;
}
