#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <mutex>

#include "chip.hpp"
#include "text.hpp"
#include "sound.hpp"


namespace poker
{

using Lock = std::unique_lock<std::mutex>;
using chipPointer = std::shared_ptr<Chip>;

class Wallet
{
public:
    Wallet(int a_x = 0, int a_y = 0, std::vector<int> a_amounts = {});

    void push(int a_amount);  
    void push(std::vector<int> a_amounts);
    void pop(int a_amount);

    //void clear();
    void exchange(int a_amount);
    void print_amount(sf::RenderWindow& a_window);

    size_t size() const;
    size_t amount() const;
    int amount_by_position(int a_x, int a_y)const;
    void draw(sf::RenderWindow& a_window, int a_x, int a_y, int a_gap_direction)const;

private:
    void auto_exchange(int a_amount);
    
private:
    Text m_amount;
    Sound m_sound;
    size_t m_total_amount;
    mutable std::mutex m_mutex;
    std::unordered_map<int, std::vector<chipPointer>> m_wallet;
};


}//namespace poker
