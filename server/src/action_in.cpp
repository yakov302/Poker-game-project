#include "action_in.hpp"

namespace poker
{

struct Arg arg;
extern bool dbg[NUM_OF_DBG_TYPES];

namespace impl
{

std::string get_name(char* a_buffer)
{
    Args arg(1, 0);
    unpack(a_buffer, arg);
    return arg.m_strings[0];
}

static void* thread_function(void* arg)
{
	Arg* arguments = static_cast<Arg*> (arg);
	arguments->m_action_in->run(arguments->m_thread_id);
	return 0;
}


}//namespace impl

ActionIn::ActionIn(TcpServer& a_tcp, Subscribs& a_subscribs, TablesContainer& a_tables_container, MsgMemory& a_msg_memory)
: m_stop()
, m_action_out(a_tcp)
, m_subscribs(a_subscribs)
, m_msg_memory(a_msg_memory)
, m_tables_container(a_tables_container)
{
    m_stop.reserve(NUM_OF_PLAYERS_PER_THREADS);
    open_thread();
}

ActionIn::~ActionIn()
{
    for(auto thread : m_threads)
    {
	    thread->join();
	    delete thread;
    }
}

void ActionIn::open_thread()
{
    if(dbg[ACTION_IN])[[unlikely]]
        std::cout << __func__ << "(): m_threads[" << m_threads.size() << "] = new std::thread(impl::thread_function, this)" << std::endl;

    size_t thread_id = m_threads.size();
    arg = {this, thread_id};
    m_stop[thread_id] = false;
    m_threads.emplace_back(new std::thread(impl::thread_function, &arg));
}

void ActionIn::delete_thread()
{
    if(dbg[ACTION_IN])[[unlikely]]
        std::cout << __func__ << "(): m_threads[" << m_threads.size()-1 << "]->erase()" << std::endl;
    
    size_t thread_id = m_threads.size() -1;
    m_stop[thread_id] = true;
    m_threads.pop_back();
}

void ActionIn::check_thread_allocation()
{
    if(dbg[ACTION_IN])[[unlikely]]
        std::cout << __func__ << "(): num of payers per threads  = players: " << m_tables_container.num_of_players() << " / threads: " <<  m_threads.size() << std::endl;

    int num_of_threads = m_threads.size();
    if(num_of_threads == 0){return;}
    int num_of_payers_per_threads = m_tables_container.num_of_players()/num_of_threads;

    if(num_of_payers_per_threads > NUM_OF_PLAYERS_PER_THREADS)
        open_thread();
    else if((num_of_payers_per_threads < NUM_OF_PLAYERS_PER_THREADS/2) && num_of_threads > 1)
        delete_thread();
}

void ActionIn::run(int a_thread_id)
{
    char* buf;
    int client_socket;
    
	while(!m_stop[a_thread_id])
    {
        if(dbg[ACTION_IN])[[unlikely]]
            std::cout << __func__ << "(): thread id: " << a_thread_id << " call m_msg_memory.dequeue()" << std::endl;
        
        buf = m_msg_memory.dequeue(&client_socket);

        if(dbg[ACTION_IN])[[unlikely]]
            std::cout << __func__ << "(): thread id: " << a_thread_id << " call get(buf, client_socket)" << std::endl;
        
        get(buf, client_socket);
    }

    if(dbg[ACTION_IN])[[unlikely]]
        std::cout << __func__ << "(): thread id: " << a_thread_id << " finish work" << std::endl;
}

void ActionIn::stop()
{
    int size = m_stop.size();
    for(int i = 0; i < size; ++i)
        m_stop[i] = true;
}

void ActionIn::get(char* a_buffer, int a_client_socket)
{
    Message_type message = message_type(a_buffer);

    if(dbg[ACTION_IN])[[unlikely]]
        std::cout << __func__ << "(): " << "get message: " << message << " frome socket: " << a_client_socket << std::endl;

    switch (message)
    {
    case REGISTRATION_REQUES:
        registration_reques(a_buffer, a_client_socket);
        break;

    case LOG_IN_REQUEST:
        log_in_reques(a_buffer, a_client_socket);
        break;
    
    case PLAY_REQUEST:
        play_reques(a_buffer, a_client_socket);
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
    Args arg(2, 0);
    unpack(a_buffer, arg);
    std::string gender = m_subscribs[arg.m_strings[0]].m_gender;

    if(dbg[ACTION_IN])[[unlikely]]
        std::cout << __func__ << "(): " << "call m_subscribs.log_in_chack(" << arg.m_strings[0] << ", " << arg.m_strings[1]<< ", " << a_client_socket<< ")" << std::endl;
    
    if(m_subscribs.log_in_chack(arg.m_strings[0], arg.m_strings[1], a_client_socket) &&
       m_tables_container.log_in_chack(arg.m_strings[0], a_client_socket))
       {
            m_tables_container.get_viewer(a_client_socket);
            m_action_out.log_in_success(arg.m_strings[0], gender, a_client_socket);
       }

}

void ActionIn::play_reques(char* a_buffer, int a_client_socket)
{
    Args arg(2, 1);
    unpack(a_buffer, arg);
    std::string gender = m_subscribs[arg.m_strings[0]].m_gender;
    
    m_tables_container.delete_viewer(a_client_socket);

    if(dbg[ACTION_IN])[[unlikely]]
        std::cout << __func__ << "(): " << "call m_tables_container.get_player(" << arg.m_strings[0] << ", " << gender << ", " << arg.m_ints[0]<< ", " <<  a_client_socket << ")" << std::endl;
    
    m_tables_container.get_player(arg.m_strings[0], gender, arg.m_ints[0], a_client_socket);
    check_thread_allocation();
}

void ActionIn::start_bet(int a_client_socket)
{   
    int table_id = m_tables_container.m_player_socket_to_table_id[a_client_socket];
    m_tables_container.m_tables[table_id].get()->m_bet_round.start_bet();
}

void ActionIn::bet(char* a_buffer, int a_client_socket)
{
    Args arg(1,1);
    unpack(a_buffer, arg);
    int table_id = m_tables_container.m_player_socket_to_table_id[a_client_socket];
    m_tables_container.m_tables[table_id].get()->m_bet_round.bet_in(arg.m_ints[0]);
}

void ActionIn::finish_bet(int a_client_socket)
{
    int table_id = m_tables_container.m_player_socket_to_table_id[a_client_socket];
    m_tables_container.m_tables[table_id].get()->m_bet_round.finish_bet();
}

void ActionIn::check(int a_client_socket)
{
    int table_id = m_tables_container.m_player_socket_to_table_id[a_client_socket];
    m_tables_container.m_tables[table_id].get()->m_bet_round.chack_in();
}

void ActionIn::fold(int a_client_socket)
{
    int table_id = m_tables_container.m_player_socket_to_table_id[a_client_socket];
    m_tables_container.m_tables[table_id].get()->m_bet_round.fold_in();
}

void ActionIn::wake_up_client(int a_client_socket)
{
    m_action_out.wake_up_client(a_client_socket);
}

void ActionIn::player_deleted(int a_client_socket)
{
    m_tables_container.delete_player(a_client_socket);
    m_tables_container.delete_viewer(a_client_socket);
    check_thread_allocation();
}


}// poker namespace
