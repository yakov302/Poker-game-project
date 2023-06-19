#include"tcp_client.hpp"

namespace poker
{

TcpClient::TcpClient(ClientSocket& a_socket)
: m_mutex()
, m_socket(a_socket)
{

}

bool TcpClient::send_to_server(char* a_buffer, int a_length)
{
	Lock lock(m_mutex);

	int total_sent_bytes = 0;
	while((total_sent_bytes < a_length) && (errno != EPIPE))
	{
    	int current_sent_byte = send(m_socket.socket(), (a_buffer + total_sent_bytes), (a_length - total_sent_bytes), 0);
		if(current_sent_byte < 0)[[unlikely]]
			perror("Send fail!\n");

		total_sent_bytes += current_sent_byte;
	}
	
	if(errno == EPIPE)[[unlikely]]
		return false;

	return true;
}

bool TcpClient::receive_from_server(char* a_buffer)
{
	Lock lock(m_mutex);

	int total_receive_bytes = recv(m_socket.socket(), a_buffer, sizeof(int), 0);
	if(total_receive_bytes  == 0)[[unlikely]]
	{perror("Receive fail!\n"); return false;}

	int message_len = message_size(a_buffer);
	while(total_receive_bytes < message_len)
	{
		int current_receive_bytes = recv(m_socket.socket(), (a_buffer + total_receive_bytes), message_len - total_receive_bytes, 0);
		if(current_receive_bytes == 0)[[unlikely]]
		{perror("Receive fail!\n"); return false;}	 
		total_receive_bytes += current_receive_bytes;
	}	

	return true;
}


}//namespace poker
