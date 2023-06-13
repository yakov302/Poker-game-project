#pragma once

#include <memory>
#include <fstream>
#include <vector>

#include "card.hpp"

#define NUM_OF_CARDS_IN_DECK 52
#define DECK_CONFIG_FILE_PATH "./resources/deck.txt"

namespace poker
{

using cardPointer = std::shared_ptr<Card>;

class Deck
{
public:
    Deck(int a_number_of_decks);

    void shuffle();
    cardPointer pop_card();
    void push_card(cardPointer card);

    bool empty()const;

private:
    std::vector<cardPointer> m_cards;
};


}// poker namespace
