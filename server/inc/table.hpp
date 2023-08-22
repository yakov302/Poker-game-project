# pragma once

#include <memory>
#include <vector>

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
    cardPointer give_card();
    void get_chip(int a_chip);
    void get_card(cardPointer a_card);
    void send_new_player_existing_chips_and_cards(int a_client_socket);

    bool is_hand_empty();
    bool is_wallet_empty();

    int num_of_card()const;

    std::vector<cardPointer>& table_cards();

private:
    ActionOut m_action_out;
    std::vector<int> m_chips;
    std::vector<cardPointer> m_cards;
};


}// poker namespace
