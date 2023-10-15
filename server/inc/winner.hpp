#pragma once

#include <algorithm>

#include "card.hpp"
#include "cli_cmd.hpp"
#include "players_container.hpp"

namespace poker
{

using cardPointer = std::shared_ptr<Card>;

void chack_winner(std::vector<std::string>& a_winners, PlayersContainer& a_players, std::vector<cardPointer>& a_table_card);


}//namespace poker
