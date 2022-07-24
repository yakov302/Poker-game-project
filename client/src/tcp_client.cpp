#include"tcp_client.hpp"
#include <iostream>

namespace poker
{

namespace impl
{

void sin_setting(struct sockaddr_in& a_sin, std::string& a_server_ip, int& a_servr_port)
{
	memset (&a_sin, 0, sizeof(*&a_sin)); //cheak way *&  in sizeof(*&a_sin)
	a_sin.sin_family = AF_INET;
	a_sin.sin_addr.s_addr =  inet_addr(a_server_ip.c_str());
	a_sin.sin_port = htons(a_servr_port);
}

void set_socket(int& a_socket, TcpClient& a_this)
{
	a_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(a_socket < 0)
		a_this.fatal_error("Set socket fail!\n");
}

void set_sockopt(int& a_socket, TcpClient& a_this)
{
	int optval = 1;	
	if(setsockopt(a_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
		a_this.fatal_error("Setsockopt fail!\n");
}

void connect_to_server(struct sockaddr_in& a_sin, int& a_socket, TcpClient& a_this)
{
	if(connect(a_socket, (struct sockaddr*)&a_sin, sizeof(*&a_sin)) < 0)
		a_this.fatal_error("Connect to server fail!\n");
}


}//namespace impl

TcpClient::TcpClient(std::string a_server_ip, int a_servr_port)
: m_socket(0)
, m_sin()
{
	impl::sin_setting(m_sin, a_server_ip, a_servr_port);
	impl::set_socket(m_socket, *this);
	impl::set_sockopt(m_socket, *this);
	impl::connect_to_server(m_sin, m_socket, *this);
}

TcpClient::~TcpClient()
{
    close(m_socket);
}


bool TcpClient::send_to_server(char* a_buffer, int a_length)
{
	std::cout <<  "\n\n" << "*************" << "\n\n";
	std::cout << "client sent: " << "\n";
	std::cout << "size: " << message_size(a_buffer) << "\n";
	std::cout << "message_type: " << message_type(a_buffer) << "\n";
	std::cout << "from socet: " << m_socket << "\n";

	int sent_byte = 0;
	while((sent_byte < a_length) && (errno != EPIPE))
	{
    	int current_byte = send(m_socket, (a_buffer + sent_byte), (a_length - sent_byte), 0);
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

bool TcpClient::receive_from_server(char* a_buffer)
{

	std::cout <<  "\n\n" << "*************" << "\n\n";
	std::cout << "clyent recive: " << "\n";
	std::cout << "to socet: " << m_socket << "\n";

	int receive_bytes = recv(m_socket, a_buffer, sizeof(int), 0);
	if(receive_bytes  == 0)
	{perror("Receive fail!\n"); return false;}

	int message_len = message_size(a_buffer);
	std::cout << "size: " << message_len << "\n";

	while(receive_bytes < message_len)
	{
		int current_receive = recv(m_socket, (a_buffer + receive_bytes), message_len - receive_bytes, 0);
		if(current_receive == 0)
		{perror("Receive fail!\n"); return false;}
		 
		receive_bytes += current_receive;
	}	

	std::cout << "message type: " << message_type(a_buffer) << "\n";
	std::cout << "receive bytes : " << receive_bytes << "\n";
	std::cout <<  "\n\n" << "*************" << "\n\n";

	return true;
}

bool TcpClient::error(std::string a_fail)
{
	perror(a_fail.c_str());
	return false;
}

void TcpClient::fatal_error(std::string a_fail)
{
	perror(a_fail.c_str());
    throw;
}

int TcpClient::socket()
{
	return m_socket;
}


}//namespace poker