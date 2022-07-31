#include "protocol.hpp"
#include <iostream>

namespace poker
{

namespace impl
{

void pack_strings(char* a_buffer, Args& a_arganmats, int& a_bytes)
{
    int num_of_str = a_arganmats.m_strings.size();
    *(a_buffer + a_bytes) = num_of_str;
    ++a_bytes;

    for(int i = 0; i < num_of_str; ++i)
    {
        int str_size = a_arganmats.m_strings[i].size();
        *(a_buffer + a_bytes) = str_size;
        ++a_bytes;

        strcpy(a_buffer + a_bytes, a_arganmats.m_strings[i].c_str());
        a_bytes += str_size;
    }
}

void pack_ints(char* a_buffer, Args& a_arganmats, int& a_bytes)
{
    int num_of_int = a_arganmats.m_ints.size();
    *(a_buffer + a_bytes) = num_of_int;
    ++a_bytes;

    for(int i = 0; i < num_of_int; ++i)
    {
        *(int*)(a_buffer + a_bytes) = a_arganmats.m_ints[i];
        a_bytes += sizeof(int); 
    }
}

void unpack_strings(char* a_buffer, Args& a_arganmats, int& a_num_of_strings, int& a_bytes)
{
    for(int i = 0; i < a_num_of_strings; ++i)
    {
        int str_size = *(a_buffer + a_bytes);
        ++a_bytes;

        std::string str(a_buffer + a_bytes, str_size);
        a_arganmats.m_strings.emplace_back(str);
        a_bytes += str_size;
    }
}

void unpack_ints(char* a_buffer, Args& a_arganmats, int& a_num_of_ints, int& a_bytes)
{
    for(int i = 0; i < a_num_of_ints; ++i)
    {
        a_arganmats.m_ints.emplace_back(*(int*)(a_buffer + a_bytes));
        a_bytes += sizeof(int);
    }
}


void encrypt(std::string a_key, char* a_buffer, int a_message_size)
{
	// int key_size = a_key.size();
    // int j = 0;

	// for(int i = 0; i < a_message_size; i++)
	// {
	// 	*(a_buffer + i) += a_key[j];
	// 	j = (j + 1)%key_size;
	// }	
}

static void decrypt(std::string a_key, char* a_buffer, int a_message_size)
{
	// int key_size = a_key.size();
	// int j = 0;

	// for(int i = 0; i < a_message_size; i++)
	// {
	// 	*(a_buffer + i) -= a_key[j];
	// 	j = (j + 1)%key_size;
	// }	
}

}//namespace impl

int pack(char* a_buffer, Args& a_arganmats, Message_type a_message)
{
    if(a_buffer == nullptr)
    {return BUFFER_NOT_INITIALIZE;}

    int bytes = sizeof(int);   // "total size" meta data place   
    *(a_buffer + bytes) = a_message;
    ++bytes;

    impl::pack_strings(a_buffer, a_arganmats, bytes);
    impl::pack_ints(a_buffer, a_arganmats, bytes);

    *a_buffer = bytes;
    impl::encrypt("poker", a_buffer, bytes);

    return bytes;
}

Message_type unpack(char* a_buffer, Args& a_arganmats)
{
    if(a_buffer == nullptr)
    {return BUFFER_NOT_INITIALIZE;}

    impl::decrypt("poker", a_buffer, message_size(a_buffer));

    int bytes = sizeof(int);   // "total size" meta data place   
    Message_type message = (Message_type)(*(a_buffer + bytes));
    ++bytes;

    int num_of_strings = *(a_buffer + bytes);
    ++bytes;

    if(num_of_strings != 0)
        impl::unpack_strings(a_buffer, a_arganmats, num_of_strings, bytes);

    int num_of_ints = *(a_buffer + bytes);
    ++bytes;

    if(num_of_ints != 0)
        impl::unpack_ints(a_buffer, a_arganmats, num_of_ints, bytes);

    return message;
}

int message_size(char* a_buffer)
{
    impl::decrypt("poker", a_buffer, sizeof(int));
    int size = *(a_buffer);
    impl::encrypt("poker", a_buffer, sizeof(int));
    return size;
}

Message_type message_type(char* a_buffer)
{
    impl::decrypt("poker", a_buffer, sizeof(int) + 1);
    Message_type type = (Message_type)*(a_buffer + sizeof(int));
    impl::encrypt("poker", a_buffer, sizeof(int) + 1);
    return type;
}

int num_of_ints(char* a_buffer)
{
    int size =  message_size(a_buffer);
    impl::decrypt("poker", a_buffer, size);

    int bytes = sizeof(int) + 1;              // "total size" meta data place  +   Message type
    int num_of_strings = *(a_buffer + bytes);
    ++bytes;

    for(int i = 0; i < num_of_strings; ++i)
    {
        int str_size = *(a_buffer + bytes);
        bytes += (str_size + 1);               // str size + str_size place
    }

    int num_of_ints = *(a_buffer + bytes);

    impl::encrypt("poker", a_buffer, size);

    return num_of_ints;
}

}// poker namespace


