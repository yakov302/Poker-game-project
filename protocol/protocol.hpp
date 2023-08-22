#pragma once

#include <string>
#include <vector>
#include <cstring>

/*
    ***buufer order*** 

    [toal message size]      (4 bytes)
    [message type]           (1 bytes)

    [number of strings]      (1 bytes)

    [strings 1 size]         (1 bytes)
    [string 1]
        .
        .
        .
    [strings n size]         (1 bytes)
    [string n]

    [number of ints]         (1 bytes)

    [int 1]
        .
        .
        .
    [int n]
*/

namespace poker
{

typedef enum Message_type
{
    //server to client

    BUFFER_NOT_INITIALIZE            = 1,
    REGISTRATION_SUCCESS             = 2,
    REGISTRATION_DUPLICATE_USER_NAME = 3,
    REGISTRATION_WRONG_GENDER        = 4,
    LOG_IN_SUCCESS                   = 5,
    LOG_IN_WRONG_USER_NAME           = 6,
    LOG_IN_WRONG_PASSWORD            = 7,
    USER_NAME_ALREADY_LOG            = 8,
    PLAY_SUCCESS                     = 9,
    TURN_ON_FLAG                     = 11,
    TURN_OFF_FLAG                    = 12,
    START_BET_UPDATE                 = 13,
    BET_UPDATE                       = 14,
    INVALID_BET_MIN                  = 15,
    INVALID_BET_MAX                  = 16,
    CHECK_UPDATE                     = 17,
    FOLD_UPDATE                      = 18,
    GET_CARD                         = 19,
    GET_CHIP                         = 20,
    GIVE_CARD                        = 21,
    GET_PLAYER                       = 22,
    DELETE_PLAYER                    = 23,
    REVEAL_CARDS                     = 24,
    TABLE_GET_CARD                   = 25,
    TABLE_GET_CHIP                   = 26,
    TABLE_GIVE_CARD                  = 27,
    TABLE_GIVE_CHIP                  = 28,
    TABLE_CLEAR_WALLET               = 29,
    WAKE_UP_CLIENT                   = 30,
    CLEAR_ACTION                     = 31,
    ROUND_WINER                      = 32,
    GAME_WINER                       = 33,
    CLEAR_TEXT                       = 34,
    RESULT                           = 35,

    //client to server

    REGISTRATION_REQUES              = 36,
    LOG_IN_REQUEST                   = 37,
    PLAY_REQUEST                     = 38,
    START_BET_ACTION                 = 40,
    BET_ACTION                       = 41,
    FINISH_BET                       = 42,
    CHECK_ACTION                     = 43,
    FOLD_ACTION                      = 44,
    WAKE_UP_SERVER                   = 45,
    CLEAR_SCREEN                     = 46

}Message_type;

typedef enum hand_results
{
    HIGH_CARD            = 1,
    ONE_PAIR             = 2, 
    TWO_PAIR             = 3,
    THREE_OF_A_KING      = 4,
    STRAIGHT             = 5, 
    FLUSH                = 6, 
    FULL_HOUSE           = 7,
    FOR_OF_A_KING        = 8,
    STRAIGHT_FLUSH       = 9,
    ROYAL_STRAIGHT_FLUSH = 10,

}hand_results;

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
