#pragma once

#include <string>
#include <sstream>
#include <iostream>

namespace poker
{

typedef enum
{
    PLAYERS_CONTAINER,
    PLAYER_MANAGER,
    TABLES_CONTAINER,
    TABLE_MANAGER,
    ACTION_IN,
    SUBSCRIBS,
    NUM_OF_DBG_TYPES
}DBG_TYPES;

bool get_cmd(std::string& cmd);


}// poker namespace
