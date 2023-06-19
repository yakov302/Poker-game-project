#include "cards_container.hpp"

namespace poker
{

namespace impl
{

void make_cards(std::unordered_map<std::string, CardsMap>& a_cards, std::string& a_line)
{
    int begine = 0;
    int end = a_line.find('|', begine);
    std::string suit = a_line.substr(begine, end-begine);

    begine = end + 1;
    end = a_line.find('|', begine);
    int number = stoi(a_line.substr(begine, end-begine));

    std::string image_path = "./resources/images/cards/" + suit + "-" + std::to_string(number) + ".png";
    a_cards[suit][number] = cardPointer(new Card(suit, number, image_path));
}

void load_cards(std::unordered_map<std::string, CardsMap>& a_cards)
{
    std::string line;
    std::ifstream config_file(CARDS_CONFIG_FILE_PATH);
    while (std::getline(config_file, line))
        make_cards(a_cards, line);
}

void reserve_maps(std::unordered_map<std::string, CardsMap>& a_cards)
{
    a_cards.reserve(4);
    for(auto& map : a_cards)
        map.second.reserve(13);
}


}// impl namespace

CardsContainer::CardsContainer()
: m_cards()
, m_null(new Card("none", 0, BACK_CARD_IMAGE_PATH))
{
    impl::reserve_maps(m_cards);
    impl::load_cards(m_cards);
}

cardPointer CardsContainer::give_card(std::string& a_suit, int a_number)
{
    if(m_cards[a_suit].find(a_number) == m_cards[a_suit].end()) [[unlikely]]
        return m_null;

   return m_cards[a_suit][a_number];
}


}// poker namespace
