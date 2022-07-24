#include "rauter.hpp"

namespace poker
{

namespace impl
{

void sin_setting(std::string& a_server_ip, int& a_servr_port, struct sockaddr_in& a_sin)
{
	memset (&a_sin, 0, sizeof(a_sin));	
	a_sin.sin_family = AF_INET;
	a_sin.sin_addr.s_addr =  inet_addr(a_server_ip.c_str());
	a_sin.sin_port = htons(a_servr_port);
}

void open_listen_socket(int& a_listen_socket, fd_set& a_set, TcpServer& a_tcp)
{
	a_listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(a_listen_socket < 0)
		a_tcp.fatal_error("Open listen socket fail!\n");

	FD_SET(a_listen_socket, &a_set);
}

void set_listen_socket(int& a_listen_socket, TcpServer& a_tcp)
{
	if(listen(a_listen_socket, 1000) < 0)
		a_tcp.fatal_error("Set listen socket fail!\n");
}

void set_setsockopt(int& a_listen_socket,TcpServer& a_tcp)
{
    int optval = 1;		
	if(setsockopt(a_listen_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
		a_tcp.fatal_error("Set setsockopt fail!\n");
}

void set_bind(int& a_listen_socket, struct sockaddr_in& a_sin, TcpServer& a_tcp)
{
	if(bind(a_listen_socket, (struct sockaddr*) &a_sin, sizeof(a_sin)) < 0)
		a_tcp.fatal_error("Bind fail!\n");
}


} //namespace impl

TcpServer::TcpServer(std::string a_server_ip, int a_servr_port)
: m_listen_socket(0)
, m_num_of_clients(0)
, m_sin()
, m_source_sockets_bits()
, m_clients_sockets()
{
	FD_ZERO(&m_source_sockets_bits);
	m_clients_sockets.reserve(1001);
    impl::sin_setting(a_server_ip, a_servr_port, m_sin);
	impl::open_listen_socket(m_listen_socket, m_source_sockets_bits, *this);
	impl::set_setsockopt(m_listen_socket, *this);
	impl::set_bind(m_listen_socket, m_sin, *this);
	impl::set_listen_socket(m_listen_socket, *this);
}

int TcpServer::accept_new_client(struct sockaddr_in& a_sin, int a_listen_socket)
{
	socklen_t sin_len = sizeof(&a_sin);
	int client_socket = accept(a_listen_socket, (struct sockaddr*)&a_sin, &sin_len);
	if(client_socket < 0)
		return -1;

	return client_socket;
}

bool TcpServer::receive_from_client(int a_client_socket, char* a_buffer)
{
	std::cout <<  "\n\n" << "*************" << "\n\n";
	std::cout << "server recive: " << "\n";
	std::cout << "from socet: " << a_client_socket << "\n";

	int receive_bytes = recv(a_client_socket, a_buffer, sizeof(int), 0);
	if(receive_bytes == 0)
	{perror("Receive fail!\n"); return false;}

	int message_len = message_size(a_buffer);
	std::cout << "size: " << message_len << "\n";

	while(receive_bytes < message_len)
	{
		int current_receive = recv(a_client_socket, (a_buffer + receive_bytes), message_len - receive_bytes, 0);
		if(current_receive == 0)
		{perror("Receive fail!\n"); return false;}
		 
		receive_bytes += current_receive;
	}	

	std::cout << "message type: " << message_type(a_buffer) << "\n";
	std::cout << "receive bytes : " << receive_bytes << "\n";
	std::cout <<  "\n\n" << "*************" << "\n\n";

	return true;
}

bool TcpServer::send_to_client(int a_client_socket, char* a_buffer, int a_message_size)
{
	std::cout <<  "\n\n" << "*************" << "\n\n";
	std::cout << "server sent: " << "\n";
	std::cout << "size: " << message_size(a_buffer) << "\n";
	std::cout << "message_type: " << message_type(a_buffer) << "\n";
	std::cout << "to socet: " << a_client_socket << "\n";

	int sent_byte = 0;
	while((sent_byte < a_message_size) && (errno != EPIPE))
	{
    	int current_byte = send(a_client_socket, (a_buffer + sent_byte), (a_message_size - sent_byte), 0);
		if(current_byte < 0)
			perror("Send fail!\n");

		sent_byte += current_byte;
	}

	if(errno == EPIPE)
		return false;

	std::cout << "sent byte: " << sent_byte << "\n";
	std::cout <<  "\n\n" << "*************" << "\n\n";

	return true;
}

void TcpServer::send_all_clients(char* a_buffer, int a_message_size)
{
	auto it = m_clients_sockets.begin();
    auto end = m_clients_sockets.end();
		
	while(it != end) 
	{	
        int client_socket = *it;
		if(!send_to_client(client_socket, a_buffer, a_message_size))
			delete_client(client_socket, it);
		++it; 
	} 
}

void TcpServer::delete_client(int a_client_socket, std::vector<int>::iterator& a_it)
{
	std::cout << "delete_client\n";
	FD_CLR(a_client_socket, &m_source_sockets_bits);
	close(a_client_socket);
	m_clients_sockets.erase(a_it);
	m_deleted_sockets.emplace_back(a_client_socket);
	m_num_of_clients--;
	--a_it;
}

void TcpServer::fatal_error(std::string a_fail)
{
	perror(a_fail.c_str());
	close_all_clients_sockets();
    throw;
}

void TcpServer::close_all_clients_sockets()
{
	for(auto socket : m_clients_sockets)		
		close(socket);
}

int& TcpServer::listen_socket()
{
	return m_listen_socket;
}

int& TcpServer::num_of_clients()
{
	return m_num_of_clients;
}

fd_set& TcpServer::source_sockets_bits()
{
	return m_source_sockets_bits;
}

std::vector<int>& TcpServer::clients_sockets()
{
	return m_clients_sockets;
}

std::vector<int>& TcpServer::deleted_sockets()
{
	return m_deleted_sockets;
}


}//namespace home