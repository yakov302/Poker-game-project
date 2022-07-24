#include"tcp_client.hpp"
#include <iostream>

namespace poker
{

TcpClient::TcpClient(ClientSocket& a_socket)
: m_socket(a_socket)
{

}

bool TcpClient::send_to_server(char* a_buffer, int a_length)
{
	std::cout <<  "\n\n" << "*************" << "\n\n";
	std::cout << "client sent: " << "\n";
	std::cout << "size: " << message_size(a_buffer) << "\n";
	std::cout << "message_type: " << message_type(a_buffer) << "\n";
	std::cout << "from socet: " << m_socket.socket() << "\n";

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


	std::cout << "sent byte: " << sent_byte << "\n";
	std::cout <<  "\n\n" << "*************" << "\n\n";

	return true;
}

bool TcpClient::receive_from_server(char* a_buffer)
{

	std::cout <<  "\n\n" << "*************" << "\n\n";
	std::cout << "client recive: " << "\n";
	std::cout << "to socet: " << m_socket.socket() << "\n";

	int receive_bytes = recv(m_socket.socket(), a_buffer, sizeof(int), 0);
	if(receive_bytes  == 0)
	{perror("Receive fail!\n"); return false;}

	int message_len = message_size(a_buffer);
	std::cout << "size: " << message_len << "\n";

	while(receive_bytes < message_len)
	{
		int current_receive = recv(m_socket.socket(), (a_buffer + receive_bytes), message_len - receive_bytes, 0);
		if(current_receive == 0)
		{perror("Receive fail!\n"); return false;}
		 
		receive_bytes += current_receive;
	}	

	std::cout << "message type: " << message_type(a_buffer) << "\n";
	std::cout << "receive bytes : " << receive_bytes << "\n";
	std::cout <<  "\n\n" << "*************" << "\n\n";

	return true;
}


}//namespace poker