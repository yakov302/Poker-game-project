#include "client_manager.hpp"
#include <iostream>

namespace poker
{

ClientManager::ClientManager(std::string a_server_ip, int a_server_port)
: m_cards(2, 0, 850, 350, 1.8, 140)
, m_chips(745, 565, {})
, m_self("player", "male", 0, 1060, 650, 1280, 640, 1.8, 20)
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
