# pragma once

#include <memory>
#include <vector>

#include "card.hpp"

namespace poker
{

using cardPointer = std::shared_ptr<Card>;

class Table
{
public:
    Table();

    int give_chip();
    cardPointer give_card();
    void get_chip(int a_chip);
    void get_card(cardPointer a_card);

    bool is_hand_empty();
    bool is_wallet_empty();

    int num_of_card()const;
    int table_amount()const;

    std::vector<cardPointer>& table_cards();

private:
    std::vector<int> m_chips;
    std::vector<cardPointer> m_cards;
};


}// poker namespace
