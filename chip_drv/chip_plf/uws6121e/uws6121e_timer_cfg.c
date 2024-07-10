/*******************************************************************************
 ** File Name:      sc6531efm_timer_cfg.c                                        *
 ** Author:         Ji.Ding                                                    *
 ** Date:           08/20/2010                                                 *
 ** Copyright:      Copyright 2001-xxxx by Spreadtrum Communications,Inc.      *
 *                  All Rights Reserved.                                       *
 **                 This software is supplied under the terms of a license     *
 **                 agreement or non-disclosure agreement with Spreadtrum.     *
 **                 Passing on and copying of this document,and communication  *
 **                 of its contents is not permitted without prior written     *
 **                 authorization.                                             *
 ** Description:    This is the header file of defines signals of the sim      *
 **                 application layer                                          *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** ---------------------------------------------------------------------------*
 ** Date           Name             Description                                *
 **----------------------------------------------------------------------------*
 ** 2018.6.23      Jiajiang.lu           Create                                    *
 ******************************************************************************/

/*******************************************************************************
 **                        Dependencies                                        *
 ******************************************************************************/
#include "sci_types.h"
#include "uws6121e_timer_cfg.h"

/*******************************************************************************
 **                        Compiler Flag                                       *
 ******************************************************************************/
#ifdef   __cplusplus
extern   "C"
{
#endif

/*******************************************************************************
 **                        Mcaro Definitions                                   *
 ******************************************************************************/

/*******************************************************************************
 **                        Structures Definitions                              *
 ******************************************************************************/

/*******************************************************************************
 **                        data prototype                                      *
 ******************************************************************************/
LOCAL TIMER_PHY_STATUS_T timer_phy_status[TIMER_PHY_NUMBER] =
{
    {TIMER_MODE_FREE,   2000000,  32,  0xFFFFFFFF, 2147483,  0, 0, 0},    /*chip timer4*/
    {TIMER_MODE_PERIOD, 16384,    24,  0x00FFFFFF, 1023999,  0, 0, 0},
    {TIMER_MODE_FREE,   16384,    24,  0x00FFFFFF, 1023999,  0, 0, 0},
    {TIMER_MODE_FREE,   16384,    24,  0x00FFFFFF, 1023999,  0, 0, 0},
};

/*******************************************************************************
 **                        Function Definitions                                *
 ******************************************************************************/

/******************************************************************************/
//  Description:    get appointed timer's capability
//  Global resource dependence:
//  Author:         Ji.Ding
//  Note:
//  Return value:   TIMER_PHY_STATUS_T *
/******************************************************************************/
PUBLIC TIMER_PHY_STATUS_T *TIMER_CFG_get_status (uint32 timer_id)
{
    if (timer_id >= TIMER_PHY_NUMBER)
    {
        return NULL;
    }
    else
    {
        return &timer_phy_status[timer_id];
    }
}

/*******************************************************************************
 **                        Compiler Flag                                       *
 ******************************************************************************/
#ifdef   __cplusplus
}
#endif

