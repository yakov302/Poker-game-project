#include "table.hpp"

namespace poker
{

Table::Table()
: m_cards()
, m_chips()
{
    m_cards.reserve(5);
    m_chips.reserve(200);
}

int Table::num_of_card()const
{
    return m_cards.size();
}

void Table::get_card(Card a_card)
{
    m_cards.emplace_back(a_card);
}

void Table::get_chip(int a_chip)
{
    m_chips.emplace_back(a_chip);
}

std::vector<Card>& Table::table_cards()
{
    return m_cards;
}

std::vector<int>& Table::table_chips()
{
    return m_chips;
}

void Table::clear_cards()
{
    m_cards.clear();
}

void Table::clear_chips()
{
    m_chips.clear();
}


}// poker namespace
