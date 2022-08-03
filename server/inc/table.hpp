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
    int pop_chip();
    std::vector<Card>& table_cards();
    int table_amount();
    void give_card();
    bool is_wallet_empty();
    bool is_hand_empty();

private:
    std::vector<Card> m_cards;
    std::vector<int> m_chips;
};


}// poker namespace
