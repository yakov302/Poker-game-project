#include "card_round.hpp"

namespace poker
{

CardRound::CardRound(PlayersContainer& a_players, Table& a_table, ActionOut& a_action_out, BetRound& a_bet_round)
: m_bet(false)
, m_stop(false)
, m_deck(1)
, m_table(a_table)
, m_bet_round(a_bet_round)
, m_action_out(a_action_out)
, m_players (a_players)
{

}

void CardRound::run(playerIterator a_open_player)
{
    std::string name;
    m_stop = false;
    deal_cards();

    while(!m_stop)
    {
        if(one_player_left(name))
        {close_card_round(); break;}

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

void CardRound::deal_cards()
{
    m_deck.shuffle();

    for(int i = 0; i < 2; ++i)
    {
        auto it = m_players.begin();
        auto end = m_players.end();

        while(it != end)
        { 
            std::string name =  it->second.get()->m_name;

            if(!m_players.is_flag_on(name, "viewer"))
            {
                usleep(500000);
                Card card = m_deck.pop_card();
                m_players.get_card(name, card);
                m_action_out.get_card(name, card);
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

bool CardRound::one_player_left(std::string& a_name)
{
    int count = 0;
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        std::string name =  it->second.get()->m_name;

        if(!m_players.is_flag_on(name, "fold")
        && !m_players.is_flag_on(name, "viewer")
        && m_players.is_it_has_a_cards(name))
        {
            a_name = name;
            ++count;
        }
        
        if(count > 1)
            return false;

        ++it;
    }

    return true; 
}

void CardRound::close_card_round()
{
    std::string name;
    if(!one_player_left(name))
    {
        name = chack_winner(m_players, m_table.table_cards());
        reveal_cards_and_print_result();
        usleep(4000000);
    }

    m_action_out.round_winer(name);
    usleep(1000000);

    pay_to_winner(name);
    table_clear_hand();
    clear_hands();
    reset_players_variables();

    m_deck.re_fill_decks();

    m_stop = true;
    m_bet = false;
}

void CardRound::reveal_cards_and_print_result()
{
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        std::string name =  it->second.get()->m_name;

        if(!m_players.is_flag_on(name, "fold")
        && !m_players.is_flag_on(name, "viewer")
        && m_players.is_it_has_a_cards(name))
        {
            m_action_out.reveal_cards(name);
            m_action_out.print_result(name, m_players.result(name));
        }

        ++it;
    }
}

void CardRound::pay_to_winner(std::string& a_winner)
{
   while(!m_table.is_wallet_empty())
   {
        usleep(100000);
        int chip = m_table.give_chip();
        m_action_out.table_give_chip(chip);
        m_action_out.get_chips(a_winner, chip);
        m_players.increase(a_winner, chip);
   } 
   m_action_out.table_clear_wallet();
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

void CardRound::clear_hands()
{
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        std::string name =  it->second.get()->m_name;

        if(!m_players.is_flag_on(name, "fold"))
        {
            for(int i = 0; i < 2; ++i)
            {
                usleep(100000);
                m_action_out.give_card(name);
                m_players.give_card(name);
            }
        }

        ++it;
    }
}

void CardRound::reset_players_variables()
{
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        std::string name =  it->second.get()->m_name;

        if(!m_players.is_flag_on(name, "viewer"))
        {
            m_action_out.turn_off(name, "reveal_cards");
            m_action_out.clear_action(name);
            m_players.turn_off(name, "fold");
            m_players.set_result(name, 0);

            if(m_players.amount(name) == 0)
                m_players.turn_on(name, "viewer");
        }

        ++it;
    }
    
    m_action_out.clear_text();
}


}// poker namespace
