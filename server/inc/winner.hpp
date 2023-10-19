#pragma once

#include <algorithm>

#include "card.hpp"
#include "cli_cmd.hpp"
#include "players_container.hpp"

#define SMALL false
#define BIG true

namespace poker
{

using cardPointer = std::shared_ptr<Card>;
using CardsMap = std::unordered_map<std::string, std::vector<cardPointer>>;

void chack_winner(std::vector<std::string>& a_winners, PlayersContainer& a_players, std::vector<cardPointer>& a_table_card);


}//namespace poker
