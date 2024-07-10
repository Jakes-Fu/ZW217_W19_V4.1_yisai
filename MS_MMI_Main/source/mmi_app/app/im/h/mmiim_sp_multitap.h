/*==============================================================================
File Name: mmiim_sp_multitap.h
Author: Lianxiang.Zhou
Date: 2010/06/03
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/06/03   Lianxiang.Zhou  Create.
==============================================================================*/
#ifndef _MMIIM_SP_MULTITAP_H_
#define _MMIIM_SP_MULTITAP_H_



/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiim_sp_tp_basic.h"
#include "mmiim_sp_ml_data.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIIM_SP_MT_KEY_PRESSED (0x0100)
#define MMIIM_SP_MT_KEY_RELEASED (0x0200)
#define MMIIM_SP_MT_KEY_LONG_PRESSED (0x0300)
#define MMIIM_SP_MT_KEY_LONG_RELEASED (0x0400)
#define MMIIM_SP_MT_KEY_REPEATED (0x0500)
#define MMIIM_SP_MT_KEY_TYPE_MASK (0xff00)
#define MMIIM_SP_MT_KEY_MASK (0x00ff)
#define MMIIM_SP_MT_KEY_INVALID (0xffff)

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//ui data
typedef struct
{
    CAF_HANDLE_T                    ctrl_handle;
    GUIIM_INIT_KERNEL_T             init_param;

    GUI_RECT_T                      rect;

    MMIIM_SP_ML_DATA_T              language_data;

    MMIIM_SP_KEY_INDEX_T            last_key;
    uint16                          last_key_index;
    BOOLEAN                         is_timer_active;

    wchar                           last_char;
}MMIIM_SP_MT_DATA_T;


#endif /*_MMIIM_SP_MULTITAP_H_*/


