/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/




typedef enum
{
    MCD_CARD_STATE_IDLE    = 0,
    MCD_CARD_STATE_READY   = 1,
    MCD_CARD_STATE_IDENT   = 2,
    MCD_CARD_STATE_STBY    = 3,
    MCD_CARD_STATE_TRAN    = 4,
    MCD_CARD_STATE_DATA    = 5,
    MCD_CARD_STATE_RCV     = 6,
    MCD_CARD_STATE_PRG     = 7,
    MCD_CARD_STATE_DIS     = 8
} MCD_CARD_STATE_T;


typedef union
{
    uint32 reg;
    struct
    {
        uint32                          :3;
        uint32 akeSeqError              :1;
        uint32                          :1;
        uint32 appCmd                   :1;
        uint32                          :2;
        uint32 readyForData             :1;
        MCD_CARD_STATE_T currentState   :4;
        uint32 eraseReset               :1;
        uint32 cardEccDisabled          :1;
        uint32 wpEraseSkip              :1;
        uint32 csdOverwrite             :1;
        uint32                          :2;
        uint32 error                    :1;
        uint32 ccError                  :1;
        uint32 cardEccFailed            :1;
        uint32 illegalCommand           :1;
        uint32 comCrcError              :1;
        uint32 lockUnlockFail           :1;
        uint32 cardIsLocked             :1;
        uint32 wpViolation              :1;
        uint32 eraseParam               :1;
        uint32 eraseSeqError            :1;
        uint32 blockLenError            :1;
        uint32 addressError             :1;
        uint32 outOfRange               :1;
    } fields;
} MCD_CARD_STATUS_T;



