#include "client_socket.hpp"

namespace poker
{

namespace impl
{

void fatal_error(std::string a_fail)
{
	perror(a_fail.c_str());
    throw;
}

void sin_setting(struct sockaddr_in& a_sin, std::string& a_server_ip, int& a_servr_port)
{
	memset(&a_sin, 0, sizeof(a_sin)); 
	a_sin.sin_family = AF_INET;
	a_sin.sin_addr.s_addr =  inet_addr(a_server_ip.c_str());
	a_sin.sin_port = htons(a_servr_port);
}

void set_socket(int& a_socket, ClientSocket& a_this)
{
	a_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(a_socket < 0)
		fatal_error("Set socket fail!\n");
}

void set_sockopt(int& a_socket, ClientSocket& a_this)
{
	int optval = 1;	
	if(setsockopt(a_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
		fatal_error("Setsockopt fail!\n");
}

void connect_to_server(struct sockaddr_in& a_sin, int& a_socket, ClientSocket& a_this)
{
	if(connect(a_socket, (struct sockaddr*)&a_sin, sizeof(*&a_sin)) < 0)
		fatal_error("Connect to server fail!\n");
}


}//namespace impl

ClientSocket::ClientSocket(std::string a_server_ip, int a_server_port)
: m_fd()
, m_socket(0)
, m_sin()
{
	impl::sin_setting(m_sin, a_server_ip, a_server_port);
	impl::set_socket(m_socket, *this);
	impl::set_sockopt(m_socket, *this);
	impl::connect_to_server(m_sin, m_socket, *this);
    FD_ZERO(&m_fd);
    FD_SET(m_socket, &m_fd);
}

ClientSocket::~ClientSocket()
{
    close(m_socket);
}

int& ClientSocket::socket()
{
    return m_socket;
}

fd_set& ClientSocket::fd()
{
    return m_fd;
}


}// poker namespace
