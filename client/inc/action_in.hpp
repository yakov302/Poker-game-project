#pragma once

#include <utility>

#include "players_container.hpp"
#include "self.hpp"
#include "player.hpp"
#include "hand.hpp"
#include "wallet.hpp"
#include "table.hpp"
#include "action_out.hpp"

namespace poker
{

class ActionIn
{
public:
    ActionIn(Hand& a_cards, Wallet& a_chips, PlayersContainer& a_players, Table& a_table, Self& a_self, ActionOut& a_action_out);

    void get(char* a_buffer);

private:
    void registration_success();
    void registration_duplicare_name();
    void registration_wrong_gender();
    void log_in_success(char* a_buffer);
    void log_in_wrong_name();
    void log_in_wrong_password();
    void user_name_alredy_log();
    void turn_on(char* a_buffer);
    void turn_off(char* a_buffer);
    void start_bet(char* a_buffer);
    void bet(char* a_buffer);
    void invalid_bet_min(char* a_buffer);
    void invalid_bet_max(char* a_buffer);
    void check(char* a_buffer);
    void fold(char* a_buffer);
    void clear_fold(char* a_buffer);
    void get_card(char* a_buffer);
    void get_chips(char* a_buffer);
    void clear_hand(char* a_buffer);
    void get_player(char* a_buffer);
    void delete_player(char* a_buffer);
    void reveal_cards(char* a_buffer);
    void table_get_card(char* a_buffer);
    void table_get_chips(char* a_buffer);
    void table_clear_hand();
    void table_clear_chips();
    void wake_up_server();
    void clear_action(char* a_buffer);
    void round_winer(char* a_buffer);
    void game_winer(char* a_buffer);
    void clear_text();
    
private:
    Hand& m_cards; 
    Wallet& m_chips;
    Table& m_table;
    PlayersContainer& m_players;
    Self& m_self;
    ActionOut& m_action_out;
};


}// poker namespace
