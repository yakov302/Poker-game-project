#include "cli_cmd.hpp"

namespace poker
{

#define MAX_NUM_OF_WORDS_IN_CMD 4

bool dbg[NUM_OF_DBG_TYPES] = {false};
std::string cli_input[MAX_NUM_OF_WORDS_IN_CMD];

namespace impl
{

void print_dbg_types()
{
    std::cout                                                  << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "debug types: "                               << std::endl;
    std::cout << "0  - for ACTION_IN"                          << std::endl;
    std::cout << "1  - for ACTION_OUT"                         << std::endl;
    std::cout << "2  - for BUTTON"                             << std::endl;
    std::cout << "3  - for CARD"                               << std::endl;
    std::cout << "4  - for CARD_CONTAINER"                     << std::endl;
    std::cout << "5  - for CHIP"                               << std::endl;
    std::cout << "6  - for CHIP_CONTAINER"                     << std::endl;
    std::cout << "7  - for CLIENT_MANAGER"                     << std::endl;
    std::cout << "8  - for CLIENT_SOCKET"                      << std::endl;
    std::cout << "9  - for HAND"                               << std::endl;
    std::cout << "10 - for PLAYER"                             << std::endl;
    std::cout << "11 - for PLAYERS_CONTAINER"                  << std::endl;
    std::cout << "12 - for RAUTER"                             << std::endl;
    std::cout << "13 - for SELF"                               << std::endl;
    std::cout << "14 - for SHAPE"                              << std::endl;
    std::cout << "15 - for SOUND"                              << std::endl;
    std::cout << "16 - for TABLE"                              << std::endl;
    std::cout << "17 - for TCP_CLIENT"                         << std::endl;
    std::cout << "18 - for TEXT_BOX"                           << std::endl;
    std::cout << "19 - for TEXT"                               << std::endl;
    std::cout << "20 - for WALLET"                             << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    std::cout                                                  << std::endl;
}

void turn_on_dbg(int type)
{
    if(type >= 0 && type < NUM_OF_DBG_TYPES)
    {
        std::cout << "turn on dbg[" << type << "]" << std::endl;
        dbg[type] = true;
    }
    else
    {
        std::cout << "invalid debug type!" << std::endl;
        print_dbg_types();
    }
}

void turn_on_all()
{
    std::cout << "turn on all dbg types" << std::endl;
    for(int i = 0; i < NUM_OF_DBG_TYPES; ++i)
        dbg[i] = true;
}

void turn_off_dbg(int type)
{
    if(type >= 0 && type < NUM_OF_DBG_TYPES)
    {
        std::cout << "turn off dbg[" << type << "]" << std::endl;
        dbg[type] = false;
    }
    else
    {
        std::cout << "invalid debug type!" << std::endl;
        print_dbg_types();
    }
}

void turn_off_all()
{
    std::cout << "turn off all dbg types" << std::endl;
    for(int i = 0; i < NUM_OF_DBG_TYPES; ++i)
        dbg[i] = false;
}

void prints_cmds()
{
    std::cout << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "invalid!"                                    << std::endl;
    std::cout << "commands syntax: "                           << std::endl;
    std::cout << " * logout          : logout"                 << std::endl;
    std::cout << " * debug types     : dbg ?"                  << std::endl;
    std::cout << " * debug on/off    : dbg <on/off> <type>"    << std::endl;
    std::cout << " * debug on/off all: dbg <on/off> all"       << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << std::endl;
}

void line_to_words(std::string& line)
{
    int i = 0;
    std::istringstream processing(line);
    while (processing >> cli_input[i]) 
    {
        ++i;
        if(i == MAX_NUM_OF_WORDS_IN_CMD)
            break;
    }
}


}//impl namespace

bool get_cmd(std::string& cmd)
{
    impl::line_to_words(cmd);

    if(cli_input[0] == "logout")
        return true;
    else if(cli_input[0] == "dbg")
    {
        if(cli_input[1] == "?")
            impl::print_dbg_types();
        else if(cli_input[1] == "on")
        {
            if(cli_input[2] == "all")
                impl::turn_on_all();
            else
                impl::turn_on_dbg(std::stoi(cli_input[2]));
        }
        else if(cli_input[1] == "off")
        {
            if(cli_input[2] == "all")
                impl::turn_off_all();
            else
                impl::turn_off_dbg(std::stoi(cli_input[2]));
        }
        else
            impl::prints_cmds();
    }
    else
        impl::prints_cmds();

    return false;
}


}// poker namespace
