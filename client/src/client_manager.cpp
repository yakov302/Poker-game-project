#include "client_manager.hpp"

namespace poker
{

Sound sound;
extern std::string empty;

ClientManager::ClientManager()
: m_cards(HEND_NUM_OF_CARD_FRONT, HEND_NUM_OF_CARD_BACK, HEND_X_POS, HEND_Y_POS, HEND_IMAGE_SCALE, HEND_GAP_BETWEEN_FRONT_CARDS, HEND_GAP_BETWEEN_BACK_CARDS)
, m_chips(WALLET_X_POS, WALLET_Y_POS)
, m_self(empty, DEFAULT_SELF_GENDER, DEFAULT_SELF_AMOUNT)
, m_players()
, m_socket()
, m_tcp(m_socket)
, m_action_out(m_tcp)
, m_table(m_cards, m_chips, m_self , m_players, m_action_out)
, m_action_in(m_cards, m_chips, m_players, m_table, m_self, m_action_out)
, m_router(m_socket, m_action_in, m_tcp, m_table)
{

}


}// poker namespace
