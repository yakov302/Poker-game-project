#include "rauter.hpp"

namespace poker
{

TcpServer::TcpServer(ServerSocket& a_socket)
: m_socket(a_socket)
{

}

int TcpServer::accept_new_client()
{
	Lock loack(m_mutex);

	socklen_t sin_len = sizeof(&m_socket.client_sin());
	int client_socket = accept(m_socket.listen_socket(), (struct sockaddr*)&m_socket.client_sin(), &sin_len);
	if(client_socket < 0)
		return -1;

	return client_socket;
}

bool TcpServer::receive_from_client(int a_client_socket, char* a_buffer)
{
	Lock loack(m_mutex);
	
	int receive_bytes = recv(a_client_socket, a_buffer, sizeof(int), 0);
	if(receive_bytes == 0)
	{perror("Receive fail!\n"); return false;}

	int message_len = message_size(a_buffer);
	while(receive_bytes < message_len)
	{
		int current_receive = recv(a_client_socket, (a_buffer + receive_bytes), message_len - receive_bytes, 0);
		if(current_receive == 0)
		{perror("Receive fail!\n"); return false;}
		 
		receive_bytes += current_receive;
	}	

	return true;
}

bool TcpServer::send_to_client(int a_client_socket, char* a_buffer, int a_message_size)
{
	Lock loack(m_mutex);

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

	return true;
}

void TcpServer::send_all_clients(char* a_buffer, int a_message_size)
{
	auto it = m_socket.connected_sockets().begin();
    auto end = m_socket.connected_sockets().end();
		
	while(it != end) 
	{	
        int client_socket = *it;
		if(!send_to_client(client_socket, a_buffer, a_message_size))
			m_socket.delete_client(it);
		++it; 
	} 
}


}//namespace home
