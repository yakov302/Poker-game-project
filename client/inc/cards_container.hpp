#pragma once

#include <memory>
#include <fstream>
#include <unordered_map>

#include "card.hpp"

#define CARDS_CONFIG_FILE_PATH "./resources/txt/deck.txt"
#define BACK_CARD_IMAGE_PATH "./resources/images/cards/back.png"

namespace poker
{

using cardPointer = std::shared_ptr<Card>;
using CardsMap = std::unordered_map<int, cardPointer>;

class CardsContainer
{
public:
    CardsContainer();
    cardPointer give_card(std::string& a_suit, int a_number);

private:
    std::unordered_map<std::string, CardsMap> m_cards;
    cardPointer m_null;
};


}// poker namespace
