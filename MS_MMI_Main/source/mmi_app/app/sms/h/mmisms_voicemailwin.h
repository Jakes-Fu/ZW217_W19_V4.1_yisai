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
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT

#ifndef _MMISMS_VOICEMAILWIN_H_
#define _MMISMS_VOICEMAILWIN_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmk_app.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
typedef struct 
{
    //MMISMS_SIMBOX_DATA_T        s_simbox_msglist;
    MMI_SMS_VMN_T cur_vmnum_info;
}MMISMS_VOICEMAIL_WINDATA_T;

//Open SIMBOX.if multi SIM,call Select SIM API

/*****************************************************************************/
//  Description : to open sim box
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
//PUBLIC void MMISMS_OpenSIMBOX(void);

#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : open voice mail win
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_EnterShowVmnWin(void);
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif
