#include "server_manager.hpp"

namespace poker
{

ServerManager::ServerManager(std::string a_server_ip, int a_servr_port)
: m_socket(a_server_ip, a_servr_port)
, m_tcp(m_socket)
, m_action_out(m_tcp)
, m_players(m_action_out)
, m_subscribs(m_action_out)
, m_table()
, m_bet_round(m_players, m_action_out, m_table)
, m_card_round(m_players, m_table, m_action_out, m_bet_round)
, m_game(m_players, m_card_round, m_action_out)
, m_action_in(m_tcp, m_action_out, m_players, m_subscribs, m_bet_round)
, m_rauter(m_socket, m_tcp, m_action_in, m_players)
{

}

void ServerManager::stop()
{
    m_rauter.stop();
    m_action_out.wake_up_server();
}


}// poker namespace
