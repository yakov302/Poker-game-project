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
    START_BET = 11,
    BET_UPDATE = 12,
    INVALID_BET_MIN = 13,
    INVALID_BET_MAX = 14,
    CHECK_UPDATE = 15,
    FOLD_UPDATE = 16,
    CLEAR_FOLD = 17,
    GET_CARD = 18,
    GET_CHIPS = 19,
    CLEAR_HAND = 20,
    GET_PLAYER = 21,
    DELETE_PLAYER = 22,
    REVEAL_CARDS = 23,
    TABLE_GET_CARD = 24,
    TABLE_GET_CHIP = 25,
    TABLE_CLEAR_HAND = 26,
    TABLE_CLEAR_CHIPS = 27,
    WAKE_UP_CLIENT = 28,
    CLEAR_ACTION = 29,
    ROUND_WINER = 30,
    GAME_WINER = 31,
    CLEAR_TEXT = 32,
    RESULT = 33,

    //client to server

    REGISTRATION_REQUES = 34,
    LOG_IN_REQUEST = 35,
    BET_ACTION = 36,
    FINISH_BET = 37,
    CHECK_ACTION = 38,
    FOLD_ACTION = 33,
    WAKE_UP_SERVER = 40

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
int num_of_ints(char* a_buffer);



}// poker namespace


