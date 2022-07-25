#include "card_round.hpp"

namespace poker
{

CardRound::CardRound(PlayersContainer& a_players, Table& a_table, ActionOut& a_action_out, BetRound& a_bet_round)
: m_stop(false)
, m_bet(false)
, m_deck(1)
, m_players (a_players)
, m_table(a_table)
, m_action_out(a_action_out)
, m_bet_round(a_bet_round)
{

}

void CardRound::run(playerIterator a_open_player)
{
    m_stop = false;
    deal_cards();
    while(!m_stop)
    {
        int num_of_cards = m_table.num_of_card();
        switch (num_of_cards)
        {
        case 0:
            if(!m_bet)
                bet(a_open_player);
            else
                open_three_cards();
            break;

        case 3:
            if(!m_bet)
                bet(a_open_player);
            else
                open_card();
            break;

        case 4:
            if(!m_bet)
                bet(a_open_player);
            else
                open_card();
            break;

        case 5:
            if(!m_bet)
                bet(a_open_player);
            else
                close_card_round();
            break;
        
        default:
            break;
        };
    }
}

void CardRound::bet(playerIterator a_open_player)
{
    m_bet_round.run(a_open_player);
    m_bet = true;
}

void CardRound::deal_cards()
{
    for(int i = 0; i < 2; ++i)
    {
        auto it = m_players.begin();
        auto end = m_players.end();

        while(it != end)
        {
            Card card = m_deck.pop_card();
            m_players.get_card(it->second->m_name, card);
            m_action_out.get_card(it->second->m_name, card);
            ++it;
        }
    }
}

void CardRound::open_three_cards()
{
    for(int i = 0; i < 3; ++i)
        open_card();
}

void CardRound::open_card()
{
    Card card = m_deck.pop_card();
    m_table.get_card(card);
    m_action_out.table_get_card(card);
    m_bet = false;
}

void CardRound::close_card_round()
{
    reveal_cards();
    std::string name = chack_winer();
    std::vector<int> chips = m_table.table_chips();
    m_action_out.get_chips(name, chips);
    m_table.clear_chips();
    m_table.clear_cards();
    m_action_out.clear_hand();
    m_deck.re_fill_decks();
    m_stop = true;
    m_bet = false;
}

void CardRound::reveal_cards()
{
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        if(!it->second->m_fold)
            m_action_out.reveal_cards(it->second->m_socket);
        ++it;
    }
}

std::string CardRound::chack_winer()
{
    return m_players.begin()->second->m_name;
}


}// poker namespace
