#pragma once

#include <algorithm>

#include "card.hpp"
#include "players_container.hpp"

namespace poker
{

std::string chack_winner(PlayersContainer& a_players, std::vector<Card>& a_table_card);


}//namespace poker
