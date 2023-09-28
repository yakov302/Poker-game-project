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

    BUFFER_NOT_INITIALIZE           ,
    REGISTRATION_SUCCESS            ,
    REGISTRATION_DUPLICATE_USER_NAME,
    REGISTRATION_WRONG_GENDER       ,
    LOG_IN_SUCCESS                  ,
    LOG_IN_WRONG_USER_NAME          ,
    LOG_IN_WRONG_PASSWORD           ,
    USER_NAME_ALREADY_LOG           ,
    PLAY_SUCCESS                    ,
    TABLE_FULL                      ,
    TABLE_EMPTY                     ,
    VIEW_SUCCESS                    ,
    TURN_ON_FLAG                    ,
    TURN_OFF_FLAG                   ,
    START_BET_UPDATE                ,
    BET_UPDATE                      ,
    INVALID_BET_MIN                 ,
    INVALID_BET_MAX                 ,
    CHECK_UPDATE                    ,
    FOLD_UPDATE                     ,
    GET_CARD                        ,
    GET_CHIP                        ,
    GIVE_CARD                       ,
    GET_PLAYER                      ,
    DELETE_PLAYER                   ,
    REVEAL_CARDS                    ,
    TABLE_GET_CARD                  ,
    TABLE_GET_CHIP                  ,
    TABLE_GIVE_CARD                 ,
    TABLE_GIVE_CHIP                 ,
    TABLE_CLEAR_WALLET              ,
    WAKE_UP_CLIENT                  ,
    CLEAR_ACTION                    ,
    ROUND_WINER                     ,
    GAME_WINER                      ,
    CLEAR_TEXT                      ,
    RESULT                          ,
    MOVES_TO_ANOTHER_TABLE          ,

    //client to server

    REGISTRATION_REQUES             ,
    LOG_IN_REQUEST                  ,
    PLAY_REQUEST                    ,
    VIEW_REQUEST                    ,
    CHANGE_TABLE                    ,
    START_BET_ACTION                ,
    BET_ACTION                      ,
    FINISH_BET                      ,
    CHECK_ACTION                    ,
    FOLD_ACTION                     ,
    WAKE_UP_SERVER                  ,
    CLEAR_SCREEN                    

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
