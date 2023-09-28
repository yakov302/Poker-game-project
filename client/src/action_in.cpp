#include "action_in.hpp"

namespace poker
{

extern Sound sound;
extern std::string empty;
extern std::string log_in;
extern std::string logged;
extern std::string bet_flag;
extern std::string register_flag;
extern std::string reveal_cards_flag;
extern int all_in_max_bet_amount;
extern int play_or_view;
extern bool all_in_flag;

namespace impl
{

std::pair<std::string, std::string> name_and_flag(char* a_buffer)
{
    Args arg(2, 0);
    std::pair<std::string, std::string> pair;
    unpack(a_buffer, arg);
    pair.first = arg.m_strings[0];
    pair.second = arg.m_strings[1];
    return pair;
}

std::pair<std::string, int> name_and_amount(char* a_buffer)
{
    Args arg(1, 1);
    std::pair<std::string, int> pair;
    unpack(a_buffer, arg);
    pair.first = arg.m_strings[0];
    pair.second = arg.m_ints[0];
    return pair;
}

std::string get_name(char* a_buffer)
{
    Args arg(1, 0);
    unpack(a_buffer, arg);
    return arg.m_strings[0];
}

int get_amount(char* a_buffer)
{
    Args arg(0, 1);
    unpack(a_buffer, arg);
    return arg.m_ints[0];
}

std::string result(hand_results a_result)
{
    switch (a_result)
    {
    case HIGH_CARD:
        return "high card";
        break;
    
    case ONE_PAIR:
        return "one pair";
        break;

    case TWO_PAIR:
        return "two pair";
        break;

    case THREE_OF_A_KING:
        return "three";
        break;

    case STRAIGHT:
        return " straight";
        break;

    case FLUSH:
        return "flush";
        break;

    case FULL_HOUSE:
        return "full house";
        break;

    case FOR_OF_A_KING:
        return "four";
        break;

    case STRAIGHT_FLUSH:
        return "straight flush";
        break;

    case ROYAL_STRAIGHT_FLUSH:
        return "royal straight flush";
        break;
    
    default:
        break;
    }

    return "";
}

int play_text_x_pos(std::string& a_txt)
{
    return 930 + ((MAX_TEXTS_SIZE - a_txt.size())/2)*12;
}

int log_in_text_x_pos(std::string& a_txt)
{
    return 740 + ((MAX_TEXTS_SIZE - a_txt.size())/2)*12;
}

int game_text_x_pos(std::string& a_txt)
{
    return 1510 + (MAX_TEXTS_SIZE - a_txt.size())/2;
}

int win_text_x_pos(std::string& a_txt)
{
    return 760 + ((MAX_WIN_TEXT_LEN - a_txt.size())/2)*16;
}

}//namespace impl

ActionIn::ActionIn(Hand& a_cards, Wallet& a_chips, PlayersContainer& a_players, Table& a_table, Self& a_self, ActionOut& a_action_out)
: m_self(a_self)
, m_cards(a_cards)
, m_table(a_table)
, m_chips(a_chips)
, m_action_out(a_action_out)
, m_players(a_players)
{

}

void ActionIn::get(char* a_buffer)
{
    Message_type message = message_type(a_buffer);

    switch (message)
    {
    case REGISTRATION_SUCCESS:
        registration_success();
        break;

    case REGISTRATION_DUPLICATE_USER_NAME:
        registration_duplicare_name();
        break;

    case REGISTRATION_WRONG_GENDER:
        registration_wrong_gender();
        break;

    case LOG_IN_SUCCESS:
        log_in_success(a_buffer);
        break;

    case LOG_IN_WRONG_USER_NAME:
        log_in_wrong_name();
        break;

    case LOG_IN_WRONG_PASSWORD:
        log_in_wrong_password();
        break;

    case USER_NAME_ALREADY_LOG:
        user_name_alredy_log();
        break;

    case PLAY_SUCCESS:
        play_success();
        break;

    case VIEW_SUCCESS:
        view_success();
        break;

    case TURN_ON_FLAG:
        turn_on(a_buffer);
        break;

    case TURN_OFF_FLAG:
        turn_off(a_buffer);
        break;

    case START_BET_UPDATE:
        start_bet(a_buffer);
        break;

    case BET_UPDATE:
        bet(a_buffer);
        break;
    
    case INVALID_BET_MIN:
        invalid_bet_min(a_buffer);
        break;

    case INVALID_BET_MAX:
        invalid_bet_max(a_buffer);
        break;

    case CHECK_UPDATE:
        check(a_buffer);
        break;

    case FOLD_UPDATE:
        fold(a_buffer);
        break;

    case GET_CARD:
        get_card(a_buffer);
        break;   

    case GET_CHIP:
        get_chip(a_buffer);
        break;

    case GIVE_CARD:
        give_card(a_buffer);
        break;

    case GET_PLAYER:
        get_player(a_buffer);
        break;

    case DELETE_PLAYER:
        delete_player(a_buffer);
        break;

    case REVEAL_CARDS:
        reveal_cards(a_buffer);
        break;

    case TABLE_FULL:
        table_is_full();
        break;
    
    case TABLE_EMPTY:
        table_is_empty();
        break;

    case TABLE_GET_CARD:
        table_get_card(a_buffer);
        break;

    case TABLE_GET_CHIP:
        table_get_chip(a_buffer);
        break;

    case TABLE_GIVE_CARD:
        table_give_card();
        break;

    case TABLE_GIVE_CHIP:
        table_give_chip(a_buffer);
        break;

    case TABLE_CLEAR_WALLET:
        table_clear_wallet();
        break;

    case WAKE_UP_SERVER:
        wake_up_server();
        break;

    case CLEAR_ACTION:
        clear_action(a_buffer);
        break;

    case ROUND_WINER:
        round_winer(a_buffer);
        break;

    case GAME_WINER:
        game_winer(a_buffer);
        break;

    case CLEAR_TEXT:
        clear_text();
        break;

    case CLEAR_SCREEN:
        clear_screen();
        break;

    case RESULT:
        print_result(a_buffer);
        break;

    case MOVES_TO_ANOTHER_TABLE:
        moves_to_another_table();
        break;

    default:
        break;
    }
}

void ActionIn::registration_success()
{
    sound.play_positive();
    std::string txt = "Registration was successful";
    m_table.set_text(txt, impl::log_in_text_x_pos(txt), LOG_IN_TEXT_Y_POS);
    m_table.turn_off_flag(register_flag);
    m_table.turn_on_flag(log_in);
}

void ActionIn::registration_duplicare_name()
{
    sound.play_invalid();
    std::string txt = "That name is already taken";
    m_table.set_text(txt, impl::log_in_text_x_pos(txt), LOG_IN_TEXT_Y_POS);
}

void ActionIn::registration_wrong_gender()
{
    sound.play_invalid();
    std::string txt = "Wrong gender";
    m_table.set_text(txt, impl::log_in_text_x_pos(txt), LOG_IN_TEXT_Y_POS);
}

void ActionIn::log_in_success(char* a_buffer)
{
    sound.play_positive();
    Args arg (2, 0);
    unpack(a_buffer, arg);
    std::string name = arg.m_strings[0];
    std::string gender =  arg.m_strings[1];
    m_self.set_name_and_gender(name, gender);
    m_table.turn_off_flag(register_flag);
    m_table.turn_off_flag(log_in);
    m_self.turn_on_flag(logged);
    play_or_view = VIEW;
    if(m_players.num_of_players() > 0)
        clear_text();
}

void ActionIn::log_in_wrong_name()
{
    sound.play_invalid();
    std::string txt = "Wrong name";
    m_table.set_text(txt, impl::log_in_text_x_pos(txt), LOG_IN_TEXT_Y_POS);
}

void ActionIn::log_in_wrong_password()
{
    sound.play_invalid();
    std::string txt = "Wrong password";
    m_table.set_text(txt, impl::log_in_text_x_pos(txt), LOG_IN_TEXT_Y_POS);
}

void ActionIn::user_name_alredy_log()
{
    sound.play_invalid();
    std::string txt = "User already logged in";
    m_table.set_text(txt, impl::log_in_text_x_pos(txt), LOG_IN_TEXT_Y_POS);
}

void ActionIn::play_success()
{
    sound.play_positive();
    std::string name = m_self.name();
    m_players.get_player(name, m_self);
    play_or_view = PLAY;
    if(m_players.num_of_players() > 1)
        clear_text();
}

void ActionIn::view_success()
{
    sound.play_positive();
    std::string name = m_self.name();
    for(int i = 0; i < 2; ++i)
        m_players.give_card(name);
    m_players.turn_off_all_flags(name);
    m_players.set_action(name, "");
    play_or_view = VIEW;
}

void ActionIn::turn_on(char* a_buffer)
{
    std::pair<std::string, std::string> pair = impl::name_and_flag(a_buffer);
    m_players.turn_on_flag(pair.first, pair.second);
}

void ActionIn::turn_off(char* a_buffer)
{
    std::pair<std::string, std::string> pair = impl::name_and_flag(a_buffer);
    m_players.turn_off_flag(pair.first, pair.second);
}

void ActionIn::start_bet(char* a_buffer)
{
    std::pair<std::string, int> pair = impl::name_and_amount(a_buffer);
    m_players.turn_on_flag(pair.first , bet_flag);
    m_players.set_action(pair.first, "bet " + std::to_string(pair.second));
    m_players.update_current_bet(pair.first, pair.second);
}

void ActionIn::bet(char* a_buffer)
{
    std::pair<std::string, int> pair = impl::name_and_amount(a_buffer);
    m_players.bet(pair.first, pair.second);
}

void ActionIn::invalid_bet_min(char* a_buffer)
{
    sound.play_invalid(); 
    std::string txt = "Invalid bet \nMin bet: " + std::to_string(impl::get_amount(a_buffer));
    m_table.set_text(txt, impl::game_text_x_pos(txt), GAME_TEXT_Y_POS);
}

void ActionIn::invalid_bet_max(char* a_buffer)
{ 
    int amount = impl::get_amount(a_buffer);

    if(all_in_flag)
    {
        all_in_max_bet_amount = amount;
        return;
    }

    sound.play_invalid();
    std::string txt = "Invalid bet \nMax bet: " + std::to_string(amount);
    m_table.set_text(txt, impl::game_text_x_pos(txt), GAME_TEXT_Y_POS);
}

void ActionIn::check(char* a_buffer)
{
    std::string name = impl::get_name(a_buffer);
    m_players.set_action(name, "check");
}

void ActionIn::fold(char* a_buffer)
{
    std::string name = impl::get_name(a_buffer);
    m_players.set_action(name, " fold");

    for(int i = 0; i < 2; ++i)
    {
        usleep(100000);
        sound.play_card();
        m_players.give_card(name);
    }
}

void ActionIn::get_card(char* a_buffer)
{
    Args arg(2, 1);
    unpack(a_buffer, arg);
    sound.play_card();
    m_players.get_card(arg.m_strings[0], arg.m_strings[1], arg.m_ints[0]);
}

void ActionIn::get_chip(char* a_buffer)
{
    std::pair<std::string, int> pair = impl::name_and_amount(a_buffer);
    m_players.get_chip(pair.first, pair.second);
}

void ActionIn::give_card(char* a_buffer)
{
    std::string name = impl::get_name(a_buffer);
    sound.play_card();
    m_players.give_card(name);
}

void ActionIn::get_player(char* a_buffer)
{
    Args arg(2, 1);
    unpack(a_buffer, arg);
    m_players.get_player(arg.m_strings[0], arg.m_strings[1], arg.m_ints[0]);
}

void ActionIn::delete_player(char* a_buffer)
{
    std::string name = impl::get_name(a_buffer);
    m_players.delete_player(name);
}

void ActionIn::reveal_cards(char* a_buffer)
{
   std::string name = impl::get_name(a_buffer);
    m_players.turn_on_flag(name, reveal_cards_flag);
}

void ActionIn::table_get_card(char* a_buffer)
{
    std::pair<std::string, int> pair = impl::name_and_amount(a_buffer);
    sound.play_card();
    m_cards.push(pair.first, pair.second);
}

void ActionIn::table_get_chip(char* a_buffer)
{
    m_chips.push(impl::get_amount(a_buffer));
}

void ActionIn::table_is_full()
{
    std::string txt = "                Table is full\nyou are moved to another table";
    m_table.set_text(txt, impl::play_text_x_pos(txt), LOG_IN_TEXT_Y_POS);
    sleep(3);
}

void ActionIn::table_is_empty()
{
    std::string txt = "Table is empty - waiting for players";
    m_table.set_text(txt, impl::log_in_text_x_pos(txt), LOG_IN_TEXT_Y_POS);
}

void ActionIn::table_give_card()
{
    sound.play_card();
    m_cards.pop();
}

void ActionIn::table_clear_wallet()
{
    m_chips.clear();
}

void ActionIn::table_give_chip(char* a_buffer)
{
    m_chips.pop(impl::get_amount(a_buffer));
}
    
void ActionIn::wake_up_server()
{
   m_action_out.wake_up_server();
}

void ActionIn::ActionIn::clear_action(char* a_buffer)
{
    std::string name = impl::get_name(a_buffer);
    m_players.set_action(name, "");
}

void ActionIn::round_winer(char* a_buffer)
{
    sound.play_positive();
    std::string txt = impl::get_name(a_buffer) + " won!";
    m_table.set_text(txt, impl::game_text_x_pos(txt), GAME_TEXT_Y_POS);
}

void ActionIn::game_winer(char* a_buffer)
{
    std::string name = impl::get_name(a_buffer);
    if(name == m_self.name())
        sound.play_positive();

    std::string txt = impl::get_name(a_buffer) + " won the game!";
    m_table.set_text(txt, impl::win_text_x_pos(txt), WIN_TEXT_Y_POS);
}

void ActionIn::clear_text()
{
    m_table.set_text(empty, 0, 0);
}

void ActionIn::clear_screen()
{
   std::string self_name = m_self.name();
    m_players.clear(self_name);
    m_table.clear();
}

void ActionIn::print_result(char* a_buffer)
{
    std::pair<std::string, int> pair = impl::name_and_amount(a_buffer);
    m_players.set_action(pair.first, impl::result((hand_results)pair.second));
}

void ActionIn::moves_to_another_table()
{
    std::string txt = "This table closes - you are moved to another table";
    m_table.set_text(txt, impl::log_in_text_x_pos(txt), LOG_IN_TEXT_Y_POS);
    sleep(3);
    clear_text();
}


}// poker namespace
