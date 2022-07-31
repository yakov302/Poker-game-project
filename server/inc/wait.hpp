#pragma once

#include <condition_variable>
#include <iostream>


namespace poker
{

using Lock = std::unique_lock<std::mutex>;

class Wait
{
public: 
    Wait();

    void enter_wait();
    void exit_wait();

private:
    bool m_flag;
    std::mutex m_mutex; 
    std::condition_variable m_cond_var;
};


}//namespace poker
