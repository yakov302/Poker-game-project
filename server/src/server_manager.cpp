#include "server_manager.hpp"

namespace poker
{

ServerManager::ServerManager(std::string a_server_ip, int a_servr_port)
: m_socket(a_server_ip, a_servr_port)
, m_tcp(m_socket)
, m_action_out(m_tcp)
, m_players(m_action_out)
, m_subscribs(m_action_out)
, m_action_in(m_tcp, m_action_out, m_players, m_subscribs)
, m_rauter(m_socket, m_tcp, m_action_in, m_players)
{

}


}// poker namespace
