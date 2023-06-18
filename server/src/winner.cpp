#include "winner.hpp"

namespace poker
{

extern std::string fold;
extern std::string result;
extern std::string viewer;

namespace impl
{


bool compare(cardPointer a, cardPointer b) {return a.get()->m_number < b.get()->m_number;};

void fill_vector(PlayersContainer& a_players, std::string& a_name, std::vector<cardPointer>& a_table_card, std::vector<cardPointer>& a_card)
{
    a_card.reserve(7);
    a_card = a_table_card;
    std::pair<cardPointer, cardPointer> cards = a_players.give_cards(a_name);
    a_card.emplace_back(cards.first);
    a_card.emplace_back(cards.second);
    std::sort(a_card.begin(), a_card.end(), compare);
}

bool royal_straight_flush(PlayersContainer& a_players, std::string& a_name, std::vector<cardPointer>& a_card)
{
    int size = a_card.size();
    if(a_card[size - 1].get()->m_number != 14)
        return false;

    int count = 1;
    for(int i = size - 1; i > 0; --i)
    {
        if(a_card[i - 1].get()->m_number == 14)
            continue;

        if(a_card[i].get()->m_suit != a_card[i - 1].get()->m_suit
        || a_card[i].get()->m_number - a_card[i - 1].get()->m_number != 1)
            return false;
        else
            ++count;

        if(count == 5)
        {
            a_players.set(a_name, result, ROYAL_STRAIGHT_FLUSH);
            return true;
        }
    }

    return false;
}

bool straight_flush(PlayersContainer& a_players, std::string& a_name, std::vector<cardPointer>& a_card)
{
    int count = 1;
    int size = a_card.size();

    for(int i = 0; i < size - 1; ++i)
    {
        if(a_card[i].get()->m_suit != a_card[i + 1].get()->m_suit
        || a_card[i + 1].get()->m_number - a_card[i].get()->m_number != 1)
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
            a_players.set(a_name, result, STRAIGHT_FLUSH);
            return true;
        }
    }
    
    return false;
}

bool four_of_a_kind(PlayersContainer& a_players, std::string& a_name, std::vector<cardPointer>& a_card)
{
    int count = 1;
    int size = a_card.size();

    for(int i = 0; i < size - 1; ++i)
    {
        if(a_card[i].get()->m_number == a_card[i + 1].get()->m_number)
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
            a_players.set(a_name, result, FOR_OF_A_KING);
            return true;
        }
    }
    
    return false;
}

bool full_house(PlayersContainer& a_players, std::string& a_name, std::vector<cardPointer>& a_card)
{
    bool pair = false; 
    bool three = false;

    int count = 1;
    int size = a_card.size();

    for(int i = 0; i < size - 1; ++i)
    {
        if(a_card[i].get()->m_number == a_card[i + 1].get()->m_number)
            ++count;
        else
            count = 1;

        if(count == 2)
        {
            if(i + 2 < size)
            {
                if(a_card[i].get()->m_number != a_card[i + 2].get()->m_number)
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
        a_players.set(a_name, result, FULL_HOUSE);
        return true;
    }

    return false;
}


bool flush(PlayersContainer& a_players, std::string& a_name, std::vector<cardPointer>& a_card)
{
    int count = 1;
    int size = a_card.size();

    for(int i = 0; i < size - 1; ++i)
    {
        if(a_card[i].get()->m_suit == a_card[i + 1].get()->m_suit)
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
            a_players.set(a_name, result, FLUSH);
            return true;
        }
    }

    return false;
}


bool straight(PlayersContainer& a_players, std::string& a_name, std::vector<cardPointer>& a_card)
{
    int count = 1;
    int size = a_card.size();

    for(int i = 0; i < size - 1; ++i)
    {
        if(a_card[i + 1].get()->m_number - a_card[i].get()->m_number != 1)
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
            a_players.set(a_name, result, STRAIGHT);
            return true;
        }
    }
    
    return false;
}

