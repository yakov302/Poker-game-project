#pragma once

#include <stack>
#include <memory>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "chip.hpp"

#define MAX_CHIPS_IN_COLUMN 21
#define AMOUNT_OF_EACH_CHIP MAX_CHIPS_IN_COLUMN*2
#define CHIPS_CONFIG_FILE_PATH "./resources/txt/chips_config.txt"

namespace poker
{

using chipPointer = std::shared_ptr<Chip>;

class ChipsContainer
{
public:
    ChipsContainer();
    chipPointer give_chip(int a_number);
    void release_chip(int a_number, int index_id);

private:
    std::unordered_map<int, std::vector<chipPointer>> m_chips;
    std::unordered_map<int, std::stack<int>> m_availables_stack;
};


}// poker namespace
