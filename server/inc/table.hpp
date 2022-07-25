# pragma once

#include <vector>

#include "card.hpp"

namespace poker
{

class Table
{
public:
    Table();

    int num_of_card()const;
    void get_card(Card a_card);
    void get_chip(int a_chip);
    std::vector<Card>& table_cards();
    std::vector<int>& table_chips();
    void clear_cards();
    void clear_chips();

private:
    std::vector<Card> m_cards;
    std::vector<int> m_chips;
};


}// poker namespace
