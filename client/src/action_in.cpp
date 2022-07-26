#include "action_in.hpp"

namespace poker
{

namespace impl
{

std::pair<std::string, std::string> name_and_flas(char* a_buffer)
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

std::string result(int a_result)
{
    switch (a_result)
    {
    case 1:
        return "high card";
        break;
    
    case 2:
        return "one pair";
        break;

    case 3:
        return "two pair";
        break;

    case 4:
        return "three";
        break;

    case 5:
        return " straight";
        break;

    case 6:
        return "flush";
        break;

    case 7:
        return "full house";
        break;

    case 8:
        return "four";
        break;

    case 9:
        return "straight flush";
        break;

    case 10:
        return "royal straight flush";
        break;
    
    default:
        break;
    }

    return "";
}


}//namespace impl

ActionIn::ActionIn(Hand& a_cards, Wallet& a_chips, PlayersContainer& a_players, Table& a_table, Self& a_self, ActionOut& a_action_out)
: m_sound()
, m_self(a_self)
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

    case RESULT:
        print_result(a_buffer);
        break;

    default:
        break;
    }
}

void ActionIn::registration_success()
{
    m_sound.play_positive();
    m_table.set_text("log_in", "Registration was successful");
    m_table.turn_off_flag("register");
    m_table.turn_on_flag("log_in");
}

void ActionIn::registration_duplicare_name()
{
    m_sound.play_invalid();
    m_table.set_text("log_in", "That name is already taken");
}

void ActionIn::registration_wrong_gender()
{
    m_sound.play_invalid();
    m_table.set_text("log_in", "Wrong gender");
}

void ActionIn::log_in_success(char* a_buffer)
{
    m_sound.play_positive();
    Args arg (2, 0);
    unpack(a_buffer, arg);
    std::string name = arg.m_strings[0];
    std::string gender =  arg.m_strings[1];
    m_self.set_name_and_gender(name, gender);
    m_players.get_player(name, m_self);
    m_table.turn_off_flag("register");
    m_table.turn_off_flag("log_in");
    m_self.turn_on_flag("logged");
}

void ActionIn::log_in_wrong_name()
{
    m_sound.play_invalid();
    m_table.set_text("log_in", "Wrong name");
}

void ActionIn::log_in_wrong_password()
{
    m_sound.play_invalid();
    m_table.set_text("log_in", "Wrong password");
}

void ActionIn::user_name_alredy_log()
{
    m_sound.play_invalid();
    m_table.set_text("log_in", "User already logged in");
}

void ActionIn::turn_on(char* a_buffer)
{
    std::pair<std::string, std::string> pair = impl::name_and_flas(a_buffer);
    m_players.turn_on_flag(pair.first, pair.second);
}

void ActionIn::turn_off(char* a_buffer)
{
    std::pair<std::string, std::string> pair = impl::name_and_flas(a_buffer);
    m_players.turn_off_flag(pair.first, pair.second);
}

void ActionIn::start_bet(char* a_buffer)
{
    std::pair<std::string, int> pair = impl::name_and_amount(a_buffer);
    m_players.turn_on_flag(pair.first , "bet");
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
    m_sound.play_invalid(); 
    m_table.set_text("text", "Invalid bet \nMin bet: " + std::to_string(impl::get_amount(a_buffer)));
}

void ActionIn::invalid_bet_max(char* a_buffer)
{ 
    m_sound.play_invalid();
    m_table.set_text("text", "Invalid bet \nMax bet: " + std::to_string(impl::get_amount(a_buffer)));
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
        m_sound.play_card();
        m_players.give_card(name);
    }
}

void ActionIn::get_card(char* a_buffer)
{
    Args arg(2, 1);
    unpack(a_buffer, arg);
    m_sound.play_card();
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
    m_sound.play_card();
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
    m_players.turn_on_flag(name, "reveal_cards");
}

void ActionIn::table_get_card(char* a_buffer)
{
    std::pair<std::string, int> pair = impl::name_and_amount(a_buffer);
    m_sound.play_card();
    m_cards.push(pair.first, pair.second);
}

void ActionIn::table_get_chip(char* a_buffer)
{
    m_chips.push(impl::get_amount(a_buffer));
}

void ActionIn::table_give_card()
{
    m_sound.play_card();
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
    m_sound.play_positive();
    std::string name = impl::get_name(a_buffer);
    m_table.set_text("text", name + " won!");
}

void ActionIn::game_winer(char* a_buffer)
{
    std::string name = impl::get_name(a_buffer);
    if(name == m_self.name())
        m_sound.play_positive();

    m_table.set_text("text", name + " won the game!");
}

void ActionIn::clear_text()
{
    m_table.set_text("text", "");
}

void ActionIn::print_result(char* a_buffer)
{
    std::pair<std::string, int> pair = impl::name_and_amount(a_buffer);
    m_players.set_action(pair.first, impl::result(pair.second));
}


}// poker namespace
