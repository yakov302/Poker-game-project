#include "chips_container.hpp"

namespace poker
{

namespace impl
{

void make_chips(std::unordered_map<int, std::vector<chipPointer>>& a_chips, int a_number, std::unordered_map<int, std::stack<int>>& a_availables_stack, int a_amount)
{
    int index_id;
    std::string image_path = "./resources/images/chips/" + std::to_string(a_number) + ".png";

    for(int i = 0; i < a_amount; ++i)
    {
        index_id = a_chips[a_number].size();
        a_chips[a_number].emplace_back(chipPointer(new Chip(a_number, image_path, index_id)));
        a_availables_stack[a_number].push(index_id);
    }
}

void reserve_vectors(std::unordered_map<int, std::vector<chipPointer>>& a_chips, int a_num_of_chips, int a_amount_from_each_chip)
{
    a_chips.reserve(a_num_of_chips);
    for(auto& vector : a_chips)
        vector.second.reserve(a_amount_from_each_chip);
}

void load_chips(std::unordered_map<int, std::vector<chipPointer>>& a_chips, std::unordered_map<int, std::stack<int>>& a_availables_stack)
{
    std::string line;
    std::ifstream config_file(CHIPS_CONFIG_FILE_PATH);

    std::getline(config_file, line);
    int num_of_chips = stoi(line);
    reserve_vectors(a_chips, num_of_chips, AMOUNT_OF_EACH_CHIP);

    while (std::getline(config_file, line))
        make_chips(a_chips, stoi(line), a_availables_stack, AMOUNT_OF_EACH_CHIP);
}


}// impl namespace

ChipsContainer::ChipsContainer()
: m_chips()
, m_availables_stack()
{
    impl::load_chips(m_chips, m_availables_stack);
}

chipPointer ChipsContainer::give_chip(int a_number)
{
    if(m_availables_stack[a_number].empty()) [[unlikely]]
    {
        impl::make_chips(m_chips, a_number, m_availables_stack, 1);
        std::cout << "ERROR: ChipsContainer generates a chip at runtime. check ChipsContainer settings!";
    }

    int index_id = m_availables_stack[a_number].top();
    m_availables_stack[a_number].pop();
    return m_chips[a_number][index_id];
}

void ChipsContainer::release_chip(int a_number, int index_id)
{
    m_availables_stack[a_number].push(index_id);
}


}// poker namespace
