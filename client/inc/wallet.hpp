#pragma once

#include <unordered_map>
#include <memory>
#include <mutex>

#include "chip.hpp"
#include "text.hpp"
#include "sound.hpp"
#include "chips_container.hpp"

namespace poker
{

#define CHIP_SIZE 90
#define TEXT_SIZE 25
#define GAP_BETWEEN_CHIPS 7
#define AMOUNT_X_GAP_POS 45
#define AMOUNT_Y_GAP_POS 92
#define MAX_CHIPS_IN_COLUMN 21
#define DIRECTION_OF_NEXT_CHIPS_COLUMNS -1
#define TEXT_COLOR sf::Color(255, 228, 156)
#define TEXT_FONT_PATH "./resources/fonts/arial.ttf"

using Lock = std::unique_lock<std::mutex>;
using chipPointer = std::shared_ptr<Chip>;

class Wallet
{
public:
    Wallet(int a_x = 0, int a_y = 0, std::vector<int> a_amounts = {});

    void push(std::vector<int> a_amounts);
    void push(int a_amount);  
    void pop(int a_amount);
    void clear();

    void exchange(int a_amount);

    size_t size() const;
    size_t amount() const;
    int amount_by_position(int a_x, int a_y)const;
    void draw(sf::RenderWindow& a_window, bool a_print_amount);

private:
    void set_positions();
    void auto_exchange(int a_amount);
    void print_amount(sf::RenderWindow& a_window);
 
private:
    int m_x;
    int m_y;
    Text m_amount;
    size_t m_total_amount;
    mutable std::mutex m_mutex;
    std::unordered_map<int, std::vector<chipPointer>> m_wallet;
};


}//namespace poker
