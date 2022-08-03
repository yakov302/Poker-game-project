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
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        if(it->second.get()->m_fold)
            m_players.turn_off(it->second.get()->m_name, "fold");

        ++it;
    }
}

void CardRound::deal_cards()
{
    m_deck.shuffle();
    for(int i = 0; i < 2; ++i)
    {
        auto it = m_players.begin();
        auto end = m_players.end();

        while(it != end)
        {   
            if(!it->second.get()->m_viewer)
            {
                usleep(500000);
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
    usleep(500000);
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
    std::string name;
    if(!one_player_left())
    {
        reveal_cards();
        name = chack_winner(m_players, m_table.table_cards());
        m_action_out.round_winer(name);
        print_result();
        usleep(5000000);
    }
    else
    {
        name = one_player();
        m_action_out.round_winer(name);
        usleep(1000000);
    }

    pay_to_winner(name);
    table_clear_hand();
    clear_hands();
    clear_actions();
    chack_money();
    turn_off_reveal_cards();
    m_deck.re_fill_decks();

    m_stop = true;
    m_bet = false;
}

void CardRound::table_clear_hand()
{
    while(!m_table.is_hand_empty())
    {
        usleep(100000);
        m_action_out.table_give_card();
        m_table.give_card();
    }
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

void CardRound::pay_to_winner(std::string& a_winner)
{
   while(!m_table.is_wallet_empty())
   {
        std::cout << "***" << a_winner << "***\n";
        int chip = m_table.pop_chip();
        m_action_out.table_give_chip(chip);
        m_action_out.get_chips(a_winner, chip);
        m_players.increase(a_winner, chip);
        usleep(100000);
   } 
}

void CardRound::clear_hands()
{
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        if(!it->second.get()->m_fold)
        {
            for(int i = 0; i < 2; ++i)
            {
                usleep(100000);
                m_action_out.give_card(it->second.get()->m_name);
                m_players.give_card(it->second.get()->m_name);
            }
        }

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

 void CardRound::print_result()
 {
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        if(!it->second.get()->m_fold 
        && !it->second.get()->m_viewer
        && it->second.get()->m_hand.size() > 0)
            m_action_out.print_result(it->second.get()->m_name, it->second.get()->m_result);

        ++it;
    }
}

void CardRound::clear_actions()
{
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        if(!it->second.get()->m_viewer)
        {
            m_action_out.clear_action(it->second.get()->m_name);
            it->second.get()->m_result = 0;
        }

        ++it;
    }
    m_action_out.clear_text();
}

}// poker namespace
