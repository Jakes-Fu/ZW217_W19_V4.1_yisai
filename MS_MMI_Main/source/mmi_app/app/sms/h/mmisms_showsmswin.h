/*************************************************************************
 ** File Name:      mmisms_showsmswin.h                                      *
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
#ifndef _MMISMS_SHOWSMSWIN_H_
#define _MMISMS_SHOWSMSWIN_H_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmk_app.h"
#ifdef SNS_SUPPORT
#include "mmisns_srv.h"
#endif
#include "mmisms_app.h"
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
#define MMISMS_MEMSTATE_LABEL_MAX_LEN  30
#ifdef DPHONE_SUPPORT 
#define MMISMS_DETAIL_LINE_SPACE    1
#else
#define MMISMS_DETAIL_LEFT_MARGIN    5
#define MMISMS_DETAIL_RIGHT_MARGIN    5
#define MMISMS_DETAIL_LINE_SPACE    5
#endif 
//Open SIMBOX.if multi SIM,call Select SIM API
/*****************************************************************************/
//  Description : to open sim box
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
/*****************************************************************************/
//  Description : entre sim box win
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_EnterShowClass0Win(MMISMS_READ_MSG_T read_msg);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
