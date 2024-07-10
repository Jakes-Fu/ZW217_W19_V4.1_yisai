/*****************************************************************************
** Copyright 2021-2022<Name of Copyright Owner: zirui.li>                    *
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
** File Name:   audioplayer_demoapp_menutable.h                              *
** Author:      zirui.li                                                     *
** Date:        10/29/2022                                                   *
** Description: This file is used define audioplayer demoapp menutable file  *
******************************************************************************/

#ifndef _AUDIOPLAYER_DEMOAPP_MENUTABLE_H_
#define _AUDIOPLAYER_DEMOAPP_MENUTABLE_H_
/*---------------------------------------------------------------------------*/
/*                          Include Files                                    */
/*---------------------------------------------------------------------------*/
#include "mmimp3_menutable.h"

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

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    ID_AUDIOPLAYER_DEMOAPP_NETWORK_MUSIC = MMIMP3_ID_MENU_MAX,
    ID_AUDIOPLAYER_DEMOAPP_LOCAL_MUSIC
}AUDIOPLAYER_DEMOAPP_MENU_ID_E;
#endif  //_AUDIOPLAYER_DEMOAPP_MENUTABLE_H_
