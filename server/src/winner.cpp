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

/*---------------------------------------------set results---------------------------------------------*/

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
            if(dbg[WINNER])[[unlikely]]
                std::cout << __func__ << "(): players.set(" << a_name << ", " << result << ", ROYAL_STRAIGHT_FLUSH)" << std::endl;
            
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
            if(dbg[WINNER])[[unlikely]]
                std::cout << __func__ << "(): players.set(" << a_name << ", " << result << ", STRAIGHT_FLUSH)" << std::endl;

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
            if(dbg[WINNER])[[unlikely]]
                std::cout << __func__ << "(): players.set(" << a_name << ", " << result << ", FOR_OF_A_KING)" << std::endl;
            
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
        if(dbg[WINNER])[[unlikely]]
            std::cout << __func__ << "(): players.set(" << a_name << ", " << result << ", FULL_HOUSE)" << std::endl;

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
            if(dbg[WINNER])[[unlikely]]
                std::cout << __func__ << "(): players.set(" << a_name << ", " << result << ", FLUSH)" << std::endl;

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
            if(dbg[WINNER])[[unlikely]]
                std::cout << __func__ << "(): players.set(" << a_name << ", " << result << ", STRAIGHT)" << std::endl;

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
            if(dbg[WINNER])[[unlikely]]
                std::cout << __func__ << "(): players.set(" << a_name << ", " << result << ", THREE_OF_A_KING)" << std::endl;

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
        if(dbg[WINNER])[[unlikely]]
            std::cout << __func__ << "(): players.set(" << a_name << ", " << result << ", ONE_PAIR)" << std::endl;

        a_players.set(a_name, result, ONE_PAIR);
        return true;
    }

    if(num_of_pair >= 2)
    {
        if(dbg[WINNER])[[unlikely]]
            std::cout << __func__ << "(): players.set(" << a_name << ", " << result << ", TWO_PAIR)" << std::endl;

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

    if(dbg[WINNER])[[unlikely]]
        std::cout << __func__ << "(): players.set(" << a_name << ", " << result << ", HIGH_CARD)" << std::endl;

    a_players.set(a_name, result, HIGH_CARD);
}

/*---------------------------------------------handling multiple winners---------------------------------------------*/

//-----multiple uses:

void print_winners(std::vector<std::string>& a_winners)
{
    std::cout << "winners: ";
    for(auto winner : a_winners)
        std::cout << winner << ", ";
    std::cout << std::endl;
}

void check_if_value_greater_or_equal_to_max(const char* func, int& a_current_value, int& a_max_value, std::string& a_winner_name, std::string value_name, std::vector<std::string>& a_winners)
{
    if(a_current_value > a_max_value)
    {
        if(dbg[WINNER])[[unlikely]]
            std::cout << func << "(): " << a_winner_name << " has "<< value_name << " of(" << a_current_value << ") > max "<< value_name << "(" <<  a_max_value << ") -> erases previous winners and set " << a_winner_name << " as winner" << std::endl;

        a_max_value = a_current_value; 
        a_winners.clear();
        a_winners.emplace_back(a_winner_name);
    }
    else if(a_current_value == a_max_value)
    {
        if(dbg[WINNER])[[unlikely]]
            std::cout << func << "(): " << a_winner_name << " has "<< value_name << " of(" << a_current_value << ") == max "<< value_name << "(" <<  a_max_value << ") -> set " << a_winner_name << " as winner" << std::endl;
        
        a_winners.emplace_back(a_winner_name);
    }
}

void choose_small_or_big(bool small_or_big, int& a_destination, int a_first, int a_second)
{
    if(small_or_big == SMALL)
    {
        if(a_first <= a_second)
            a_destination = a_first;
        else   
            a_destination = a_second;
    }

    if(small_or_big == BIG)
    {
        if(a_first >= a_second)
            a_destination = a_first;
        else   
            a_destination = a_second;
        
    }
}

void choose_not_equal_to(int& a_destination, int a_first, int a_second, int not_equal_to)
{        
    if(a_first != not_equal_to)
        a_destination = a_first;
    else
        a_destination = a_second;
}

int find_pair_value(std::vector<cardPointer>& a_cards, int a_index)
{
    int pair_index = 0;
    int size = a_cards.size();
    for(int i = size - 1; i > 0; --i)
    {
        if(a_cards[i].get()->m_number == a_cards[i - 1].get()->m_number)
        {
            ++pair_index;
            if(pair_index == a_index)
                return a_cards[i].get()->m_number;
        }
    }

    return 0;
}

int find_tree_value(std::vector<cardPointer>& a_cards, int a_index)
{
    bool find_pair = false;
    int size = a_cards.size();
    for(int i = size - 1; i > 0; --i)
    {
        if(a_cards[i].get()->m_number == a_cards[i - 1].get()->m_number)
        {
            if(find_pair)
                return a_cards[i].get()->m_number;
            find_pair = true;
        }
        else
           find_pair = false; 
    }

    return 0;
}

void compare_results(const char* func, std::vector<std::string>& a_winners, CardsMap& a_cards_map, std::string results_str, int (*results_value)(std::vector<cardPointer>& cards, int index), int a_index)
{
    int max = 1;
    int pair_val;
    std::vector<std::string> new_winners;

    for(auto winner : a_winners)
    {
        pair_val = results_value(a_cards_map[winner], a_index);
        check_if_value_greater_or_equal_to_max(func, pair_val, max, winner, results_str, new_winners);
    }

    a_winners = new_winners;

    if(dbg[WINNER])[[unlikely]]
        print_winners(a_winners);
}

//-----high_card:

