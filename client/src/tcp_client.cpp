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

	int sent_byte = 0;
	while((sent_byte < a_length) && (errno != EPIPE))
	{
    	int current_byte = send(m_socket.socket(), (a_buffer + sent_byte), (a_length - sent_byte), 0);
		if(current_byte < 0)
			perror("Send fail!\n");

		sent_byte += current_byte;
	}
	
	if(errno == EPIPE)
		return false;

	return true;
}

bool TcpClient::receive_from_server(char* a_buffer)
{
	Lock lock(m_mutex);

	int receive_bytes = recv(m_socket.socket(), a_buffer, sizeof(int), 0);
	if(receive_bytes  == 0)
	{perror("Receive fail!\n"); return false;}

	int message_len = message_size(a_buffer);
	while(receive_bytes < message_len)
	{
		int current_receive = recv(m_socket.socket(), (a_buffer + receive_bytes), message_len - receive_bytes, 0);
		if(current_receive == 0)
		{perror("Receive fail!\n"); return false;}	 
		receive_bytes += current_receive;
	}	

	return true;
}


}//namespace poker
