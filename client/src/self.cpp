#include"self.hpp"

namespace poker
{

std::string logged = "logged";
std::string exchange = "exchange";

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
    a_flags["logged"]   = false;
    a_flags["exchange"] = false;
}


}//impl namespace

Self::Self(std::string a_name, std::string a_gender,  int a_amount)
: Player(a_name, a_gender, a_amount, SELF_X_POS, SELF_Y_POS, HAND_X_POS, HAND_Y_POS, HAND_SCALE, GAP_BETWEEN_FRONT_CARDS, GAP_BETWEEN_BACK_CARDS)
, m_wallet(SELF_WALLET_X_POS, SELF_WALLET_Y_POS)
, m_name(a_name)
{
    impl::fill_wallet(m_wallet);
    impl::set_self_flags(m_flags);
    set_amount(std::to_string(m_wallet.amount()));
}

int Self::bet(int a_x, int a_y)
{
    return m_wallet.amount_by_position(a_x, a_y);
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
    set_amount(std::to_string(m_wallet.amount()));
}

void Self::get_chip(int a_chip)
{
    m_wallet.push(a_chip);
    set_amount(std::to_string(m_wallet.amount()));
}

void Self::draw_player(sf::RenderWindow& a_window)
{
    a_window.draw(m_shape);
    m_wallet.draw(a_window, false);
    m_hand.draw_back(a_window);

    m_texts["name"].get()->draw(a_window);
    m_texts["amount"].get()->draw(a_window);
    m_texts["action"].get()->draw(a_window);

    if(m_flags["my_turn"])
       m_texts["your_turn"].get()->draw(a_window);

    if(m_flags["reveal_cards"])
        m_hand.draw_front(a_window);
}

int Self::amount()const
{
    return m_wallet.amount();
}

const std::string& Self::name()
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

bool Self::is_flag_on(std::string& a_flag)
{
    if(m_flags.find(a_flag) == m_flags.end())[[unlikely]]
        return false;

    return m_flags[a_flag];
}

void Self::set_name_and_gender(std::string& a_name, std::string& a_gender)
{
    if(a_gender == "female")
        set_shape(a_gender);
    set_name(a_name);
    m_name = a_name;
}


}// poker namespace
