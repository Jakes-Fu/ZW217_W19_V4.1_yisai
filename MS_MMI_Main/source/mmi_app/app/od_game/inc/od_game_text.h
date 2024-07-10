/*****************************************************************************
** File Name:      xxx_mmi_text.h                                            *
** Author:                                                                   *
** Date:           07/2010                                                   *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool					              *
******************************************************************************/

#ifndef _OD_GAME_TEXT_H_
#define _OD_GAME_TEXT_H_
#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// ÎÄ×Ö×ÊÔ´ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"
typedef enum
{
    TXT_OD_GAME_TXT_NULL = ( MMI_MODULE_OD_GAME<< 16 )  | MMI_SYS_RES_FLAG,
#include "od_game_mdu_def.h"
    TXT_OD_GAME_TXT_MAX
} OD_GAME_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif