# pragma once

#include <memory>
#include <vector>
#include <algorithm>

#include "card.hpp"
#include "cli_cmd.hpp"
#include "action_out.hpp"
#include "tcp_server.hpp"

#define NUM_OF_TABLE_CARDS 5
#define NUM_OF_TABLE_CHIPS 200

namespace poker
{

using cardPointer = std::shared_ptr<Card>;

class Table
{
public:
    Table(TcpServer& a_tcp_server);

    int give_chip();
    bool give_chips(std::vector<int>& chips, int amount);
    cardPointer give_card();
    void get_chip(int a_chip);
    void get_card(cardPointer a_card);
    void send_new_player_existing_chips_and_cards(int a_client_socket);

    bool is_hand_empty();
    bool is_wallet_empty();

    int num_of_card()const;
    int total_chips_amount()const;

    std::vector<cardPointer>& table_cards();

private:
    int m_chips_total_amount;
    ActionOut m_action_out;
    std::vector<int> m_chips;
    std::vector<cardPointer> m_cards;
};


}// poker namespace
