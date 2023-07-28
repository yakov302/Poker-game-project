#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include "cli_cmd.hpp"
#include "card.hpp"

#define NUM_OF_VARS 4
#define NUM_OF_CARDS 2
#define NUM_OF_FLAGS 3

namespace poker
{

using cardPointer = std::shared_ptr<Card>;

class Player
{
public:
    Player(std::string& a_name, std::string& a_gender, int a_amount, int a_socket);

    std::string m_name;
    std::string m_gender;
    std::vector<cardPointer> m_hand;
    std::unordered_map<std::string, int> m_vars;
    std::unordered_map<std::string, bool> m_flags;
};


}// poker namespace
