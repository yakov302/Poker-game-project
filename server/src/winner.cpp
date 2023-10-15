#include "winner.hpp"

namespace poker
{

extern std::string fold;
extern std::string result;
extern std::string viewer;
extern bool dbg[NUM_OF_DBG_TYPES];

namespace impl
{

extern bool active_player_with_card(PlayersContainer& a_players, std::string& name);

bool compare(cardPointer a, cardPointer b) {return a.get()->m_number < b.get()->m_number;};

void combine_player_cards_with_table_cards(PlayersContainer& a_players, std::string& a_name, std::vector<cardPointer>& a_table_card, std::vector<cardPointer>& a_cards)
{
    a_cards.reserve(7);
    a_cards = a_table_card;
    std::pair<cardPointer, cardPointer> cards = a_players.show_cards(a_name);
    a_cards.emplace_back(cards.first);
    a_cards.emplace_back(cards.second);
    std::sort(a_cards.begin(), a_cards.end(), compare);
}

bool royal_straight_flush(PlayersContainer& a_players, std::string& a_name, std::vector<cardPointer>& a_cards)
{
    int size = a_cards.size();
    if(a_cards[size - 1].get()->m_number != 14)
        return false;

    int count = 1;
    for(int i = size - 1; i > 0; --i)
    {
        if(a_cards[i - 1].get()->m_number == 14)
            continue;

        if(a_cards[i].get()->m_suit != a_cards[i - 1].get()->m_suit
        || a_cards[i].get()->m_number - a_cards[i - 1].get()->m_number != 1)
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

bool straight_flush(PlayersContainer& a_players, std::string& a_name, std::vector<cardPointer>& a_cards)
{
    int count = 1;
    int size = a_cards.size();

    for(int i = 0; i < size - 1; ++i)
    {
        if(a_cards[i].get()->m_suit != a_cards[i + 1].get()->m_suit
        || a_cards[i + 1].get()->m_number - a_cards[i].get()->m_number != 1)
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

bool four_of_a_kind(PlayersContainer& a_players, std::string& a_name, std::vector<cardPointer>& a_cards)
{
    int count = 1;
    int size = a_cards.size();

    for(int i = 0; i < size - 1; ++i)
    {
        if(a_cards[i].get()->m_number == a_cards[i + 1].get()->m_number)
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

bool full_house(PlayersContainer& a_players, std::string& a_name, std::vector<cardPointer>& a_cards)
{
    bool pair = false; 
    bool three = false;

    int count = 1;
    int size = a_cards.size();

    for(int i = 0; i < size - 1; ++i)
    {
        if(a_cards[i].get()->m_number == a_cards[i + 1].get()->m_number)
            ++count;
        else
            count = 1;

        if(count == 2)
        {
            if(i + 2 < size)
            {
                if(a_cards[i].get()->m_number != a_cards[i + 2].get()->m_number)
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


bool flush(PlayersContainer& a_players, std::string& a_name, std::vector<cardPointer>& a_cards)
{
    int count = 1;
    int size = a_cards.size();

    for(int i = 0; i < size - 1; ++i)
    {
        if(a_cards[i].get()->m_suit == a_cards[i + 1].get()->m_suit)
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

bool straight(PlayersContainer& a_players, std::string& a_name, std::vector<cardPointer>& a_cards)
{
    int count = 1;
    int size = a_cards.size();

    for(int i = 0; i < size - 1; ++i)
    {
        if(a_cards[i + 1].get()->m_number - a_cards[i].get()->m_number != 1)
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

bool three_of_a_kind(PlayersContainer& a_players, std::string& a_name, std::vector<cardPointer>& a_cards)
{
    int count = 1;
    int size = a_cards.size();

    for(int i = 0; i < size - 1; ++i)
    {
        if(a_cards[i].get()->m_number == a_cards[i + 1].get()->m_number)
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


bool pairs(PlayersContainer& a_players, std::string& a_name, std::vector<cardPointer>& a_cards)
{
    int count = 1;
    int num_of_pair = 0;
    int size = a_cards.size();

    for(int i = 0; i < size - 1; ++i)
    {
        if(a_cards[i].get()->m_number == a_cards[i + 1].get()->m_number)
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

void set_result(PlayersContainer& a_players, std::string& a_name, std::vector<cardPointer>& a_cards)
{
    if(royal_straight_flush(a_players, a_name, a_cards))
        return;

    if(straight_flush(a_players, a_name, a_cards))
        return;

    if(four_of_a_kind(a_players, a_name, a_cards))
        return;

    if(full_house(a_players, a_name, a_cards))
        return;

    if(flush(a_players, a_name, a_cards))
        return;

    if(straight(a_players, a_name, a_cards))
        return;

    if(three_of_a_kind(a_players, a_name, a_cards))
        return;

    if(pairs(a_players, a_name, a_cards))
        return;

    a_players.set(a_name, result, HIGH_CARD);
}

// std::string compare_high_card(PlayersContainer& a_players)
// {
//     int max = 1;
//     std::string winner = "";
//     std::pair<cardPointer, cardPointer> cards;

//     auto it = a_players.begin();
//     auto end = a_players.end();

//     while(it != end)
//     {
//         std::string name = it->second.get()->m_name;

//         if(impl::active_player_with_card(a_players, name))
//         {
//             cards = a_players.show_cards(name);
//             if(cards.first->m_number > max)
//             {
//                 max = cards.first->m_number;
//                 winner = name;
//             }
//             if(cards.second->m_number > max)
//             {
//                 max = cards.second->m_number;
//                 winner = name;
//             }
//         }
//         ++it;
//     }

//     return winner;
// }

// bool the_same(PlayersContainer& a_players)
// {
//     int prev = 0;
//     bool first = true;
//     auto it = a_players.begin();
//     auto end = a_players.end();

//     while(it != end)
//     {
//         std::string name = it->second.get()->m_name;

//         if(impl::active_player_with_card(a_players, name))
//         {
//             if(first)
//             {
//                prev = a_players.get(name, result);
//                first = false;
//             }
//             else
//             {
//                 if(a_players.get(name, result) != prev)
//                     return false;
//                 else
//                     prev = a_players.get(name, result); 
//             }    
//         }
//         ++it;
//     }

//     return true;
// }

void find_winner(std::vector<std::string>& a_winners, PlayersContainer& a_players)
{
    int max_result = 1;

    for(auto player : a_players)
    {
        std::string name = player.second.get()->m_name;

        if(impl::active_player_with_card(a_players, name))
        {
            int player_result = a_players.get(name, result);
            
            if(player_result > max_result)
            {
                max_result = player_result;
                a_winners.clear();
                a_winners.emplace_back(name);
            }
            else if(player_result == max_result)
                a_winners.emplace_back(name);
        }
    }

    // if(winners.size() > 1)
    //     return function to implement(a_winners)
}


}//namespace impl

void chack_winner(std::vector<std::string>& a_winners, PlayersContainer& a_players, std::vector<cardPointer>& a_table_card)
{
    for(auto player : a_players)
    {
        std::string name = player.second.get()->m_name;

        if(impl::active_player_with_card(a_players, name))       
        {
            std::vector<cardPointer> cards;
            impl::combine_player_cards_with_table_cards(a_players, name, a_table_card, cards);
            impl::set_result(a_players, name, cards);
        }
       
    }

    impl::find_winner(a_winners, a_players);
}


}//namespace poker
