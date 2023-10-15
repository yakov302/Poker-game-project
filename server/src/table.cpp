#include "table.hpp"

namespace poker
{

extern bool dbg[NUM_OF_DBG_TYPES];

namespace impl
{

void erase_chip(std::vector<int>& a_chips, int chip)
{
    auto it = std::find(a_chips.begin(), a_chips.end(), chip);
    
    if (it != a_chips.end()) 
        a_chips.erase(it);
}

void exchange(std::vector<int>& a_chips, int a_amount)
{
    switch (a_amount)
    {    
    case 5:
        for(int i = 0; i < 5; ++i)
            a_chips.push_back(1);    
        erase_chip(a_chips, 5);
        break;
    
    case 10:
        for(int i = 0; i < 2; ++i)
            a_chips.push_back(5);   
        erase_chip(a_chips, 10);
        break;

    case 25:
        for(int i = 0; i < 2; ++i)
            a_chips.push_back(10);
        a_chips.push_back(5); 
        erase_chip(a_chips, 25);
        break;

    case 50:
        for(int i = 0; i < 2; ++i)
            a_chips.push_back(25);   
        erase_chip(a_chips, 50);
        break;

    case 100:
        for(int i = 0; i < 2; ++i)
            a_chips.push_back(50);   
        erase_chip(a_chips, 100);
        break;

    case 500:
        for(int i = 0; i < 5; ++i)
            a_chips.push_back(100);   
        erase_chip(a_chips, 500);
        break;

    case 1000:
        for(int i = 0; i < 2; ++i)
            a_chips.push_back(500);   
        erase_chip(a_chips, 1000);
        break;

    case 5000:
        for(int i = 0; i < 5; ++i)
            a_chips.push_back(1000);   
        erase_chip(a_chips, 5000);
        break;

    case 10000:
        for(int i = 0; i < 2; ++i)
            a_chips.push_back(5000);   
        erase_chip(a_chips, 10000);
        break;
    
    default:
        break;
    } 
}

}//impl namespace

Table::Table(TcpServer& a_tcp_server)
: m_chips_total_amount(0)
, m_action_out(a_tcp_server)
, m_chips()
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
    m_chips_total_amount += a_chip;
    m_chips.emplace_back(a_chip);
}

int Table::give_chip()
{
    if(m_chips.empty())[[unlikely]]
        return 0;

    int chip = m_chips[m_chips.size()-1];
    m_chips_total_amount -= chip;
    m_chips.pop_back();
    return chip;
}

bool Table::give_chips(std::vector<int>& chips, int amount)
{
    if(dbg[TABLE])[[unlikely]]
        std::cout << __func__ << "(): amount: " << amount << std::endl;  

    int total_amount = 0;

    if(m_chips.empty())
    {
        if(dbg[TABLE])[[unlikely]]
            std::cout << __func__ << "(): m_chips empty return false! "  << std::endl; 
        
        return false;
    }

    for(auto chip : m_chips)
    {
        if(total_amount + chip <= amount)
        {
            chips.emplace_back(chip);
            total_amount += chip;
            m_chips_total_amount -= chip;
            impl::erase_chip(m_chips, chip);

            if(dbg[TABLE])[[unlikely]]
                std::cout << __func__ << "(): give: " << chip <<  " total chips left: " << m_chips_total_amount << std::endl; 
        }
        else
        {
            if(dbg[TABLE])[[unlikely]]
                std::cout << __func__ << "(): call exchange(m_chips, " << chip << ")" << std::endl; 
            
            impl::exchange(m_chips, chip);
            return true;
        }
    }
    
    return true;
}

cardPointer Table::give_card()
{
    if(m_cards.empty())[[unlikely]]
        return nullptr;

    cardPointer card = m_cards[m_cards.size()-1];
    m_cards.pop_back();
    return card;
}

int Table::total_chips_amount()const
{
    return m_chips_total_amount;
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

void Table::send_new_player_existing_chips_and_cards(int a_client_socket)
{
    for(int chip : m_chips)
        m_action_out.table_get_chip(chip, a_client_socket);

    for(auto card : m_cards)
        m_action_out.table_get_card(card, a_client_socket);
}


}// poker namespace
