#include "player.hpp"

namespace poker
{

namespace impl
{

void init_vars(std::unordered_map<std::string, int>& a_vars, int a_amount, int a_socket)
{
    a_vars.reserve(NUM_OF_VARS);
    a_vars["m_amount"] = a_amount;
    a_vars["m_socket"] = a_socket;
    a_vars["m_result"] = 0;
    a_vars["m_bet"]    = 0;
}

void init_flags(std::unordered_map<std::string, bool>& a_flags)
{
    a_flags.reserve(NUM_OF_FLAGS);
    a_flags["m_fold"]    = false;
    a_flags["m_viewer"]  = false;
    a_flags["m_my_turn"] = false;    
}


}//impl namespace

Player::Player(std::string& a_name, std::string& a_gender, int a_amount, int a_socket)
: m_name(a_name)
, m_gender(a_gender)
, m_hand()
{
    m_hand.reserve(NUM_OF_CARDS);
    impl::init_flags(m_flags);
    impl::init_vars(m_vars, a_amount, a_socket);
};


}// poker namespace
