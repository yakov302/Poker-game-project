#include"player.hpp"

namespace poker
{

namespace impl
{

void set_flags(std::unordered_map<std::string, bool>& a_flags)
{
    a_flags["bet"]          = false;
    a_flags["my_turn"]      = false;
    a_flags["reveal_cards"] = false;
}

void set_texts(std::unordered_map<std::string, textPointer>& a_texts, std::string& a_name, int a_x, int a_y, int a_amount)
{
    int name_x_pos = a_x + (MAX_NAME_SIZE-a_name.size())*6.5;

    a_texts["name"]      = textPointer(new Text(ARIAL_FONT_PATH, a_name,                   NAME_COLOR, TEXT_SIZE, name_x_pos, a_y + 110));
    a_texts["amount"]    = textPointer(new Text(ARIAL_FONT_PATH, std::to_string(a_amount), TEXT_COLOR, TEXT_SIZE, a_x + 50,   a_y + 140));
    a_texts["action"]    = textPointer(new Text(ARIAL_FONT_PATH, "",                       TEXT_COLOR, TEXT_SIZE, a_x,        a_y      ));
    a_texts["your_turn"] = textPointer(new Text(ARIAL_FONT_PATH, "Your turn",              TEXT_COLOR, TEXT_SIZE, a_x + 18,   a_y - 37 ));
}


}//impl namespace

Player::Player(std::string a_name, std::string a_gender, int a_amount, int a_x_self, int a_y_self, int a_x_card, int a_y_card, float a_scale_card, int a_gap_front, int a_gap_back)
: m_x(a_x_self)
, m_y(a_y_self)
, m_current_bet(0)
, m_total_amount(a_amount)
, m_flags()
, m_texts() 
, m_hand(NUM_OF_CARDS_PLAYER_HOLD, NUM_OF_CARDS_PLAYER_HOLD, a_x_card, a_y_card, a_scale_card, a_gap_front, a_gap_back)
{
    set_shape(a_gender);
    impl::set_texts(m_texts, a_name, a_x_self, a_y_self, m_total_amount);
    impl::set_flags(m_flags); 
}

void Player::get_chip(int a_chip)
{
    m_total_amount += a_chip;
    set_amount(std::to_string(m_total_amount));
}

void Player::bet(int a_amount)
{
    m_current_bet += a_amount;
    m_total_amount -= a_amount;
    set_amount(std::to_string(m_total_amount));
    set_action("bet " + std::to_string(m_current_bet));
}

void Player::update_current_bet(int a_amount)
{
    m_current_bet = a_amount;
}

void Player::get_card(std::string& a_suit, int a_number)
{
    m_hand.push(a_suit, a_number);
}

void Player::give_card()
{
    m_hand.pop();
}

void Player::set_action(std::string a_action)
{
    m_texts["action"].get()->set_text(a_action);
    m_texts["action"].get()->set_position(m_x + 20 + ((MAX_ACTION_SIZE - a_action.size())/2)*10, m_y + 167);
}

void Player::set_amount(std::string a_amount)
{
    m_texts["amount"].get()->set_text(a_amount);
    m_texts["amount"].get()->set_position(m_x + 40 + ((MAX_AMOUNT_DIGIT - a_amount.size())/2)*11, m_y + 140);
}

void Player::turn_on_flag(std::string a_flag)
{
    if(m_flags.find(a_flag) == m_flags.end())
        return;
    
    m_flags[a_flag] = true;
}

void Player::turn_off_flag(std::string a_flag)
{
    if(m_flags.find(a_flag) == m_flags.end())
        return;
    
    m_flags[a_flag] = false;
}
 
void Player::draw_player(sf::RenderWindow& a_window)
{
    a_window.draw(m_shape);
    m_hand.draw_back(a_window);

    m_texts["name"].get()->draw(a_window);
    m_texts["amount"].get()->draw(a_window);
    m_texts["action"].get()->draw(a_window);

    if(m_flags["my_turn"])
       m_texts["your_turn"].get()->draw(a_window);

    if(m_flags["reveal_cards"])
        m_hand.draw_front(a_window);
}

void Player::set_shape(std::string& a_gender, float a_scale)
{
    std::string imagePath = "./resources/images/avatars/" + a_gender + ".png";
    m_texture.loadFromFile(imagePath);
    m_shape.setTexture(m_texture);
    m_shape.scale(a_scale, a_scale);
    m_shape.setPosition(m_x, m_y);
}

void Player::set_name(std::string& a_name)
{
    int x_pos =  m_x + (MAX_NAME_SIZE - a_name.size())*6.5;
    m_texts["name"] = textPointer(new Text(ARIAL_FONT_PATH, a_name, NAME_COLOR, TEXT_SIZE, x_pos, m_y + 110));
}


}// poker namespace
