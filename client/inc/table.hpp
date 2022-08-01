#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <thread>

#include "hand.hpp"
#include "wallet.hpp"
#include "button.hpp"
#include "self.hpp"
#include "tcp_client.hpp"
#include "action_out.hpp"
#include "text_box.hpp"

#include "players_container.hpp"

namespace poker
{

using buttonPointer = std::shared_ptr<Button>;
using textPointer = std::shared_ptr<Text>;
using textBoxPointer  = std::shared_ptr<TextBox>;

class Table
{
public:
    Table(Hand& a_cards, Wallet& a_chips, Self& a_self, PlayersContainer& a_other_players, ActionOut& a_action_out);
    ~Table();
    
    void run();
    void stop();
    bool is_runing();
    void turn_on_flag(std::string a_flag);
    void turn_off_flag(std::string a_flag);
    void set_text(std::string a_type, std::string a_text, sf::Color a_color = sf::Color(255, 228, 156));

    sf::RenderWindow& window();

private:
    void run_log_in();
    void check_events();
    void draw_login_screen();
    void draw_log_in();
    void draw_register();
    void draw_all();
    void draw_your_turn();
    void check_mouse_looged();
    bool check_your_turn();
    bool check_go_button();
    bool check_bet_button();
    bool check_check_button();
    bool check_fold_button();
    bool check_exchange_button();
    bool check_reveal_cards();
    void check_mouse_not_looged();
    bool check_log_in_button(bool a_is_on);
    bool check_register_button(bool a_is_on);
    bool check_name_bar();
    bool check_password_bar();
    bool check_gender_bar();

private:
    sf::Event m_event;
    sf::RenderWindow m_window;
    std::thread* m_thread;
    std::unordered_map<std::string, bool> m_flags;
    std::unordered_map<std::string, textPointer> m_texts;
    std::unordered_map<std::string, buttonPointer> m_buttons;
    std::unordered_map<std::string, textBoxPointer> m_text_boxs;
    
    Hand& m_cards; 
    Wallet& m_chips;
    Self& m_self;
    PlayersContainer& m_players;
    ActionOut& m_action_out;
};


}// poker namespace