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
    USER_NAME_ALREADY_LOG = 8,
    TURN_ON_FLAG = 9,
    TURN_OFF_FLAG = 10,
    BET_UPDATE = 11,
    INVALID_BET = 80,
    CHECK_UPDATE = 12,
    FOLD_UPDATE = 13,
    GET_CARD = 14,
    GET_CHIPS = 15,
    CLEAR_HAND = 16,
    GET_PLAYER = 17,
    DELETE_PLAYER = 18,
    REVEAL_CARDS = 19,
    TABLE_GET_CARD = 20,
    TABLE_GET_CHIP = 21,
    TABLE_CLEAR_HAND = 22,
    TABLE_CLEAR_CHIPS = 23,
    WAKE_UP_CLIENT = 24,

    //client to server

    REGISTRATION_REQUES = 25,
    LOG_IN_REQUEST = 26,
    BET_ACTION = 27,
    FINISH_BET = 28,
    CHECK_ACTION = 29,
    FOLD_ACTION = 30,
    WAKE_UP_SERVER = 31

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


