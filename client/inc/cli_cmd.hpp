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
    BUTTON,
    CARD,
    CARD_CONTAINER,
    CHIP,
    CHIP_CONTAINER,
    CLIENT_MANAGER,
    CLIENT_SOCKET, 
    HAND,
    PLAYER,
    PLAYERS_CONTAINER,
    RAUTER,
    SELF,
    SHAPE,
    SOUND,
    TABLE,
    TCP_CLIENT,
    TEXT_BOX,
    TEXT,
    WALLET,
    NUM_OF_DBG_TYPES
}DBG_TYPES;

bool get_cmd(std::string& cmd);


}// poker namespace
