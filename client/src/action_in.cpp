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

std::string get_name(char* a_buffer)
{
    Args arg(1, 0);
    unpack(a_buffer, arg);
    return arg.m_strings[0];
}


}//namespace impl


ActionIn::ActionIn(Hand& a_cards, Wallet& a_chips, PlayersContainer& a_players, Table& a_table, Self& a_self)
: m_cards(a_cards)
, m_chips(a_chips)
, m_table(a_table)
, m_players(a_players)
, m_self(a_self)
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

    case TURN_ON_FLAG:
        turn_on(a_buffer);
        break;

    case TURN_OFF_FLAG:
        turn_off(a_buffer);
        break;

    case BET_UPDATE:
        bet(a_buffer);
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

    case GET_CHIPS:
        get_chips(a_buffer);
        break;
    case CLEAR_HAND:
        clear_hand(a_buffer);
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
        table_get_chips(a_buffer);
        break;

    case TABLE_CLEAR_HAND:
        table_clear_hand(a_buffer);
        break;

    case TABLE_CLEAR_CHIPS:
        table_clear_chips(a_buffer);
        break;

    default:
        break;
    }
}

void ActionIn::registration_success()
{
    m_table.set_text("Registration was successful");
    m_table.turn_off_flag("register");
    m_table.turn_on_flag("log_in");
}

void ActionIn::registration_duplicare_name()
{
    m_table.set_text("That name is already taken");
}

void ActionIn::registration_wrong_gender()
{
    m_table.set_text("         Wrong gender\n\n Only male/female allowed");
}

void ActionIn::log_in_success(char* a_buffer)
{
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
    m_table.set_text("           Wrong name");
}

void ActionIn::log_in_wrong_password()
{
    m_table.set_text("       Wrong password");
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

void ActionIn::bet(char* a_buffer)
{
    Args arg(1,1);
    unpack(a_buffer, arg);
    m_players.bet(arg.m_strings[0], arg.m_ints[0]);
}

void ActionIn::check(char* a_buffer)
{
    std::string name = impl::get_name(a_buffer);
    m_players.set_action(name, "check");
}

void ActionIn::fold(char* a_buffer)
{
    std::string name = impl::get_name(a_buffer);
    m_players.set_action(name, "fold");
    m_players.clear_hand(name);
}

void ActionIn::get_card(char* a_buffer)
{
    Args arg(2, 1);
    unpack(a_buffer, arg);
    m_players.get_card(arg.m_strings[0], arg.m_strings[1], arg.m_ints[0]);
}

void ActionIn::get_chips(char* a_buffer)
{
    // need to rite function
    std::string player_name = a_buffer;
    std::vector<int> chips({5,5,5});
    m_players.get_chips(player_name, chips);
}

void ActionIn::clear_hand(char* a_buffer)
{
    std::string name = impl::get_name(a_buffer);
    m_players.clear_hand(name);
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
    std::pair<std::string, std::string> pair = impl::name_and_flas(a_buffer);
    m_players.turn_on_flag(pair.first, pair.second);
}

void ActionIn::table_get_card(char* a_buffer)
{
    Args arg(1,1);
    unpack(a_buffer, arg);
    m_cards.push(arg.m_strings[0], arg.m_ints[0]);
}

void ActionIn::table_get_chips(char* a_buffer)
{
    Args arg(0, 1);
    unpack(a_buffer, arg);
    m_chips.push(arg.m_ints[0]);
}

void ActionIn::table_clear_hand(char* a_buffer)
{
    m_cards.clear();
}

void ActionIn::table_clear_chips(char* a_buffer)
{
    m_chips.clear();
}
    

}// poker namespace
