#pragma once

#include <thread>

#include "hand.hpp"
#include "self.hpp"
#include "wallet.hpp"
#include "button.hpp"
#include "sound.hpp"
#include "text_box.hpp"
#include "action_out.hpp"
#include "players_container.hpp"

#define MAX_TEXTS_SIZE 27
#define TABLE_TEXT_SIZE 30
#define BACKGTOUND_IMAGE_SCALE 3

#define ZERO_POS 0
#define BUTTON_SIZE 60
#define GAME_TEXT_SZIE 20
#define BUTTON_X_POS 1700
#define TEXT_BUTTON_X_GAP 5
#define BUTTON_GAME_Y_POS 250
#define GAME_TEXT_BUTTON_Y_GAP 5
#define OPEN_SCREEN_TEXT_SZIE 25
#define OPEN_SCREEN_TEXT_Y_GAP 22
#define BUTTON_OPEN_SCREEN_Y_POS 370
#define BUTTON_GAME_IMAGE_SCALE 0.5
#define BUTTON_OPEN_SCREEN_IMAGE_SCALE 1

#define TEXT_BOX_SIZE 80
#define TEXT_BOX_X_POS 835
#define TEXT_BOX_Y_POS 290
#define TEXT_BOX_Y_POS 290
#define TEXT_BOX_DIGITS_LIMET 9
#define TEXT_BOX_IMAGE_SCALE 1.2

#define BUTTON_IMAGE_PATH "./resources/images/buttons/button.png"
#define TEXT_BOX_IMAGE_PATH "./resources/images/text_box/text_box.png"
#define BACKGTOUND_IMAGE_PATH  "./resources/images/background/background.jpg"

namespace poker
{

using textPointer = std::shared_ptr<Text>;
using buttonPointer = std::shared_ptr<Button>;
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
    void set_text(std::string a_type, std::string a_text);

private:
    void run_log_in();
    void draw_log_in();
    void draw_register();
    void draw_login_screen();

    void check_mouse_not_looged();
    bool check_register_button(bool a_is_on);
    bool check_log_in_button(bool a_is_on);
    bool check_password_bar();
    bool check_gender_bar();
    bool check_name_bar();

    void draw_all();
    void draw_your_turn();

    void check_mouse_looged();
    bool check_your_turn();
    bool check_go_button();
    bool check_bet_button();
    bool check_check_button();
    bool check_fold_button();
    bool check_reveal_cards();
    bool check_exchange_button();

    void check_events();

private:
    sf::Event m_event;
    sf::RenderWindow m_window;

    std::thread* m_thread;
    std::unordered_map<std::string, bool> m_flags;
    std::unordered_map<std::string, textPointer> m_texts;
    std::unordered_map<std::string, buttonPointer> m_buttons;
    std::unordered_map<std::string, textBoxPointer> m_text_boxs;
    
    Self& m_self;
    Hand& m_cards; 
    Wallet& m_chips;
    ActionOut& m_action_out;
    PlayersContainer& m_players;
};


}// poker namespace
