#include"deck.hpp"

namespace poker
{

namespace impl
{

void make_card(std::vector<cardPointer>& a_cards, std::string& a_line)
{
    int begine = 0;
    int end = a_line.find('|', begine);
    std::string suit = a_line.substr(begine, end-begine);

    begine = end + 1;
    end = a_line.find('|', begine);
    int number = stoi(a_line.substr(begine, end-begine));

    a_cards.emplace_back(cardPointer(new Card(suit, number)));
}

void load_card(std::vector<cardPointer> &m_cards)
{
    std::ifstream loadFile("./resources/deck.txt");
    std::string line;
    while (std::getline(loadFile, line))
    {
        make_card(m_cards, line);
    }
}


}//namespace impl

Deck::Deck(int a_number_of_decks)
: m_cards()
{
    m_cards.reserve(52*a_number_of_decks);
    for(int i = 0; i < a_number_of_decks; ++i)
    {
        impl::load_card(m_cards);
    }
}

void Deck::shuffle()
{
    if(!m_cards.empty())
    {
        time_t t;
        srand((unsigned) time(&t));
        const int size = m_cards.size();
        for(int i = 0; i < size; ++i)
        {
            int index1 = rand()%size;
            int index2 = rand()%size;
            std::swap(m_cards[index1], m_cards[index2]);
        }
    }
}

Card Deck::pop_card()
{
    if(!m_cards.empty())
    {
        cardPointer card = m_cards[m_cards.size()-1];
        m_cards.pop_back();
        m_removed_cards.emplace_back(card);
        return *card;
    }
    return Card("", 0);
}

void Deck::re_fill_decks()
{
    if(!m_removed_cards.empty())
    {
        for (auto& e : m_removed_cards)
        {
            m_cards.emplace_back(e);
        }
        m_removed_cards.clear();
    }
}

bool Deck::empty()const
{
    return m_cards.empty();
}


}// poker namespace
