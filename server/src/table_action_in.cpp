#include "table_action_in.hpp"

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

TableActionIn::TableActionIn(ActionOut& a_action_out, PlayersContainer& a_players, BetRound& a_bet_round)
: m_bet_round(a_bet_round)
, m_action_out(a_action_out)
, m_players(a_players)
{

}

void TableActionIn::get(char* a_buffer, int a_client_socket)
{
    Message_type message = message_type(a_buffer);

    switch (message)
    {
    case START_BET_ACTION:
        start_bet();
        break;

    case BET_ACTION:
        bet(a_buffer);
        break;
    
    case FINISH_BET:
        finish_bet();
        break;

    case CHECK_ACTION:
        check();
        break;

    case FOLD_ACTION:
        fold();
        break;

    default:
        break;
    }
}

void TableActionIn::start_bet()
{
    m_bet_round.start_bet();
}

void TableActionIn::bet(char* a_buffer)
{
    Args arg(1,1);
    unpack(a_buffer, arg);
    m_bet_round.bet_in(arg.m_ints[0]);
}

void TableActionIn::finish_bet()
{
    m_bet_round.finish_bet();
}

void TableActionIn::check()
{
    m_bet_round.chack_in();
}

void TableActionIn::fold()
{
    m_bet_round.fold_in();
}

void TableActionIn::player_deleted(int a_client_socket)
{
    m_bet_round.player_deleted(a_client_socket);
}


}// poker namespace
