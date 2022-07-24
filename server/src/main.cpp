#include "server_manager.hpp"
#include "sha256.hpp"
#include "subscribs.hpp"
#include "tcp_server.hpp"
#include "deck.hpp"
#include <ctime>
#include <iostream>
#include <time.h>
#include "protocol.hpp"

using namespace poker;
int main()
{
    ServerManager server("127.0.0.1", 5555);
    return 0;
}