bool three_of_a_kind(PlayersContainer& a_players, std::string& a_name, std::vector<cardPointer>& a_card)
{
    int count = 1;
    int size = a_card.size();

    for(int i = 0; i < size - 1; ++i)
    {
        if(a_card[i].get()->m_number == a_card[i + 1].get()->m_number)
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
            a_players.set(a_name, result, THREE_OF_A_KING);
            return true;
        }   
    }
    
    return false;
}


bool pairs(PlayersContainer& a_players, std::string& a_name, std::vector<cardPointer>& a_card)
{
    int count = 1;
    int num_of_pair = 0;
    int size = a_card.size();

    for(int i = 0; i < size - 1; ++i)
    {
        if(a_card[i].get()->m_number == a_card[i + 1].get()->m_number)
            ++count;
        else
            count = 1; 

        if(count == 2)
            ++num_of_pair; 
    }

    if(num_of_pair == 1)
    {
        a_players.set(a_name, result, ONE_PAIR);
        return true;
    }

    if(num_of_pair >= 2)
    {
        a_players.set(a_name, result, TWO_PAIR);
        return true;
    }

    return false;
}

void set_combination(PlayersContainer& a_players, std::string& a_name, std::vector<cardPointer>& a_card)
{
    if(royal_straight_flush(a_players, a_name, a_card))
        return;

    if(straight_flush(a_players, a_name, a_card))
        return;

    if(four_of_a_kind(a_players, a_name, a_card))
        return;

    if(full_house(a_players, a_name, a_card))
        return;

    if(flush(a_players, a_name, a_card))
        return;

    if(straight(a_players, a_name, a_card))
        return;

    if(three_of_a_kind(a_players, a_name, a_card))
        return;

    if(pairs(a_players, a_name, a_card))
        return;

    a_players.set(a_name, result, HIGH_CARD);
}

std::string compare_high_card(PlayersContainer& a_players)
{
    int max = 1;
    std::string winner = "";
    std::pair<cardPointer, cardPointer> cards;

    auto it = a_players.begin();
    auto end = a_players.end();

    while(it != end)
    {
        std::string name = it->second.get()->m_name;

        if(!a_players.is_flag_on(name, fold)
        && !a_players.is_flag_on(name, viewer)
        && a_players.is_it_has_a_cards(name))
        {
            cards = a_players.give_cards(name);
            if(cards.first->m_number > max)
            {
                max = cards.first->m_number;
                winner = name;
            }
            if(cards.second->m_number > max)
            {
                max = cards.second->m_number;
                winner = name;
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
        std::string name = it->second.get()->m_name;

        if(!a_players.is_flag_on(name, fold)
        && !a_players.is_flag_on(name, viewer)
        && a_players.is_it_has_a_cards(name))
        {
            if(first)
            {
               prev = a_players.get(name, result);
               first = false;
            }
            else
            {
                if(a_players.get(name, result) != prev)
                    return false;
                else
                    prev = a_players.get(name, result); 
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
        std::string name = it->second.get()->m_name;

        if(!a_players.is_flag_on(name, fold)
        && !a_players.is_flag_on(name, viewer)
        && a_players.is_it_has_a_cards(name))
        {
            if(a_players.get(name, result) > max)
            {
                max = a_players.get(name, result);
                winner = name;
            }
        }
        ++it;
    }

    return winner;
}


}//namespace impl

std::string chack_winner(PlayersContainer& a_players, std::vector<cardPointer>& a_table_card)
{
    auto it = a_players.begin();
    auto end = a_players.end();

    while(it != end)
    {
        std::string name = it->second.get()->m_name;

        if(!a_players.is_flag_on(name, fold)
        && !a_players.is_flag_on(name, viewer)
        && a_players.is_it_has_a_cards(name))
        {
            std::vector<cardPointer> card;
            impl::fill_vector(a_players, name, a_table_card, card);
            impl::set_combination(a_players, name, card);
        }
        ++it;
    }
    return impl::find_winner(a_players);
}


}//namespace poker
