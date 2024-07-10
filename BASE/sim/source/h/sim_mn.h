/******************************************************************************
 ** File Name:      sim_mn.h                                                  *
 ** Author:         Raislin.Kong                                              *
 ** Date:           2002.2                                                    *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    The sturcture only use in sim layer and mn layer          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2002.2         Raislin.Kong     Create                                    *
 ******************************************************************************/

#ifndef _SIM_MN_H_
#define _SIM_MN_H_

#include "sim_macro_switch.h"
typedef struct
{
    uint8       id_len;
    uint8       id[SIM_MAX_SMSP_ALPHA_ID_LEN];
} SIM_ALPHA_ID;
#endif  //_SIM_MN_H_
