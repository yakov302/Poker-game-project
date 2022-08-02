#include "winner.hpp"
#include <iostream>

namespace poker
{

namespace impl
{

void fill_vector(playerIterator& a_player, std::vector<Card>& a_table_card, std::vector<Card>& a_card)
{
    std::cout << a_player->second.get()->m_name << " cards: " << "\n";
    for(size_t i = 0; i < a_player->second.get()->m_hand.size(); ++i)
        std::cout << a_player->second.get()->m_hand[i].m_number << "\n";

    a_card.reserve(7);
    a_card = a_table_card;
    a_card.emplace_back(a_player->second.get()->m_hand[0]);
    a_card.emplace_back(a_player->second.get()->m_hand[1]);
    std::sort(a_card.begin(), a_card.end());
    std::cout << a_player->second.get()->m_name << " cards: " << "\n";
    for(auto c : a_card)
        std::cout << c.m_number <<" \n";
}

bool royal_straight_flush(playerIterator& a_player, std::vector<Card>& a_card)
{
    int size = a_card.size();
    if(a_card[size - 1].m_number != 14)
        return false;

    int count = 1;
    for(int i = size - 1; i > 0; --i)
    {
        if(a_card[i - 1].m_number == 14)
            continue;

        if(a_card[i].m_suit != a_card[i - 1].m_suit
        || a_card[i].m_number - a_card[i - 1].m_number != 1)
            return false;
        else
            ++count;

        if(count == 5)
        {
            a_player->second.get()->m_result = ROYAL_STRAIGHT_FLUSH;
            return true;
        }
    }

    return false;
}

bool straight_flush(playerIterator& a_player, std::vector<Card>& a_card)
{
    int count = 1;
    int size = a_card.size();

    for(int i = 0; i < size - 1; ++i)
    {
        if(a_card[i].m_suit != a_card[i + 1].m_suit
        || a_card[i + 1].m_number - a_card[i].m_number != 1)
        {
            if(i < 2)
                count = 1;
            else
                return false;
        }
        else
            ++count;

        if(count == 5)
        {
            a_player->second.get()->m_result = STRAIGHT_FLUSH;
            return true;
        }
    }
    
    return false;
}

bool four_of_a_kind(playerIterator& a_player, std::vector<Card>& a_card)
{
    int count = 1;
    int size = a_card.size();

    for(int i = 0; i < size - 1; ++i)
    {
        if(a_card[i].m_number == a_card[i + 1].m_number)
            ++count;
        else
        {
            if(i >= 3)
                return false;
            else
                count = 1;
        }

        if(count == 4)
        {
            a_player->second.get()->m_result = FOR_OF_A_KING;
            return true;
        }
    }
    
    return false;
}

bool full_house(playerIterator& a_player, std::vector<Card>& a_card)
{
    bool three = false;
    bool pair = false;
    int count = 1;
    int size = a_card.size();

    for(int i = 0; i < size - 1; ++i)
    {
        if(a_card[i].m_number == a_card[i + 1].m_number)
            ++count;
        else
            count = 1;

        if(count == 2)
        {
            if(i + 2 < size)
            {
                if(a_card[i].m_number != a_card[i + 2].m_number)
                    pair = true;
            }
            else
                pair = true;
        }

        if(count == 3)
            three = true;
    }
    
    if(three && pair)
    {
        a_player->second.get()->m_result = FULL_HOUSE;
        return true;
    }
    return false;
}


bool flush(playerIterator& a_player, std::vector<Card>& a_card)
{
    int count = 1;
    int size = a_card.size();

    for(int i = 0; i < size - 1; ++i)
    {
        if(a_card[i].m_suit == a_card[i + 1].m_suit)
            ++count;
        else
        {
            if(i < 2)
                count = 1;
            else
                return false;
        }

        if(count == 5)
        {
            a_player->second.get()->m_result = FLUSH;
            return true;
        }
    }

    return false;
}


bool straight(playerIterator& a_player, std::vector<Card>& a_card)
{
    int count = 1;
    int size = a_card.size();

    for(int i = 0; i < size - 1; ++i)
    {
        if(a_card[i + 1].m_number - a_card[i].m_number != 1)
        {
            if(i < 2)
                count = 1;
            else
                return false;
        }
        else
            ++count;

        if(count == 5)
        {
            a_player->second.get()->m_result = STRAIGHT;
            return true;
        }
    }
    
    return false;
}

bool three_of_a_kind(playerIterator& a_player, std::vector<Card>& a_card)
{
    int count = 1;
    int size = a_card.size();

    for(int i = 0; i < size - 1; ++i)
    {
        if(a_card[i].m_number == a_card[i + 1].m_number)
            ++count;
        else
        {
            if(i >= 4)
                return false;
            else
                count = 1; 
        }

        if(count == 3)
        {
            a_player->second.get()->m_result = THREE_OF_A_KING;
            return true;
        }   
    }
    
    return false;
}


bool pairs(playerIterator& a_player, std::vector<Card>& a_card)
{
    int count = 1;
    int num_of_pair = 0;
    int size = a_card.size();

    for(int i = 0; i < size - 1; ++i)
    {
        if(a_card[i].m_number == a_card[i + 1].m_number)
            ++count;
        else
            count = 1; 

        if(count == 2)
            ++num_of_pair; 
    }

    if(num_of_pair == 1)
    {
        a_player->second.get()->m_result = ONE_PAIR;
        return true;
    }

    if(num_of_pair == 2)
    {
        a_player->second.get()->m_result = TWO_PAIR;
        return true;
    }

    return false;
}

void set_combination(playerIterator& a_player, std::vector<Card>& a_card)
{
    if(royal_straight_flush(a_player, a_card))
        return;

    if(straight_flush(a_player, a_card))
        return;

    if(four_of_a_kind(a_player, a_card))
        return;

    if(full_house(a_player, a_card))
        return;

    if(flush(a_player, a_card))
        return;

    if(straight(a_player, a_card))
        return;

    if(three_of_a_kind(a_player, a_card))
        return;

    if(pairs(a_player, a_card))
        return;

    a_player->second.get()->m_result = HIGH_CARD;
}

std::string compare_high_card(PlayersContainer& a_players)
{
    int max = 1;
    std::string winner = "";

    auto it = a_players.begin();
    auto end = a_players.end();

    while(it != end)
    {
        if(!it->second.get()->m_fold 
        && !it->second.get()->m_viewer
        && it->second.get()->m_hand.size() > 0)
        {
            if(it->second.get()->m_hand[0].m_number > max)
            {
                max = it->second.get()->m_hand[0].m_number;
                winner = it->second.get()->m_name;
            }
            if(it->second.get()->m_hand[1].m_number > max)
            {
                max = it->second.get()->m_hand[1].m_number;
                winner = it->second.get()->m_name;
            }
        }
        ++it;
    }

    return winner;
}

bool the_same(PlayersContainer& a_players)
{
    int prev = 0;
    bool first = true;
    auto it = a_players.begin();
    auto end = a_players.end();

    while(it != end)
    {
        if(!it->second.get()->m_fold 
        && !it->second.get()->m_viewer
        && it->second.get()->m_hand.size() > 0)
        {
            if(first)
            {
               prev = it->second.get()->m_result; 
               first = false;
            }
            else
            {
                if(it->second.get()->m_result != prev)
                    return false;
                else
                    prev = it->second.get()->m_result; 
            }    
        }
        ++it;
    }
    return true;
}

std::string find_winner(PlayersContainer& a_players)
{
    if(the_same(a_players))
        return compare_high_card(a_players);

    int max = 1;
    std::string winner = "";

    auto it = a_players.begin();
    auto end = a_players.end();

    while(it != end)
    {
        if(!it->second.get()->m_fold 
        && !it->second.get()->m_viewer
        && it->second.get()->m_hand.size() > 0)
        {
            if(it->second.get()->m_result > max)
            {
                max = it->second.get()->m_result;
                winner = it->second.get()->m_name;
            }
        }
        ++it;
    }

    return winner;
}


}//namespace impl

std::string chack_winner(PlayersContainer& a_players, std::vector<Card>& a_table_card)
{
    std::cout << "table cards: \n";
    for(auto c : a_table_card)
        std::cout << c.m_number << "\n";

    auto it = a_players.begin();
    auto end = a_players.end();

    while(it != end)
    {
        if(!it->second.get()->m_fold 
        && !it->second.get()->m_viewer
        && it->second.get()->m_hand.size() > 0)
        {
            std::vector<Card> card;
            impl::fill_vector(it, a_table_card, card);
            impl::set_combination(it, card);
        }
        ++it;
    }
    return impl::find_winner(a_players);
}

}//namespace poker
