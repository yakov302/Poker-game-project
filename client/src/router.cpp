#include"router.hpp"
 #include <unistd.h>
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
: m_buffer(new char[1024])
, m_socket(a_socket)
, m_tcp(a_tcp)
, m_action_in(a_action_in)
, m_table(a_table)
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
        std::cout << "------entered select-------\n";
        select(1024, &m_socket.fd(), 0, 0, 0);
        std::cout << "------exit select-------\n";

        m_tcp.receive_from_server(m_buffer);
        m_action_in.get(m_buffer);
    }
}


}// poker namespace
