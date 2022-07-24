#pragma once

#include <memory>
#include <fstream>
#include <vector>

#include "card.hpp"

namespace poker
{

using cardPointer = std::shared_ptr<Card>;

class Deck
{
public:
    Deck(int a_number_of_decks);

    void shuffle();
    void re_fill_decks();
    Card pop_card();

    bool empty()const;

private:
    std::vector<cardPointer> m_cards;
    std::vector<cardPointer> m_removed_cards;
};


}// poker namespace
