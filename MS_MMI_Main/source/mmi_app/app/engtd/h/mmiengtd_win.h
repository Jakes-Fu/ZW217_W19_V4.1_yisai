/****************************************************************************
** File Name:      mmieng_win.h			                                    *
** Author:         allen				                                    *
** Date:           2004.09.10												*
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.		*
** Description:    									               			*
*****************************************************************************
**                         Important Edit History							*
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             	*
** 09/2004      allen               Create
** 
****************************************************************************/

#ifdef ENGTD_SUPPORT
#ifndef  _MMIENGTD_WIN_H_    
#define  _MMIENGTD_WIN_H_   

#include "mmk_app.h"
#include "mmidisplay_data.h"
#include "guicommon.h"
#include "guitext.h"
#include "mn_type.h"

#ifdef __cplusplus
    extern "C"
    {
#endif

#define MMIENG_ASNSELECT_NUM    3
//@maggie for cr58510 add begin
#define MMIENG_ARMLOGOUTPATHSELECT_NUM 3

#define ENG_WIN_MAX_CHAR		18

#define ENG_SHOWRESULT_TIMER_DELAY	6000
#define ENG_TARGET_MAX_LEN		((MN_SMS_ASSERT_DEST_ADDR_LEN_MAX - 1) * 2)

PUBLIC MMI_RESULT_E ENGTDMainMenuWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

#ifdef __cplusplus
    }
#endif

#endif
#endif// ENGTD_SUPPORT