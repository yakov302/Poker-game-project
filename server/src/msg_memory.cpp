#include "msg_memory.hpp"

namespace poker
{

extern bool dbg[NUM_OF_DBG_TYPES];

MsgMemory::MsgMemory(size_t a_capacity)
: m_stop(false)
, m_in(0)
, m_out(0)
, m_size(0)
, m_capacity(a_capacity)
, m_memory(new char[m_capacity*BUFFER_SIZE])
, m_sockets(new int[m_capacity])
, m_mutex()
, m_cond()
{
    if(dbg[MSG_MEMORY])[[unlikely]]
        std::cout << __func__ << "(): constructor is built" << std::endl;
}

MsgMemory::~MsgMemory()
{
    delete[] m_memory;
    delete[] m_sockets;
}

char* MsgMemory::enqueue(int a_socket)
{
    if(dbg[MSG_MEMORY])[[unlikely]]
        std::cout << __func__ << "(): enter wait" << std::endl;

    Lock lock(m_mutex);
    m_cond.wait(lock, [this]() {return !full();}); 

    if(dbg[MSG_MEMORY])[[unlikely]]
        std::cout << __func__ << "(): index: " << m_in << " pos(in bytes): " << m_in*BUFFER_SIZE <<  " socket: " << a_socket << " size: " << m_size+1 <<  std::endl;

    char* buf = m_memory + (m_in*BUFFER_SIZE);
    m_sockets[m_in] = a_socket;
    m_in = (m_in + 1) % m_capacity;
    m_size++;

    return buf;
}

void MsgMemory::unlock()
{
    if(dbg[MSG_MEMORY])[[unlikely]]
        std::cout << __func__ << "(): unlock mutex" << std::endl;

    m_cond.notify_all();
}


char* MsgMemory::dequeue(int* a_socket)
{
    if(dbg[MSG_MEMORY])[[unlikely]]
        std::cout << __func__ << "(): enter wait" << std::endl;

    Lock lock(m_mutex);
    m_cond.wait(lock, [this]() {return !empty();});


    if(dbg[MSG_MEMORY])[[unlikely]]
        std::cout << __func__ << "(): index: " << m_out << " pos(in bytes): " << m_out*BUFFER_SIZE <<  " socket: " <<  m_sockets[m_out] << " size: " << m_size-1 <<  std::endl;

    char* buf = m_memory + (m_out*BUFFER_SIZE);
    *a_socket = m_sockets[m_out];
    m_out = (m_out + 1) % m_capacity;
    m_size--;

    m_cond.notify_all();
    return buf;
}

bool MsgMemory::full() const
{
    if(dbg[MSG_MEMORY])[[unlikely]]
    {
        if(m_stop || !(m_size >= m_capacity))
            std::cout << __func__ << "(): out from block" << std::endl;
        else
            std::cout << __func__ << "(): in to block" << std::endl;
    }

    if(m_stop)
        return false;
    return (m_size >= m_capacity);
}

bool MsgMemory::empty() const
{

    if(dbg[MSG_MEMORY])[[unlikely]]
    {
        if(m_stop || !(m_size == 0))
            std::cout << __func__ << "(): out from block" << std::endl;
        else
            std::cout << __func__ << "(): in to block" << std::endl;
    }

    if(m_stop)
        return false;
    return (m_size == 0);
}

void MsgMemory::stop()
{
    m_stop = true;
    unlock();
}


} //poker namespace
