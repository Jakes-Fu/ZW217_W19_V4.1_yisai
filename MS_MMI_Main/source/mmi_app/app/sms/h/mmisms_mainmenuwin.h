/*************************************************************************
 ** File Name:      mmisms_receive.h                                      *
 ** Author:         liming.deng                                          *
 ** Date:           2007/8/23                                            *
 ** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about receiving sms    *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2012/07/12      liming.deng      Create.                              *
*************************************************************************/
#ifndef _MMISMS_MAINMENUWIN_H_
#define _MMISMS_MAINMENUWIN_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmk_app.h"
#include "mmi_default.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif



typedef struct 
{
    MMI_WIN_ID_T            win_id;
    uint8                  menu_win_timer_id;   
}MMISMS_MAINMENU_WINDATA_T;

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : open the SMS delete query window
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_EnterSMSMainMenuWin(void);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
