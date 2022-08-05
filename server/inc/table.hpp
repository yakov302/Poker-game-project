# pragma once

#include <vector>

#include "card.hpp"

namespace poker
{

class Table
{
public:
    Table();

    int give_chip();
    void give_card();
    void get_chip(int a_chip);
    void get_card(Card a_card);

    bool is_hand_empty();
    bool is_wallet_empty();

    int num_of_card()const;
    int table_amount()const;

    std::vector<Card>& table_cards();

private:
    std::vector<int> m_chips;
    std::vector<Card> m_cards;
};


}// poker namespace
