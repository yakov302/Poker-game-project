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

Rauter::Rauter(TcpServer& a_tcp, ActionIn& a_action_in, PlayersContainer& a_players)
: m_stop(false)
, m_buffer(new char[1024])
, m_activity(0)
, m_temp_sockets_bits()
, m_sin()
, m_source_sockets_bits(a_tcp.source_sockets_bits())
, m_listen_socket(a_tcp.listen_socket())
, m_num_of_clients(a_tcp.num_of_clients())
, m_clients_sockets(a_tcp.clients_sockets())
, m_deleted_sockets(a_tcp.deleted_sockets())
, m_tcp(a_tcp)
, m_action_in(a_action_in)
, m_players(a_players)
{
	FD_ZERO(&m_temp_sockets_bits);
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
		m_temp_sockets_bits = m_tcp.source_sockets_bits();
		std::cout << "------entered select-------\n";
		m_activity = select(1024, &m_temp_sockets_bits, 0, 0, 0);
		std::cout << "------exit select-------\n";
		if((m_activity < 0) && (errno != EINTR))
			fatal_error("Select fail!\n");

		if(FD_ISSET(m_listen_socket, &m_temp_sockets_bits))
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
	for(auto& socket : m_deleted_sockets)
	{
		m_players.delete_player(socket);
	}
	m_deleted_sockets.clear(); // need to by threds save - tcp can enter new deleted
}

void Rauter::accept_new_client()
{
	if (m_num_of_clients > 1000)
		m_clients_sockets.erase(m_clients_sockets.begin());

	int client_socket = m_tcp.accept_new_client(m_sin, m_listen_socket);
	if(client_socket == -1)
		fatal_error("Accept fail!\n");

	insert_client(client_socket);
}

void Rauter::insert_client(int a_client_socket)
{
	FD_SET(a_client_socket, &m_source_sockets_bits);	
	m_clients_sockets.emplace_back(a_client_socket);			
	++m_num_of_clients;
}

void Rauter::take_care_exists_clients()
{
    auto it = m_clients_sockets.begin();
    auto end = m_clients_sockets.end();
		
	while((it != end) && (m_activity > 0)) 
	{	
        int client_socket = *it;
		if(FD_ISSET(client_socket, &m_temp_sockets_bits))
		{
			if(!m_tcp.receive_from_client(client_socket, m_buffer))
				m_tcp.delete_client(client_socket, it);

            else
            {
                move_client_to_back(client_socket, it);
                m_action_in.get(m_buffer, client_socket);
            }
			
			m_activity--;
		}
		++it; 
	} 
}

void Rauter::move_client_to_back(int a_client_socket, std::vector<int>::iterator& a_it)
{
	m_clients_sockets.erase(a_it);
	m_clients_sockets.emplace_back(a_client_socket);
	--a_it;
}

void Rauter::stop()
{
    m_stop = true;
}

void Rauter::fatal_error(std::string a_fail)
{
	perror(a_fail.c_str());
	m_tcp.close_all_clients_sockets();
    throw;
}


}//namespace home