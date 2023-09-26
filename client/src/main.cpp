#include <thread>

#include "client_manager.hpp"
#include "cli_cmd.hpp"
#include "wait.hpp"

using namespace poker;

bool main_loop = true;
std::string cli_cmd;
Wait main_loop_wait;

void* get_line(void* arg)
{
    while(main_loop)
    {
        std::getline(std::cin, cli_cmd);
        main_loop_wait.exit_wait();
        sleep(1);
    }

    return nullptr;
}

int main()
{
    poker::ClientManager client;

    std::thread get_cli_cmd(get_line, nullptr);

    while(main_loop) 
    {
        main_loop_wait.enter_wait();
        
        if(get_cmd(cli_cmd) || !main_loop)
        {
            client.stop();
            break;
        }
    }

    return 0;
}


