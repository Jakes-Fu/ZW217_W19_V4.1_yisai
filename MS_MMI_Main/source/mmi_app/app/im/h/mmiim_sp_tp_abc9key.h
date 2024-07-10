/***************************************************************************
** File Name:      mmiim_sp_tp_abc9key.h                                   *
** Author:         hua.fang                                                *
** Date:           07/06/2010                                              *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/2010        hua.fang         Create                                  *
**																		   *
****************************************************************************/
#ifndef _MMIIM_SP_TP_ABC9KEY_H_
#define _MMIIM_SP_TP_ABC9KEY_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "mmiim_sp_tp_basic.h"
#include "mmiim_sp_ml_data.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#if defined(MMIIM_USE_SPIM_TP)

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
typedef struct mmiim_sp_tp_abc9key_data_tag
{
    CAF_HANDLE_T            ctrl_handle;                        // ctrl handle
    GUIIM_INIT_KERNEL_T     init_param;                         // init param
    GUI_RECT_T              rect;                               // rect of ime

    MMIIM_SP_ML_DATA_T      language_data;                      // language data

    MMIIM_SP_TP_KB_INFO_T   kb;                                 // kb panel

    GUIIM_CAPITAL_MODE_T    cap_mode;                           // cap mode

    wchar                   cand[GUIIM_CUSTOM_CHAR_LIMIT];      // candidate
    uint16                  cand_count;                         // candidate count, and also check whether has candidate
    uint16                  cur_page;                           // indicate which page when candidate count is larger than that in one page

    wchar                   last_char;                          // last committed char
} MMIIM_SP_TP_ABC9KEY_DATA_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif //MMIIM_USE_SPIM_TP
#endif  // _MMIIM_SP_TP_ABC9KEY_H_


