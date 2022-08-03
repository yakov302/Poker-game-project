#include "client_manager.hpp"

namespace poker
{

ClientManager::ClientManager(std::string a_server_ip, int a_server_port)
: m_cards(5, 0, 850, 350, 1.8, 150, 0)
, m_chips(745, 565, {})
, m_self("player", "male", 0, 1060, 650, 1260, 630, 1.8, 30, 15)
, m_players()
, m_socket(a_server_ip, a_server_port)
, m_tcp(m_socket)
, m_action_out(m_tcp)
, m_table(m_cards, m_chips, m_self , m_players, m_action_out)
, m_action_in(m_cards, m_chips, m_players, m_table, m_self, m_action_out)
, m_router(m_socket, m_action_in, m_tcp, m_table)
{

}


}// poker namespace
