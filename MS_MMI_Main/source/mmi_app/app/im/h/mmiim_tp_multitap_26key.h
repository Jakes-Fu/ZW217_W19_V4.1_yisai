/*==============================================================================
File Name: mmiim_tp_multitap_26key.h
Author: Haiwu.Chen
Date: 2010/09/04
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/09/04	 Haiwu.Chen		 Create.
==============================================================================*/
#if (!defined KEYPAD_TYPE_TYPICAL_KEYPAD ||defined  MMI_IM_PDA_SUPPORT)
#ifndef _MMIIM_TP_MULTITAP_26KEY_H_
#define _MMIIM_TP_MULTITAP_26KEY_H_

#if defined(TOUCH_PANEL_SUPPORT)

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/
#include "mmiim_sp_tp_basic.h"
#include "mmiim_sp_ml_data.h"


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIIM_MULTITAP_26KEY_CANDIDATE_TIMER_OUT   800

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/

#ifdef _cplusplus
extern   "C"
{
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
typedef struct
{
    BOOLEAN                 is_timer_active;        // is time active
    uint8                   key_timer_id;           // timer id

    MMIIM_SP_ML_DATA_T      language_data;          // language data

    int16                   last_multitap_press_index;       // 上一次的有效按键，此值是"kb"定义的wchar值
    int16                   last_multitap_char_index;        // 字母索引

} MMIIM_TP_MULTITAP_26KEY_DATA_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
}
#endif

#endif //#if defined(TOUCH_PANEL_SUPPORT)

#endif //_MMIIM_TP_MULTITAP_26KEY_H_
#endif
