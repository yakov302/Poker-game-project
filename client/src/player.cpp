#include"player.hpp"

namespace poker
{

namespace impl
{

void set_flags(std::unordered_map<std::string, bool>& a_flags)
{
    a_flags["my_turn"] = false;
    a_flags["bet"] = false;
    a_flags["reveal_cards"] = false;
}

void set_texts(std::unordered_map<std::string, textPointer>& a_texts, std::string& a_name, int a_x, int a_y, int a_amount)
{
    a_texts["name"] = textPointer(new Text("./resources/fonts/arial.ttf", a_name, sf::Color(139, 134, 134), 25, a_x + (11-a_name.size())*6.5, a_y + 110));
    a_texts["your_turn"] = textPointer(new Text("./resources/fonts/arial.ttf", "Your turn", sf::Color(255, 228, 156), 25, a_x + 18, a_y - 37));
    a_texts["amount"] = textPointer(new Text("./resources/fonts/arial.ttf", std::to_string(a_amount), sf::Color(255, 228, 156), 25, a_x + 50, a_y + 140));
    a_texts["action"] = textPointer(new Text("./resources/fonts/arial.ttf", "", sf::Color(255, 228, 156), 25, a_x + 30, a_y + 167));
    a_texts["fold"] = textPointer(new Text("./resources/fonts/arial.ttf", "", sf::Color(255, 228, 156), 25, a_x + 30, a_y + 167));
}


}//impl namespace

Player::Player(std::string a_name, std::string a_gender, int a_amount, int a_x_self, int a_y_self, int a_x_card, int a_y_card, float a_scale_card, int a_gap_of_printing)
: m_x(a_x_self)
, m_y(a_y_self)
, m_hand(2, 2, a_x_card, a_y_card, a_scale_card, a_gap_of_printing)
, m_current_bet(0)
, m_total_amount(a_amount)
, m_flags()
, m_texts() 
{
    set_shape(m_shape, m_texture, a_gender,a_x_self,a_y_self);
    impl::set_texts(m_texts, a_name, a_x_self, a_y_self, m_total_amount);
    impl::set_flags(m_flags); 
}

void Player::get_chip(int a_chip)
{
    m_total_amount += a_chip;
    m_texts["amount"].get()->set_text(std::to_string(m_total_amount));
}

void Player::bet(int a_amount)
{
    m_current_bet += a_amount;
    m_total_amount -= a_amount;
    m_texts["amount"].get()->set_text(std::to_string(m_total_amount));
    m_texts["action"].get()->set_text("bet " + std::to_string(m_current_bet));
}

void Player::get_card(std::string& a_suit, int a_number)
{
    m_hand.push(a_suit, a_number);
}

void Player::clear_hand()
{
    m_hand.clear();
}

void Player::set_action(std::string a_action)
{
    m_texts["action"].get()->set_text(a_action);
}

void Player::set_fold(std::string a_text)
{
    m_texts["fold"].get()->set_text(a_text);
}

void Player::turn_on_flag(std::string a_flag)
{
    if(m_flags.find(a_flag) == m_flags.end())
        return;
    
    m_flags[a_flag] = true;

    if(a_flag == "bet")
    {
        // m_current_bet = 0;
        // m_texts["action"].get()->set_text("bet " + std::to_string(m_current_bet));
    }
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
    m_texts["fold"].get()->draw(a_window);

    if(m_flags["my_turn"])
       m_texts["your_turn"].get()->draw(a_window);

    if(m_flags["reveal_cards"])
        m_hand.draw_front(a_window);
}


void Player::set_shape(sf::Sprite& a_shape, sf::Texture& a_texture, std::string& a_gender, int a_x, int a_y)
{
    std::string imagePath = "./resources/images/avatars/" + a_gender + ".png";
    a_texture.loadFromFile(imagePath);
    a_shape.setTexture(a_texture);
    a_shape.scale(1, 1);
    a_shape.setPosition(a_x, a_y);
}

void Player::set_name(std::unordered_map<std::string, textPointer>& a_texts, std::string& a_name, int a_x, int a_y, int a_amount)
{
    a_texts["name"] = textPointer(new Text("./resources/fonts/arial.ttf", a_name, sf::Color(139, 134, 134), 25, a_x + (11-a_name.size())*6.5, a_y + 110));
}

}// poker namespace
