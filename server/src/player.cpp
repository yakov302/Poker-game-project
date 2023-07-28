#include "player.hpp"

namespace poker
{
    
extern bool dbg[NUM_OF_DBG_TYPES];

namespace impl
{

void init_vars(std::unordered_map<std::string, int>& a_vars, int a_amount, int a_socket)
{
    a_vars.reserve(NUM_OF_VARS);
    a_vars["amount"] = a_amount;
    a_vars["socket"] = a_socket;
    a_vars["result"] = 0;
    a_vars["bet"]    = 0;
}

void init_flags(std::unordered_map<std::string, bool>& a_flags)
{
    a_flags.reserve(NUM_OF_FLAGS);
    a_flags["fold"]    = false;
    a_flags["viewer"]  = false;
    a_flags["my_turn"] = false;    
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
