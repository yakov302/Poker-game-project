#include"router.hpp"

namespace poker
{

namespace impl
{

static void* thread_function(void* a_arg)
{
    Router* router = static_cast<Router*>(a_arg);
    router->run();
    return 0;
}


}//namespace impl

Router::Router(ClientSocket& a_socket, ActionIn& a_action_in, TcpClient& a_tcp, Table& a_table)
: m_buffer(new char[BUFFER_SIZE])
, m_table(a_table)
, m_tcp(a_tcp)
, m_action_in(a_action_in)
, m_socket(a_socket)
{
    m_thread = new std::thread(impl::thread_function, this);
}

Router::~Router()
{
    m_thread->join();
    delete m_thread;
    delete[] m_buffer;
}

void Router::run()
{
    while(m_table.is_runing())
    {
        select(MAX_SOCKET_NUM_TO_LISTEN, &m_socket.fd(), 0, 0, 0);

        if(!m_tcp.receive_from_server(m_buffer))
            m_table.stop();

        m_action_in.get(m_buffer);
    }
}


}// poker namespace
