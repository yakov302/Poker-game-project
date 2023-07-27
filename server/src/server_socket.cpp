#include "server_socket.hpp"

namespace poker
{

namespace impl
{

void load_frome_file(std::string& a_server_ip, int& a_server_port)
{
	std::ifstream file("./resources/server_ip_and_port.txt");
	file >> a_server_ip;
	file >> a_server_port;
}

void sin_setting(struct sockaddr_in& a_sin)
{
    int server_port;
	std::string server_ip;
	load_frome_file(server_ip, server_port);

	memset (&a_sin, 0, sizeof(a_sin));	
	a_sin.sin_family = AF_INET;
	a_sin.sin_addr.s_addr =  inet_addr(server_ip.c_str());
	a_sin.sin_port = htons(server_port);
}

void open_listen_socket(int& a_listen_socket, fd_set& a_set, ServerSocket& a_server_socket)
{
	a_listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(a_listen_socket < 0)[[unlikely]]
		a_server_socket.fatal_error("Open listen socket fail!\n");

	FD_ZERO(&a_set);
	FD_SET(a_listen_socket, &a_set);
}

void set_listen_socket(int& a_listen_socket, ServerSocket& a_server_socket)
{
	if(listen(a_listen_socket, 1000) < 0)[[unlikely]]
		a_server_socket.fatal_error("Set listen socket fail!\n");
}

void set_setsockopt(int& a_listen_socket,ServerSocket& a_server_socket)
{
    int optval = 1;		
	if(setsockopt(a_listen_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)[[unlikely]]
		a_server_socket.fatal_error("Set setsockopt fail!\n");
}

void set_bind(int& a_listen_socket, struct sockaddr_in& a_sin, ServerSocket& a_server_socket)
{
	if(bind(a_listen_socket, (struct sockaddr*) &a_sin, sizeof(a_sin)) < 0)[[unlikely]]
		a_server_socket.fatal_error("Bind fail!\n");
}


} //namespace impl

ServerSocket::ServerSocket()
: m_listen_socket(0)
, m_num_of_clients(0)
, m_source_fd()
, m_active_fd()
, m_server_sin()
, m_client_sin()
, m_connected_sockets()
, m_deleted_sockets()
{
    impl::sin_setting(m_server_sin);
	impl::open_listen_socket(m_listen_socket, m_source_fd, *this);
	impl::set_setsockopt(m_listen_socket, *this);
	impl::set_bind(m_listen_socket, m_server_sin, *this);
	impl::set_listen_socket(m_listen_socket, *this);
}

ServerSocket::~ServerSocket()
{
    close(m_listen_socket);
    close_all_clients_sockets();
}

void ServerSocket::delete_client(std::list<int>::iterator& a_it)
{
    int socket = *a_it;
	close(socket);
    FD_CLR(socket, &m_source_fd);
    m_connected_sockets.remove(socket);
    m_deleted_sockets.emplace_front(socket);
    --m_num_of_clients;
    --a_it;
}

void ServerSocket::insert_client(int a_client_socket)
{
    FD_SET(a_client_socket, &m_source_fd);
    m_connected_sockets.emplace_front(a_client_socket);
	++m_num_of_clients;
}

int& ServerSocket::listen_socket()
{
    return m_listen_socket;
}

struct sockaddr_in& ServerSocket::client_sin()
{
    return m_client_sin;
}

int& ServerSocket::num_of_clients()
{
    return m_num_of_clients;
}

bool ServerSocket::is_in_active_fd(int a_socket)
{
    return FD_ISSET(a_socket, &m_active_fd);
}

fd_set& ServerSocket::copy_of_source_fd()
{
    FD_ZERO(&m_active_fd);
    m_active_fd = m_source_fd;
    return m_active_fd;
}

void ServerSocket::move_client_to_front(std::list<int>::iterator& a_it)
{
    int socket = *a_it;
    m_connected_sockets.erase(a_it);
    m_connected_sockets.emplace_front(socket);
    --a_it;
}

void ServerSocket::delete_less_active_client()
{
    m_connected_sockets.pop_back();
    --m_num_of_clients;
}

std::list<int>& ServerSocket::connected_sockets()
{
    return m_connected_sockets;
}

void ServerSocket::delete_from_deleted_sockets(std::list<int>::iterator& a_it)
{
    m_deleted_sockets.erase(a_it);
    --a_it;
}

std::list<int>& ServerSocket::deleted_sockets()
{
    return m_deleted_sockets;
}

void ServerSocket::fatal_error(std::string a_fail)
{
	perror(a_fail.c_str());
	close_all_clients_sockets();
    throw;
}

void ServerSocket::close_all_clients_sockets()
{
	for(auto socket : m_connected_sockets)		
		close(socket);
}


}// poker namespace
