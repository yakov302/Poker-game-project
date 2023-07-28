#include "table.hpp"

namespace poker
{

extern bool dbg[NUM_OF_DBG_TYPES];

Table::Table()
: m_chips()
, m_cards()
{
    m_cards.reserve(NUM_OF_TABLE_CARDS);
    m_chips.reserve(NUM_OF_TABLE_CHIPS);
}

void Table::get_card(cardPointer a_card)
{
    m_cards.emplace_back(a_card);
}

void Table::get_chip(int a_chip)
{
    m_chips.emplace_back(a_chip);
}

int Table::give_chip()
{
    if(m_chips.empty())[[unlikely]]
        return 0;

    int chip = m_chips[m_chips.size()-1];
    m_chips.pop_back();
    return chip;
}

cardPointer Table::give_card()
{
    if(m_cards.empty())[[unlikely]]
        return nullptr;

    cardPointer card = m_cards[m_cards.size()-1];
    m_cards.pop_back();
    return card;
}

int Table::num_of_card()const
{
    return m_cards.size();
}

bool Table::is_wallet_empty()
{
    return m_chips.empty();
}

bool Table::is_hand_empty()
{
    return m_cards.empty();
}

std::vector<cardPointer>& Table::table_cards()
{
    return m_cards;
}


}// poker namespace
