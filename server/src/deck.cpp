#include"deck.hpp"

namespace poker
{

extern bool dbg[NUM_OF_DBG_TYPES];

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
    std::string line;
    std::ifstream load_file(DECK_CONFIG_FILE_PATH);
    while (std::getline(load_file, line))
        make_card(m_cards, line);
}


}//namespace impl

Deck::Deck(int a_number_of_decks)
: m_cards()
{
    m_cards.reserve(NUM_OF_CARDS_IN_DECK*a_number_of_decks);
    for(int i = 0; i < a_number_of_decks; ++i)
        impl::load_card(m_cards);
}

void Deck::shuffle()
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

cardPointer Deck::pop_card()
{
    if(m_cards.empty())[[unlikely]]
        return nullptr;

    cardPointer card = m_cards[m_cards.size()-1];
    m_cards.pop_back();
    return card;
}

void Deck::push_card(cardPointer card)
{
    if(card == nullptr)[[unlikely]]
        return;
        
    m_cards.push_back(card);
}

bool Deck::empty()const
{
    return m_cards.empty();
}


}// poker namespace
