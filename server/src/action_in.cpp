#include "action_in.hpp"
#include <iostream>

namespace poker
{

namespace impl
{

std::string get_name(char* a_buffer)
{
    Args arg(1, 0);
    unpack(a_buffer, arg);
    return arg.m_strings[0];
}

}//namespace impl


ActionIn::ActionIn(TcpServer& a_tcp, ActionOut& a_action_out, PlayersContainer& a_players, Subscribs& a_subscribs, BetRound& a_bet_round)
: m_buffer(new char[1024])
, m_tcp(a_tcp)
, m_action_out(a_action_out)
, m_players(a_players)
, m_subscribs(a_subscribs)
, m_bet_round(a_bet_round)
{

}

ActionIn::~ActionIn()
{
    delete[] m_buffer;
}

 void ActionIn::get(char* a_buffer, int a_client_socket)
{
    Message_type message = message_type(a_buffer);

    switch (message)
    {
    case REGISTRATION_REQUES:
        registration_reques(a_buffer, a_client_socket);
        break;

    case LOG_IN_REQUEST:
        log_in_reques(a_buffer, a_client_socket);
        break;

    case BET_ACTION:
        bet(a_buffer);
        break;
    
    case FINISH_BET:
        finish_bet(a_buffer);
        break;

    case CHECK_ACTION:
        check(a_buffer);
        break;

    case FOLD_ACTION:
        fold(a_buffer);
        break;

    case WAKE_UP_CLIENT:
        wake_up_client(a_client_socket);
        break;

    default:
        break;
    }

}

void ActionIn::registration_reques(char* a_buffer, int a_client_socket)
{
    Args arg(3, 0);
    unpack(a_buffer, arg);
    Subscriber subscriber;
    subscriber.m_name = arg.m_strings[0];
    subscriber.m_password = arg.m_strings[1];
    subscriber.m_gender = arg.m_strings[2];
    m_subscribs.new_subscriber(subscriber, a_client_socket);
}

void ActionIn::log_in_reques(char* a_buffer, int a_client_socket)
{
    Args arg(2, 1);
    unpack(a_buffer, arg);
    
    std::string gender = m_subscribs[arg.m_strings[0]].m_gender;
    if(m_subscribs.log_in_chack(arg.m_strings[0], arg.m_strings[1], a_client_socket)
    && m_players.log_in_chack(arg.m_strings[0], a_client_socket))
        m_players.new_player(arg.m_strings[0], gender, arg.m_ints[0], a_client_socket);
}

void ActionIn::bet(char* a_buffer)
{
    Args arg(1,1);
    unpack(a_buffer, arg);

    m_bet_round.bet_in(arg.m_ints[0]);
}

void ActionIn::finish_bet(char* a_buffer)
{
    // // need to cheack if is a legal bet
    // std::string name = impl::get_name(a_buffer);

    // //need to send to "bet round" and he will activate this function
    // m_action_out.turn_off(name, "my_turn");
    // m_action_out.turn_off(name, "bet");
    m_bet_round.finish_bet();
}

void ActionIn::check(char* a_buffer)
{
    std::string name = impl::get_name(a_buffer);

    m_action_out.turn_off(name, "my_turn");
    m_action_out.check(name);
}

void ActionIn::fold(char* a_buffer)
{
    std::string name = impl::get_name(a_buffer);
    m_action_out.turn_off(name, "my_turn");
    m_action_out.fold(name);
}

void ActionIn::wake_up_client(int a_client_socket)
{
    m_action_out.wake_up_client(a_client_socket);
}


}// poker namespace
