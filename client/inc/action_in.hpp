#pragma once

#include "self.hpp"
#include "hand.hpp"
#include "table.hpp"
#include "sound.hpp"
#include "wallet.hpp"
#include "action_out.hpp"
#include "players_container.hpp"

#define PLAY 1
#define VIEW 2
#define MAX_WIN_TEXT_LEN 23
#define WIN_TEXT_Y_POS 390
#define GAME_TEXT_Y_POS 680
#define LOG_IN_TEXT_Y_POS 80

namespace poker
{

class ActionIn
{
public:
    ActionIn(Hand& a_cards, Wallet& a_chips, PlayersContainer& a_players, Table& a_table, Self& a_self, ActionOut& a_action_out);

    void get(char* a_buffer);

private:
    void registration_success();
    void registration_wrong_gender();
    void registration_duplicare_name();

    void log_in_wrong_name();
    void user_name_alredy_log();
    void log_in_wrong_password();
    void log_in_success(char* a_buffer);

    void play_success();
    void view_success();

    void turn_on(char* a_buffer);
    void turn_off(char* a_buffer);

    void bet(char* a_buffer);
    void start_bet(char* a_buffer);
    void invalid_bet_min(char* a_buffer);
    void invalid_bet_max(char* a_buffer);

    void check(char* a_buffer);
    void fold(char* a_buffer);

    void get_card(char* a_buffer);
    void get_chip(char* a_buffer);
    void give_card(char* a_buffer);
    void reveal_cards(char* a_buffer);

    void get_player(char* a_buffer);
    void delete_player(char* a_buffer);

    void table_is_full();
    void table_is_empty();
    void table_give_card();
    void table_clear_wallet();
    void table_get_card(char* a_buffer);
    void table_get_chip(char* a_buffer);
    void table_give_chip(char* a_buffer);

    void clear_text();
    void clear_screen();
    void wake_up_server();
    void clear_action(char* a_buffer);
    void round_winer(char* a_buffer);
    void game_winer(char* a_buffer);
    void print_result(char* a_buffer);
    
private:
    Self& m_self;
    Hand& m_cards; 
    Table& m_table;
    Wallet& m_chips;
    ActionOut& m_action_out;
    PlayersContainer& m_players;
};


}// poker namespace
