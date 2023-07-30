#pragma once

#include <mutex>
#include <vector>
#include <iostream>
#include <condition_variable>

#include "cli_cmd.hpp"

#define BUFFER_SIZE 512

namespace poker
{

using Lock = std::unique_lock<std::mutex>;

class MsgMemory
{
public:
    MsgMemory(size_t a_capacity);
    ~MsgMemory();

    char* enqueue(int a_socket);
    char* dequeue(int* a_socket);
    void unlock();
    void stop();

private:
    bool full()const;
    bool empty()const;
    MsgMemory(MsgMemory const&  source);            // no impl by design
    MsgMemory& operator=(MsgMemory const& source);  // no impl by design

private:
    int m_stop;
    size_t m_in;
    size_t m_out;
    size_t m_size;
    size_t m_capacity;

    char* m_memory; 
    int* m_sockets; 
    mutable std::mutex m_mutex;
    mutable std::condition_variable m_cond;
};


} //poker namespace
