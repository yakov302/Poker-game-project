#include "card_round.hpp"

namespace poker
{

extern std::string fold;
extern std::string socket;
extern std::string amount;
extern std::string result;
extern std::string viewer;
std::string reveal_cards = "reveal_cards";
extern bool dbg[NUM_OF_DBG_TYPES];

namespace impl
{

bool active_player_with_card(PlayersContainer& a_players, std::string& name)
{
    return !a_players.is_flag_on(name, fold)
        && !a_players.is_flag_on(name, viewer)
        && a_players.is_it_has_a_cards(name);
}


}//impl namespace

CardRound::CardRound(PlayersContainer& a_players, Table& a_table, ActionOut& a_action_out, BetRound& a_bet_round, Deck& a_deck)
: m_bet(false)
, m_stop(false)
, m_deck(a_deck)
, m_table(a_table)
, m_bet_round(a_bet_round)
, m_action_out(a_action_out)
, m_players (a_players)
{

}

void CardRound::run()
{
    std::vector<std::string> name;
    m_stop = false;
    deal_cards();

    while(!m_stop)
    {
        if(one_player_left(name)){stop(name); break;}

        int num_of_cards = m_table.num_of_card();

        switch (num_of_cards)
        {
        case 0:
            if(!m_bet)
                bet();
            else
                open_three_cards();
            break;

        case 3:
            if(!m_bet)
                bet();
            else
                open_card();
            break;

        case 4:
            if(!m_bet)
                bet();
            else
                open_card();
            break;

        case 5:
            if(!m_bet)
                bet();
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

            if(!m_players.is_flag_on(name, viewer))
            {
                usleep(500000);
                cardPointer card = m_deck.pop_card();
                m_players.get_card(name, card);
                m_action_out.get_card(name, card);
            }
            ++it;
        }
    }
}

void CardRound::bet()
{
    m_bet_round.run();
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
    cardPointer card = m_deck.pop_card();
    m_table.get_card(card);
    m_action_out.table_get_card(card);
    m_bet = false;
}

bool CardRound::one_player_left(std::vector<std::string>& a_names)
{
    int count = 0;
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        std::string name =  it->second.get()->m_name;

        if(impl::active_player_with_card(m_players, name))
        {
            a_names.emplace_back(name);
            ++count;
        }
        
        if(count > 1)
        {
            for(int i = 0; i < count; ++i)
                a_names.pop_back();
            return false;
        }

        ++it;
    }

    return true; 
}

void CardRound::stop(std::vector<std::string>& names)
{
    m_action_out.round_winer(names); 

    usleep(1000000);
    pay_to_winner(names);
    table_clear_hand();
    clear_hands();
    reset_players_variables();

    m_stop = true;
    m_bet = false;
}

void CardRound::close_card_round()
{
    std::vector<std::string> winners;
    winners.reserve(m_players.num_of_players());

    if(!one_player_left(winners))
    {
        chack_winner(winners, m_players, m_table.table_cards());
        reveal_cards_and_print_result();
        usleep(4000000);
    }

    stop(winners);
}

void CardRound::reveal_cards_and_print_result()
{
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        std::string name =  it->second.get()->m_name;

        if(impl::active_player_with_card(m_players, name))
        {
            m_action_out.reveal_cards(name);
            m_action_out.print_result(name, m_players.get(name, result));
        }

        ++it;
    }
}

void CardRound::pay_to_winner(std::vector<std::string>& a_winners)
{
    int player_amount = 0;
    int player_part = m_table.total_chips_amount()/a_winners.size();

    if(dbg[CARD_ROUND])[[unlikely]]
        std::cout << __func__ << "(): total chips in table is: " << m_table.total_chips_amount() << " num of winers is: " <<  a_winners.size() << " each winner's share is: " <<  player_part << std::endl;
    
    for(auto winner : a_winners)
    {  
        if(dbg[CARD_ROUND])[[unlikely]]
            std::cout << __func__ << "(): start pay to : " << winner << std::endl;   
        
        while(player_amount < player_part)
        {
            if(dbg[CARD_ROUND])[[unlikely]]
                std::cout << __func__ << "(): call m_table.give_chips(chips, (" << player_part - player_amount << ")" << std::endl;
        
            std::vector<int> chips;
            if(!m_table.give_chips(chips, (player_part - player_amount)))
                break;

            for(auto chip : chips)
            {                
                usleep(100000);
                player_amount += chip;
                m_action_out.table_give_chip(chip);
                m_action_out.get_chips(winner, chip);
                m_players.take_chip(winner, chip);

                if(dbg[CARD_ROUND])[[unlikely]]
                    std::cout << __func__ << "(): winner get: " << chip << " total: " << player_amount << std::endl;
            }
        }

        player_amount = 0;
    }

    if(dbg[CARD_ROUND])[[unlikely]]
        std::cout << __func__ << "(): call m_action_out.table_clear_wallet() " << std::endl;
    
    m_action_out.table_clear_wallet(); //Unnecessary?
}

void CardRound::table_clear_hand()
{
    while(!m_table.is_hand_empty())
    {
        usleep(100000);
        m_action_out.table_give_card();
        m_deck.push_card(m_table.give_card());
    }
}

void CardRound::clear_hands()
{
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        std::string name =  it->second.get()->m_name;

        if(!m_players.is_flag_on(name, fold)
        &&  m_players.is_it_has_a_cards(name))
        {
            for(int i = 0; i < 2; ++i)
            {
                usleep(100000);
                m_action_out.give_card(name);
                m_deck.push_card(m_players.give_card(name));
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

        if(!m_players.is_flag_on(name, viewer))
        {
            m_action_out.turn_off(name, reveal_cards);
            m_action_out.clear_action(name);
            m_players.turn_off(name, fold);
            m_players.set(name, result, 0);

            if(m_players.get(name, amount) == 0)
                m_players.turn_on(name, viewer);
        }

        ++it;
    }

    if(m_players.num_of_players() > 1)   
        m_action_out.clear_text();
    else if(m_players.num_of_players() > 0)
        m_action_out.table_is_empty(m_players.begin()->second.get()->m_vars[socket]);
}


}// poker namespace
