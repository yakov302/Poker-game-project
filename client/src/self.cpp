#include"self.hpp"

namespace poker
{

namespace impl
{
    
void fill_wallet(Wallet& a_wallet)
{
    a_wallet.push({ 100,100,100,
                    50,50,50,50,50,50,
                    25,25,25,25,25,25,25,25,
                    10,10,10,10,10,10,10,10,10,10,
                    5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
                    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1});
}

void set_self_flags(std::unordered_map<std::string, bool>& a_flags)
{
    a_flags["exchange"] = false;
    a_flags["logged"] = false;
}


}//impl namespace

Self::Self(std::string a_name, std::string a_gender,  int a_amount, int a_x_self, int a_y_self, int a_x_card, int a_y_card, float a_scale_card, int a_gap_front, int a_gap_back)
: Player(a_name, a_gender, a_amount, a_x_self, a_y_self, a_x_card, a_y_card, a_scale_card,  a_gap_front, a_gap_back)
, m_wallet(a_x_self + 50, a_y_self + 140)
, m_name(a_name)
{
    impl::fill_wallet(m_wallet);
    impl::set_self_flags(m_flags);
}

int Self::bet(int a_x, int a_y)
{
    return  m_wallet.amount_by_position(a_x, a_y);
}

bool Self::exchange(int a_x, int a_y)
{
    int amount = m_wallet.amount_by_position(a_x, a_y);
    if(amount != 0)
    {
        m_wallet.exchange(amount);
        return true;
    }
    return false;
}

void Self::bet(int a_amount)
{
    m_current_bet += a_amount;
    m_wallet.pop(a_amount);
    set_action("bet " + std::to_string(m_current_bet));
}

void Self::get_chip(int a_chip)
{
    m_wallet.push(a_chip);
}

void Self::draw_player(sf::RenderWindow& a_window)
{
    a_window.draw(m_shape);
    set_amount(std::to_string(m_wallet.amount()));
    m_wallet.draw(a_window, 880, 750, -1);
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

int Self::amount()const
{
    return m_wallet.amount();
}

std::string Self::name() const
{
    return m_name;
}

bool Self::is_in_back_range(int a_x, int a_y)const
{
    return m_hand.is_in_back_range(a_x, a_y);
}

bool Self::is_in_wallet_range(int a_x, int a_y)const
{
    if(m_wallet.amount_by_position(a_x, a_y) == 0)
        return false;

    return true;
}

void Self::draw_hand_front(sf::RenderWindow& a_window)const
{
    m_hand.draw_front(a_window);
}

bool Self::is_flag_on(std::string a_flag)
{
    if(m_flags.find(a_flag) == m_flags.end())
        return false;

    return m_flags[a_flag];
}

void Self::set_name_and_gender(std::string a_name, std::string a_gender)
{
    set_shape(m_shape, m_texture, a_gender,m_x, m_y);
    set_name(m_texts, a_name, m_x, m_y, m_total_amount);
    m_name = a_name;
}


}// poker namespace
