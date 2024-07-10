/*****************************************************************************
** Copyright 2022 Unisoc(Shanghai) Technologies Co.,Ltd                      *
** Licensed under the Unisoc General Software License,                       *
** version 1.0 (the License);                                                *
** you may not use this file except in compliance with the License.          *
** You may obtain a copy of the License at                                   *
** https://www.unisoc.com/en_us/license/UNISOC_GENERAL_LICENSE_V1.0-EN_US    *
** Software distributed under the License is distributed on an "AS IS" BASIS,*
** WITHOUT WARRANTIES OF ANY KIND, either express or implied.                *
** See the Unisoc General Software License, version 1.0 for more details.    *
******************************************************************************/

/*****************************************************************************
** File Name:      ual_tele_common.h
** Author:         Jichuan.Zhang
** Description:
*****************************************************************************/
#ifndef _UAL_TELE_COMMON_H_
#define _UAL_TELE_COMMON_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "ual_type.h"
#include "ual_common.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

#define UAL_TELE_RES_SUCCESS 0
//UAL TODO:TEMP  solution

/*---------------------------------------------------------------------------*/
/*                          ENUM AND STRUCT                                */
/*---------------------------------------------------------------------------*/

/* SIM index*/
#if defined MULTI_SIM_SYS_SINGLE
    typedef enum
    {
        SIM_ID_1 = 0,
    //UAL TODO:TEMP  solution: SIM_ID_MAX与tele service的SIM_ID_MAX重复定义，所以先改为UAL_SIM_ID_MAX。
        UAL_SIM_ID_MAX
    } ual_tele_sim_id_e;
#elif defined MULTI_SIM_SYS_DUAL
    typedef enum
    {
        SIM_ID_1 = 0,
        SIM_ID_2,
    //UAL TODO:TEMP  solution: SIM_ID_MAX与tele service的SIM_ID_MAX重复定义，所以先改为UAL_SIM_ID_MAX。
        UAL_SIM_ID_MAX
    } ual_tele_sim_id_e;
#endif

#ifndef SIM_ID_MAX
#define SIM_ID_MAX UAL_SIM_ID_MAX
#endif

/* plmn info*/
typedef struct
{
    uint16 mcc;
    uint16 mnc;
    uint16 mnc_digit_num;
} ual_tele_plmn_info_t;


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif // _UAL_TELE_COMMON_H_

