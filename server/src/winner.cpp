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

void compare_by_second_card(std::vector<std::string>& a_winners, PlayersContainer& a_players, int a_high_card)
{
    int max = 1;
    int low_card;
    std::vector<std::string> new_winners;
    std::pair<cardPointer, cardPointer> cards;

    for(auto winner : a_winners)
    {
        cards = a_players.show_cards(winner);
       
        if(cards.first->m_number != a_high_card)
            low_card = cards.first->m_number;
        else
            low_card = cards.second->m_number;

        if(low_card > max)
        {
            max = low_card; 
            new_winners.clear();
            new_winners.emplace_back(winner);
        }
        else if(low_card == max)
            new_winners.emplace_back(winner);
    }

    a_winners = new_winners;
}

void handle_multiple_high_card(std::vector<std::string>& a_winners, PlayersContainer& a_players)
{
    int high_card = 1;
    a_winners.clear();
    std::pair<cardPointer, cardPointer> cards;

    for(auto player : a_players)
    {
        std::string name = player.second.get()->m_name;

        if(impl::active_player_with_card(a_players, name))
        {
            cards = a_players.show_cards(name);
            if(cards.first->m_number > high_card)
            {
                high_card = cards.first->m_number;
                a_winners.clear();
                a_winners.emplace_back(name);
            }
            else if(cards.first->m_number == high_card)
                a_winners.emplace_back(name);

            if(cards.second->m_number > high_card)
            {
                high_card = cards.second->m_number;
                a_winners.clear();
                a_winners.emplace_back(name);
            }
            else if(cards.second->m_number == high_card)
                a_winners.emplace_back(name);
        }
    }

    if(a_winners.size() > 1)
        compare_by_second_card(a_winners, a_players, high_card);
}

int find_pair_value(std::vector<cardPointer>& a_cards)
{
    int size = a_cards.size();
    for(int i = 0; i < size - 1; ++i)
    {
        if(a_cards[i].get()->m_number == a_cards[i + 1].get()->m_number)
            return a_cards[i].get()->m_number;
    }

    return 0;
}
 
void compare_pairs(std::vector<std::string>& a_winners, CardsMap& a_cards_map)
{
    int max = 1;
    int pair_val;
    std::vector<std::string> new_winners;

    for(auto winner : a_winners)
    {
        pair_val = find_pair_value(a_cards_map[winner]);
        if(pair_val > max)
        {
            max = pair_val; 
            new_winners.clear();
            new_winners.emplace_back(winner);
        }
        else if(pair_val == max)
            new_winners.emplace_back(winner);
    }

    a_winners = new_winners;
}

void find_high_card_multiple_one_pair_case(std::vector<std::string>& a_winners, PlayersContainer& a_players, CardsMap& a_cards_map)
{
    int max = 1;
    int pair_val;
    int card_val;
    std::vector<std::string> new_winners;
    std::pair<cardPointer, cardPointer> cards;

    for(auto winner : a_winners)
    {
        cards = a_players.show_cards(winner);
        pair_val = find_pair_value(a_cards_map[winner]);
        
        if(cards.first->m_number != pair_val)
            card_val = cards.first->m_number;
        else
            card_val = cards.second->m_number;

        if(card_val > max)
        {
            max = card_val; 
            new_winners.clear();
            new_winners.emplace_back(winner);
        }
        else if(card_val == max)
            new_winners.emplace_back(winner);
    }

    a_winners = new_winners;
}

void handle_multiple_one_pair(std::vector<std::string>& a_winners, PlayersContainer& a_players, CardsMap& a_cards_map)
{
    compare_pairs(a_winners, a_cards_map);
    if(a_winners.size() > 1)
        find_high_card_multiple_one_pair_case(a_winners, a_players, a_cards_map);
}

void handle_multiple_winners(std::vector<std::string>& a_winners, PlayersContainer& a_players, CardsMap& a_cards_map)
{
    int winners_result = a_players.get(a_winners[0], result);

    switch (winners_result)
    {
        case HIGH_CARD:
            handle_multiple_high_card(a_winners, a_players);
            break;

        case ONE_PAIR:
            handle_multiple_one_pair(a_winners, a_players, a_cards_map);
            break;

        case TWO_PAIR:
            /* code */
            break;

        case THREE_OF_A_KING:
            /* code */
            break;

        case STRAIGHT:
            /* code */
            break;

        case FLUSH:
            /* code */
            break;

        case FULL_HOUSE:
            /* code */
            break;

        case FOR_OF_A_KING:
            /* code */
            break;

        case STRAIGHT_FLUSH:
            /* code */
            break;

        case ROYAL_STRAIGHT_FLUSH:
            /* code */
            break;    

        default:
            break;
    }
}

void find_winner(std::vector<std::string>& a_winners, PlayersContainer& a_players, CardsMap& a_cards_map)
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

    if(a_winners.size() > 1)
        handle_multiple_winners(a_winners, a_players, a_cards_map);
}


}//namespace impl

void chack_winner(std::vector<std::string>& a_winners, PlayersContainer& a_players, std::vector<cardPointer>& a_table_card)
{
    CardsMap cards_map;

    for(auto player : a_players)
    {
        std::string name = player.second.get()->m_name;

        if(impl::active_player_with_card(a_players, name))       
        {
            impl::combine_player_cards_with_table_cards(a_players, name, a_table_card, cards_map[name]);
            impl::set_result(a_players, name, cards_map[name]);
        }
       
    }

    impl::find_winner(a_winners, a_players, cards_map);
}


}//namespace poker
