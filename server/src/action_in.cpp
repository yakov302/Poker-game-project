#include "action_in.hpp"

namespace poker
{

extern bool dbg[NUM_OF_DBG_TYPES];

namespace impl
{

std::string get_name(char* a_buffer)
{
    Args arg(1, 0);
    unpack(a_buffer, arg);
    return arg.m_strings[0];
}


}//namespace impl

ActionIn::ActionIn(TcpServer& a_tcp, Subscribs& a_subscribs, TablesContainer& a_tables_container)
: m_action_out(a_tcp)
, m_subscribs(a_subscribs)
, m_tables_container(a_tables_container)
{

}

void ActionIn::get(char* a_buffer, int a_client_socket)
{
    Message_type message = message_type(a_buffer);

    if(dbg[ACTION_IN])[[unlikely]]
        std::cout << __func__ << "(): " << "get  message: " << message << " frome socket: " << a_client_socket << " to table id: " <<  m_tables_container.m_socket_to_table_id[a_client_socket] << std::endl;

    switch (message)
    {
    case REGISTRATION_REQUES:
        registration_reques(a_buffer, a_client_socket);
        break;

    case LOG_IN_REQUEST:
        log_in_reques(a_buffer, a_client_socket);
        break;

    case START_BET_ACTION:
        start_bet(a_client_socket);
        break;

    case BET_ACTION:
        bet(a_buffer, a_client_socket);
        break;
    
    case FINISH_BET:
        finish_bet(a_client_socket);
        break;

    case CHECK_ACTION:
        check(a_client_socket);
        break;

    case FOLD_ACTION:
        fold(a_client_socket);
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

    if(dbg[ACTION_IN])[[unlikely]]
        std::cout << __func__ << "(): " << "call m_subscribs.log_in_chack(" << arg.m_strings[0] << ", " << arg.m_strings[1]<< ", " << a_client_socket<< ")" << std::endl;
    
    if(!m_subscribs.log_in_chack(arg.m_strings[0], arg.m_strings[1], a_client_socket))
        return;

    if(dbg[ACTION_IN])[[unlikely]]
        std::cout << __func__ << "(): " << "call m_tables_container.get_player(" << arg.m_strings[0] << ", " << gender << ", " << arg.m_ints[0]<< ", " <<  a_client_socket << ")" << std::endl;
    
    m_tables_container.get_player(arg.m_strings[0], gender, arg.m_ints[0], a_client_socket);
}

void ActionIn::start_bet(int a_client_socket)
{   
    int table_id = m_tables_container.m_socket_to_table_id[a_client_socket];
    m_tables_container.m_tables[table_id].get()->m_bet_round.start_bet();
}

void ActionIn::bet(char* a_buffer, int a_client_socket)
{
    Args arg(1,1);
    unpack(a_buffer, arg);
    int table_id = m_tables_container.m_socket_to_table_id[a_client_socket];
    m_tables_container.m_tables[table_id].get()->m_bet_round.bet_in(arg.m_ints[0]);
}

void ActionIn::finish_bet(int a_client_socket)
{
    int table_id = m_tables_container.m_socket_to_table_id[a_client_socket];
    m_tables_container.m_tables[table_id].get()->m_bet_round.finish_bet();
}

void ActionIn::check(int a_client_socket)
{
    int table_id = m_tables_container.m_socket_to_table_id[a_client_socket];
    m_tables_container.m_tables[table_id].get()->m_bet_round.chack_in();
}

void ActionIn::fold(int a_client_socket)
{
    int table_id = m_tables_container.m_socket_to_table_id[a_client_socket];
    m_tables_container.m_tables[table_id].get()->m_bet_round.fold_in();
}

void ActionIn::wake_up_client(int a_client_socket)
{
    m_action_out.wake_up_client(a_client_socket);
}

void ActionIn::player_deleted(int a_client_socket)
{
    int table_id = m_tables_container.m_socket_to_table_id[a_client_socket];
    if(m_tables_container.is_table_is_alive(table_id))
        m_tables_container.m_tables[table_id].get()->m_bet_round.player_deleted(a_client_socket);
}


}// poker namespace
