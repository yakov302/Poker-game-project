#pragma once

#include <string>
#include <sstream>
#include <iostream>

namespace poker
{

typedef enum
{
    ACTION_IN,
    ACTION_OUT,
    BET_ROUND,
    CARD_ROUND,
    CARD,
    DECK,
    GAME, 
    PLAYER,
    PLAYERS_CONTAINER,
    RAUTER,
    SUBSCRIBS,
    TABLE_MANAGER,
    PLAYER_MANAGER,
    TABLE,
    TABLES_CONTAINER,
    TCP_SEVER,
    WINNER,
    NUM_OF_DBG_TYPES
}DBG_TYPES;

bool get_cmd(std::string& cmd);


}// poker namespace
