#pragma once

#include <algorithm>

#include "card.hpp"
#include "players_container.hpp"

namespace poker
{

typedef enum cards_combinations
{
    HIGH_CARD = 1,
    ONE_PAIR = 2, 
    TWO_PAIR = 3,
    THREE_OF_A_KING = 4,
    STRAIGHT = 5, 
    FLUSH = 6, 
    FULL_HOUSE = 7,
    FOR_OF_A_KING = 8,
    STRAIGHT_FLUSH = 9,
    ROYAL_STRAIGHT_FLUSH = 10,

}cards_combinations;

std::string chack_winner(PlayersContainer& a_players, std::vector<Card>& a_table_card);


}//namespace poker
