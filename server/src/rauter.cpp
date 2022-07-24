#include "rauter.hpp"

namespace poker
{

namespace impl
{

static void* thread_function(void* arg)
{
	Rauter* rauter = static_cast<Rauter*> (arg);
	rauter->run();
	return 0;
}


} //namespace impl

Rauter::Rauter(ServerSocket& a_socket, TcpServer& a_tcp, ActionIn& a_action_in, PlayersContainer& a_players)
: m_stop(false)
, m_buffer(new char[1024])
, m_activity(0)
, m_socket(a_socket)
, m_tcp(a_tcp)
, m_action_in(a_action_in)
, m_players(a_players)
{
	m_thread = new std::thread(impl::thread_function, this);
}

Rauter::~Rauter()
{
	delete[] m_buffer;
	m_thread->join();
	delete m_thread;
}

void Rauter::run()
{
	while(!m_stop)
	{	
		
		std::cout << "------entered select-------\n";
		m_activity = select(1024, &m_socket.copy_of_source_fd(), 0, 0, 0);
		std::cout << "------exit select-------\n";
		if((m_activity < 0) && (errno != EINTR))
			fatal_error("Select fail!\n");

		if(m_socket.is_in_active_fd(m_socket.listen_socket()))
		{
			accept_new_client();
			m_activity--;
		}
		delete_deleted();
		take_care_exists_clients();
	}
}

void Rauter::delete_deleted()
{
	auto it = m_socket.deleted_socket().begin();
	auto end = m_socket.deleted_socket().end();

	while(it != end && !m_socket.deleted_socket().empty())
	{
		m_players.delete_player(*it);
		m_socket.delete_from_deleted_socket(it);
		++it;
	}
}

void Rauter::accept_new_client()
{
	if (m_socket.num_of_clients() > 1000)
		m_socket.delete_less_active_client();

	int client_socket = m_tcp.accept_new_client();
	if(client_socket == -1)
		fatal_error("Accept fail!\n");

	insert_client(client_socket);
}

void Rauter::insert_client(int a_client_socket)
{
	m_socket.enter_to_source_fd(a_client_socket);
	m_socket.enter_to_connected_socket(a_client_socket);
	m_socket.increase_num_of_clients();
}

void Rauter::take_care_exists_clients()
{
    auto it = m_socket.connected_socket().begin();
    auto end = m_socket.connected_socket().end();
		
	while((it != end) && (m_activity > 0)) 
	{	
        int client_socket = *it;
		if(m_socket.is_in_active_fd(client_socket))
		{
			if(!m_tcp.receive_from_client(client_socket, m_buffer))
				m_tcp.delete_client(client_socket, it);

            else
            {
                m_socket.move_client_to_front(it);
                m_action_in.get(m_buffer, client_socket);
            }
			
			--m_activity;
		}
		++it; 
	} 
}

void Rauter::stop()
{
    m_stop = true;
}

void Rauter::fatal_error(std::string a_fail)
{
	perror(a_fail.c_str());
	m_socket.close_all_clients_sockets();
    throw;
}


}//namespace home