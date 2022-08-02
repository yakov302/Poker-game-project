#include "card_round.hpp"
#include <iostream>

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
    turn_off_folds();
    deal_cards();

    while(!m_stop)
    {
        int num_of_cards = m_table.num_of_card();
        if(one_player_left())
        {
            close_card_round();
            break;
        }

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

void CardRound::turn_off_folds()
{
    std::cout << "----------------------------> turn_off_folds\n";
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        if(it->second.get()->m_fold)
        {
            m_players.turn_off(it->second.get()->m_name, "fold");
            m_action_out.clear_fold(it->second.get()->m_name);
        }

        ++it;
    }
}

void CardRound::deal_cards()
{
    std::cout << "----------------------------> deal_cards\n";
    m_deck.shuffle();
    for(int i = 0; i < 2; ++i)
    {
        auto it = m_players.begin();
        auto end = m_players.end();

        while(it != end)
        {   
            if(!it->second.get()->m_viewer)
            {
                Card card = m_deck.pop_card();
                m_players.get_card(it->second.get()->m_name, card);
                m_action_out.get_card(it->second.get()->m_name, card);
            }
            ++it;
        }
    }
}

void CardRound::bet(playerIterator a_open_player)
{
    m_bet_round.run(a_open_player);
    m_bet = true;
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

bool CardRound::one_player_left()
{
    int count = 0;
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        if(!it->second.get()->m_fold 
        && !it->second.get()->m_viewer
        && it->second.get()->m_hand.size() > 0)
            ++count;
        
        if(count > 1)
            return false;

        ++it;
    }

    return true; 
}

std::string CardRound::one_player()
{
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        if(!it->second.get()->m_fold 
        && !it->second.get()->m_viewer
        && it->second.get()->m_hand.size() > 0)
            return it->second.get()->m_name;
        
        ++it;
    }
    return "";
}

void CardRound::close_card_round()
{
    if(!one_player_left())
        reveal_cards();

    std::string name = chack_winer();
    m_action_out.round_winer(name);
    usleep(3000000);

    m_action_out.get_chips(name, m_table.table_chips());
    m_action_out.table_clear_chips();
    m_action_out.table_clear_hand();
    m_action_out.clear_text();
    turn_off_reveal_cards();
    clear_hands();

    m_players.increase(name, m_table.table_amount());
    m_table.clear_chips();
    m_table.clear_cards();
    m_deck.re_fill_decks();
    chack_money();

    m_stop = true;
    m_bet = false;
}

void CardRound::reveal_cards()
{
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        if(!it->second.get()->m_fold)
            m_action_out.reveal_cards(it->second.get()->m_name);
        ++it;
    }
    usleep(5000000);
}

void CardRound::turn_off_reveal_cards()
{
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        m_action_out.turn_off(it->second.get()->m_name, "reveal_cards");
        ++it;
    }
}

std::string CardRound::chack_winer()
{
    if(one_player_left())
        return one_player();

    return m_players.begin()->second.get()->m_name;
}

void CardRound::clear_hands()
{
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        if(!it->second.get()->m_fold)
            m_action_out.clear_hand(it->second.get()->m_name);

        ++it;
    }
}

void CardRound::chack_money()
{
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        if(it->second.get()->m_amount == 0)
            it->second.get()->m_viewer = true;

        ++it;
    }
}


}// poker namespace
