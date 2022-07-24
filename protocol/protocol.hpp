#pragma once

#include <string>
#include <vector>
#include <string.h>

//      ***buufer order*** 

//      [toal message size]      (4 bytes)
//      [message type]           (1 bytes)

//      [number of strings]      (1 bytes)

//      [strings 1 size]         (1 bytes)
//      [string 1]
//          .
//          .
//          .
//      [strings n size]         (1 bytes)
//      [string n]

//      [number of ints]         (1 bytes)

//      [int 1]
//          .
//          .
//          .
//      [int n]

namespace poker
{

typedef enum Message_type
{
    //server to client

    BUFFER_NOT_INITIALIZE = 1,
    REGISTRATION_SUCCESS = 2,
    REGISTRATION_DUPLICATE_USER_NAME = 3,
    REGISTRATION_WRONG_GENDER = 4,
    LOG_IN_SUCCESS = 5,
    LOG_IN_WRONG_USER_NAME = 6,
    LOG_IN_WRONG_PASSWORD = 7,
    TURN_ON_FLAG = 8,
    TURN_OFF_FLAG = 9,
    BET_UPDATE = 10,
    CHECK_UPDATE = 11,
    FOLD_UPDATE = 12,
    GET_CARD = 13,
    GET_CHIPS = 14,
    CLEAR_HAND = 15,
    GET_PLAYER = 16,
    DELETE_PLAYER = 17,
    REVEAL_CARDS = 18,
    TABLE_GET_CARD = 19,
    TABLE_GET_CHIP = 20,
    TABLE_CLEAR_HAND = 21,
    TABLE_CLEAR_CHIPS = 22,
    WAKE_UP_CLIENT = 23,

    //client to server

    REGISTRATION_REQUES = 24,
    LOG_IN_REQUEST = 25,
    BET_ACTION = 26,
    FINISH_BET = 27,
    CHECK_ACTION = 28,
    FOLD_ACTION = 29,
    WAKE_UP_SERVER = 30

}Message_type;

class Args
{
public:
    Args(int num_of_str, int num_of_int)
    {
        m_strings.reserve(num_of_str);
        m_ints.reserve(num_of_int);
    };

    std::vector<std::string> m_strings;
    std::vector<int> m_ints;
};

int pack(char* a_buffer, Args& a_arganmats, Message_type a_message);
Message_type unpack(char* a_buffer, Args& a_arganmats);
int message_size(char* a_buffer);
Message_type message_type(char* a_buffer);


}// poker namespace


