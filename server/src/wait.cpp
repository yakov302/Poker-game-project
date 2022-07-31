#include "wait.hpp"

namespace poker
{

Wait::Wait()
{

}

void Wait::enter_wait()
{
    Lock lock(m_mutex);
    m_flag = true;
    std::cout << "----(enter wait)----\n";
    m_cond_var.wait(lock, [this]() {return !m_flag;});
    std::cout << "----(exit wait)----\n";
}

void Wait::exit_wait()
{
    m_flag = false;
    m_cond_var.notify_all();
}


}//namespace poker
