#include "server_manager.hpp"

namespace poker
{

ServerManager::ServerManager()
: m_socket()
, m_tcp(m_socket)
, m_tables_container(m_tcp)
, m_subscribs(m_tcp)
, m_action_in(m_tcp, m_subscribs, m_tables_container)
, m_rauter(m_socket, m_tcp, m_action_in, m_tables_container)
{

}

void ServerManager::stop()
{
    m_rauter.stop();
    char wake_up_router[1];
    m_tables_container.stop();
    m_tcp.send_to_client(m_socket.listen_socket(), wake_up_router, 1);
}


}// poker namespace
