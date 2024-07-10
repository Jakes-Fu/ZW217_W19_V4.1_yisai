/******************************************************************************
** File Name:      sim_sms.h                                                 *
** Author:         Raislin.Kong                                              *
** Date:           2002.2                                                    *
** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
**                 All Rights Reserved.                                      *
**                 This software is supplied under the terms of a license    *
**                 agreement or non-disclosure agreement with Spreadtrum.    *
**                 Passing on and copying of this document,and communication *
**                 of its contents is not permitted without prior written    *
**                 authorization.                                            *
** Description:    This is the header file that define the signals related   *
**                 with short message                                        *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** --------------------------------------------------------------------------*
** Date           Name             Description                               *
**---------------------------------------------------------------------------*
** 2002.2         Raislin.Kong     Create                                    *
******************************************************************************/

#ifndef _SIM_SMS_H_
#define _SIM_SMS_H_
#ifdef   __cplusplus
extern   "C"
{
#endif

typedef struct
{
    //uint8 sim_sms_num;        //the SMS record num in the EF_SMS in the SIM card
    //uint8 me_sms_num;         //the max SMS num that supported by the mobile
    uint8   valide_sms_num;     //the valide SMS num
    BOOLEAN    *states_got_ptr;
    MN_SMS_STATUS_E *states_ptr;
} SIM_SMS_SETTING_T;
#ifdef   __cplusplus
}
#endif
#endif  //_SIM_SMS_H_
