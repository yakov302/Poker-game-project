#include "table_manager.hpp"

namespace poker
{

extern bool dbg[NUM_OF_DBG_TYPES];

TableManager::TableManager(TcpServer& a_tcp)
: m_deck(1)
, m_table()
, m_actio_out(a_tcp)
, m_players(m_actio_out, m_deck)
, m_bet_round(m_players, m_actio_out, m_table, m_deck)
, m_card_round(m_players, m_table, m_actio_out, m_bet_round, m_deck)
, m_game(m_players, m_card_round, m_actio_out)
{

}

bool TableManager::is_table_full()
{
    if(dbg[TABLE_MANAGER])[[unlikely]]
        std::cout << __func__ << "(): " << "return (num_of_players: " << m_players.num_of_players() << ">= MAX_NUM_OF_PLAYERS: " << MAX_NUM_OF_PLAYERS << ")" << std::endl;

    return (m_players.num_of_players() >= MAX_NUM_OF_PLAYERS);
}

bool TableManager::is_table_empty()
{
    if(dbg[TABLE_MANAGER])[[unlikely]]
        std::cout << __func__ << "(): " << "return (num_of_players: " << m_players.num_of_players() << " <= 0)" << std::endl;

    return (m_players.num_of_players() <= 0);
}


}// poker namespace