void compare_by_second_card(std::vector<std::string>& a_winners, PlayersContainer& a_players, int a_high_card)
{
    int max = 1;
    int low_card;
    std::vector<std::string> new_winners;
    std::pair<cardPointer, cardPointer> cards;

    for(auto winner : a_winners)
    {
        cards = a_players.show_cards(winner);
        choose_not_equal_to(low_card, cards.first->m_number, cards.second->m_number, a_high_card);
        check_if_value_greater_or_equal_to_max(__func__, low_card, max, winner, "low card", new_winners);
    }

    a_winners = new_winners;

    if(dbg[WINNER])[[unlikely]]
        print_winners(a_winners);
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
            check_if_value_greater_or_equal_to_max(__func__, cards.first->m_number, high_card, name, "high card", a_winners);
            check_if_value_greater_or_equal_to_max(__func__, cards.second->m_number, high_card, name, "high card", a_winners);
        }
    }

    if(dbg[WINNER])[[unlikely]]
        print_winners(a_winners);

    if(a_winners.size() > 1)
        compare_by_second_card(a_winners, a_players, high_card);
}

//-----one_pair:

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
        pair_val = find_pair_value(a_cards_map[winner], 1);
        choose_not_equal_to(card_val, cards.first->m_number, cards.second->m_number, pair_val);
        check_if_value_greater_or_equal_to_max(__func__, card_val, max, winner, "card", new_winners);
    }

    a_winners = new_winners;

    if(dbg[WINNER])[[unlikely]]
        print_winners(a_winners);
}

void handle_multiple_one_pair(std::vector<std::string>& a_winners, PlayersContainer& a_players, CardsMap& a_cards_map)
{
    compare_results(__func__, a_winners, a_cards_map, "pair", find_pair_value, 1);
    if(a_winners.size() > 1)
        find_high_card_multiple_one_pair_case(a_winners, a_players, a_cards_map);
}

//-----two_pair:

void find_high_card_multiple_two_pair_case(std::vector<std::string>& a_winners, PlayersContainer& a_players, CardsMap& a_cards_map)
{
    int max = 1;
    int card_val;
    int first_pair_val;
    int second_pair_val;
    std::vector<std::string> new_winners;
    std::pair<cardPointer, cardPointer> cards;

    for(auto winner : a_winners)
    {
        cards = a_players.show_cards(winner);
        first_pair_val = find_pair_value(a_cards_map[winner], 1);
        second_pair_val = find_pair_value(a_cards_map[winner], 2);
        
        if((cards.first->m_number != first_pair_val) && 
           (cards.first->m_number != second_pair_val))
            card_val = cards.first->m_number;
        else if((cards.second->m_number != first_pair_val) && 
                (cards.second->m_number != second_pair_val))
            card_val = cards.second->m_number;
        else
            choose_small_or_big(BIG, card_val, cards.first->m_number, cards.second->m_number);

        check_if_value_greater_or_equal_to_max(__func__, card_val, max, winner, "card", new_winners);
    }

    a_winners = new_winners;

    if(dbg[WINNER])[[unlikely]]
        print_winners(a_winners);
}

void handle_multiple_two_pair(std::vector<std::string>& a_winners, PlayersContainer& a_players, CardsMap& a_cards_map)
{
    compare_results(__func__,  a_winners, a_cards_map, "pair", find_pair_value, 1);
    if(a_winners.size() > 1)
        compare_results(__func__,  a_winners, a_cards_map, "low pair", find_pair_value, 2);
    if(a_winners.size() > 1)
        find_high_card_multiple_two_pair_case(a_winners, a_players, a_cards_map);
}

//-----three:

void handle_multiple_three(std::vector<std::string>& a_winners, PlayersContainer& a_players, CardsMap& a_cards_map)
{
    compare_results(__func__, a_winners, a_cards_map, "three", find_tree_value, 1);
    // if(a_winners.size() > 1)
    //     find_high_card_multiple_three_case(a_winners, a_players, a_cards_map); //make jenral with one card
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
            handle_multiple_two_pair(a_winners, a_players, a_cards_map);
            break;

        case THREE_OF_A_KING:
            handle_multiple_three(a_winners, a_players, a_cards_map);
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
            check_if_value_greater_or_equal_to_max(__func__, player_result, max_result, name, "result", a_winners);
        }
    }

    if(dbg[WINNER])[[unlikely]]
        print_winners(a_winners);

    if(a_winners.size() > 1)
        handle_multiple_winners(a_winners, a_players, a_cards_map);
}

void print_cards(std::vector<cardPointer>& a_cards)
{
    std::cout << "cards: ";
    for(auto card : a_cards)
        std::cout << "(" << card.get()->m_number << "-" << card.get()->m_suit << "), ";
    std::cout << std::endl;
}


void combine_player_cards_with_table_cards(PlayersContainer& a_players, std::string& a_name, std::vector<cardPointer>& a_table_card, std::vector<cardPointer>& a_cards)
{
    a_cards.reserve(7);
    a_cards = a_table_card;
    std::pair<cardPointer, cardPointer> cards = a_players.show_cards(a_name);
    a_cards.emplace_back(cards.first);
    a_cards.emplace_back(cards.second);
    std::sort(a_cards.begin(), a_cards.end(), compare);

    if(dbg[WINNER])[[unlikely]]
        print_cards(a_cards);
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
            if(dbg[WINNER])[[unlikely]]
                std::cout << __func__ << "(): combine cards & set result for " << name << std::endl;

            impl::combine_player_cards_with_table_cards(a_players, name, a_table_card, cards_map[name]);
            impl::set_result(a_players, name, cards_map[name]);
        }
       
    }

    impl::find_winner(a_winners, a_players, cards_map);
}


}//namespace poker
