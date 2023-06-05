#include "wallet.hpp"

namespace poker
{

ChipsContainer chip_container;

namespace impl
{

void make_reserve(std::unordered_map<int, std::vector<chipPointer>>& a_wallet)
{
    a_wallet[1].reserve(MAX_CHIPS_IN_COLUMN);
    a_wallet[5].reserve(MAX_CHIPS_IN_COLUMN);
    a_wallet[10].reserve(MAX_CHIPS_IN_COLUMN);
    a_wallet[25].reserve(MAX_CHIPS_IN_COLUMN);
    a_wallet[50].reserve(MAX_CHIPS_IN_COLUMN);
    a_wallet[100].reserve(MAX_CHIPS_IN_COLUMN);
    a_wallet[500].reserve(MAX_CHIPS_IN_COLUMN);
    a_wallet[1000].reserve(MAX_CHIPS_IN_COLUMN);
    a_wallet[5000].reserve(MAX_CHIPS_IN_COLUMN);
    a_wallet[10000].reserve(MAX_CHIPS_IN_COLUMN);
}


}//namespace impl

Wallet::Wallet(int a_x, int a_y, std::vector<int> a_amounts)
: m_x(a_x)
, m_y(a_y)
, m_amount(TEXT_FONT, "0", TEXT_COLOR, TEXT_SIZE, m_x + AMOUNT_X_GAP_POS, m_y + AMOUNT_Y_GAP_POS)
, m_total_amount(0)
, m_mutex()
, m_wallet()
{
    impl::make_reserve(m_wallet);
    for(auto& amount : a_amounts)
        push(amount);
}

void Wallet::push(int a_amount)
{
    m_sound.play_chip();
    m_wallet[a_amount].push_back(chip_container.give_chip(a_amount));
    m_total_amount += a_amount;
    auto_exchange(a_amount);
    set_positions();
}

void Wallet::push(std::vector<int> a_amounts)
{
    for(auto& amount : a_amounts)
        push(amount);
}

void Wallet::pop(int a_amount)
{
    Lock lock(m_mutex);
    m_sound.play_chip();
    if(!m_wallet[a_amount].empty())
    {
        chip_container.release_chip(a_amount, m_wallet[a_amount].back().get()->index_id());
        m_wallet[a_amount].pop_back();
        m_total_amount -= a_amount;
    }
    set_positions();
}

void Wallet::clear()
{
    for(auto& vector : m_wallet)
    {
        while(!vector.second.empty())
            pop(vector.first);
    }
}

void Wallet::set_positions()
{
    int x = m_x;
    for(auto& vec : m_wallet)
    {
        if(!vec.second.empty())
        {
            int y = m_y;
            for(auto& chip : vec.second)
                chip.get()->set_position(x, y -= GAP_BETWEEN_CHIPS);

            x += DIRECTION_OF_NEXT_CHIPS_COLUMNS*CHIP_SIZE;
        }
    }
}

void Wallet::draw(sf::RenderWindow& a_window, bool a_print_amount)
{
    Lock lock(m_mutex);

    for(auto& vec : m_wallet)
    {
        for(auto& chip : vec.second)
            chip.get()->draw(a_window);
    }

    if(a_print_amount)
        print_amount(a_window);
}

size_t Wallet::amount() const
{
    return m_total_amount;
}

int Wallet::amount_by_position(int a_x, int a_y)const
{
    for(auto& vec : m_wallet)
    {
        if(!vec.second.empty())
        {
            int lest = vec.second.size() - 1;
            if( a_x <= vec.second[lest].get()->right()  - (vec.second[lest].get()->width()/4)  &&
                a_x >= vec.second[lest].get()->left()   + (vec.second[lest].get()->width()/4)  &&
                a_y <= vec.second[lest].get()->bottom() - (vec.second[lest].get()->height()/3) &&
                a_y >= vec.second[lest].get()->top()    + (vec.second[lest].get()->height()/3)
            )return vec.first;
        }
    }
    return 0;
}

size_t Wallet::size() const
{
    size_t size = 0;
    for(auto& vec : m_wallet)
        size += vec.second.size();
    return size;
}

void Wallet::print_amount(sf::RenderWindow& a_window) 
{
    if(m_total_amount > 0)
    {
        m_amount.set_text(std::to_string(m_total_amount));
        m_amount.draw(a_window);
    }
}

void Wallet::auto_exchange(int a_amount)
{
    if(m_wallet[a_amount].size() >= MAX_CHIPS_IN_COLUMN)
    {
        switch (a_amount)
        {
        case 1:
            for(int i = 0; i < 5; ++i)
                pop(1);    
            push(5);
            break;
        
        case 5:
            for(int i = 0; i < 2; ++i)
                pop(5);    
            push(10);
            break;
        
        case 10:
            for(int i = 0; i < 2; ++i)
                pop(10);
            pop(5);    
            push(25);
            break;

        case 25:
            for(int i = 0; i < 2; ++i)
                pop(25);   
            push(50);
            break;

        case 50:
            for(int i = 0; i < 2; ++i)
                pop(50);   
            push(100);
            break;

        case 100:
            for(int i = 0; i < 5; ++i)
                pop(100);   
            push(500);
            break;

        case 500:
            for(int i = 0; i < 2; ++i)
                pop(500);   
            push(1000);
            break;

        case 1000:
            for(int i = 0; i < 5; ++i)
                pop(1000);   
            push(5000);
            break;

        case 5000:
            for(int i = 0; i < 2; ++i)
                pop(5000);   
            push(10000);
            break;
        
        default:
            break;
        }
    }
}

void Wallet::exchange(int a_amount)
{
    if(!m_wallet[a_amount].empty())
    {
        switch (a_amount)
        {    
        case 5:
            for(int i = 0; i < 5; ++i)
                push(1);    
            pop(5);
            break;
        
        case 10:
            for(int i = 0; i < 2; ++i)
                push(5);   
            pop(10);
            break;

        case 25:
            for(int i = 0; i < 2; ++i)
                push(10);
            push(5); 
            pop(25);
            break;

        case 50:
            for(int i = 0; i < 2; ++i)
                push(25);   
            pop(50);
            break;

        case 100:
            for(int i = 0; i < 2; ++i)
                push(50);   
            pop(100);
            break;

        case 500:
            for(int i = 0; i < 5; ++i)
                push(100);   
            pop(500);
            break;

        case 1000:
            for(int i = 0; i < 2; ++i)
                push(500);   
            pop(1000);
            break;

        case 5000:
            for(int i = 0; i < 5; ++i)
                push(1000);   
            pop(5000);
            break;

        case 10000:
            for(int i = 0; i < 2; ++i)
                push(5000);   
            pop(10000);
            break;
        
        default:
            break;
        } 
    }
}


}//namespace poker
