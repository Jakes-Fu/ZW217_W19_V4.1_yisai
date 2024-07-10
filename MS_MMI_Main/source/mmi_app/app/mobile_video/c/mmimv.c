/*****************************************************************************
** File Name:      mmimv.c                                           *
** Author:                                                                   *
** Date:           05/2010                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2004       		     Creat
******************************************************************************/

#define _MMIMV_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_mobile_video_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmimv_id.h"
#include "mmk_app.h"
#include "sig_code.h"
#include "mmimv_export.h"
#include "mv_timer_api.h"
#include "mmivp_internal.h"
#include "mv_net_api.h"
#include "mv_debug.h"
#include "mv_player_api.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/




/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
extern BOOLEAN g_is_normal_exit;
                                                 
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//global variable declare
MMI_APPLICATION_T			g_mv_app = {0};        


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : the process message function of the mobile video application
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePsAndRefMsg( PWND app_ptr, uint16 msg_id, DPARAM param);


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : the process message function of the mobile video application
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePsAndRefMsg( PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    UNUSED(app_ptr);

    //check the param
    //MV_TRACE_LOW:"[MV]mmimv.c:enter function HandlePsAndRefMsg(), MVIDEO_TIMER = 0x%x msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMV_75_112_2_18_2_39_6_0,(uint8*)"dd", MVIDEO_TIMER, msg_id);

    switch( msg_id )
    {
   case  MVIDEO_EXIT_IND:
        g_is_normal_exit = TRUE;
        MMK_CloseWin(MMIMV_MAIN_WIN_ID);
        break;


    case MVIDEO_CONTACT_OPEN:
        MV_CreateContactDialog();
        break;

    case MVIDEO_OPEN_EDIT_WIN:
        MV_CreateEditWin();
        break;

    case MVIDEO_CREATE_REQ:
        MV_MediaPlayer_Create();
        break;

    case MVIDEO_RELEASE_REQ:
        MV_MediaPlayer_Release();
        break;
        
    case MVIDEO_EXIT_REQ:
        MV_MediaPlayer_Exit();
        break;

    case MVIDEO_PLAY_REQ:
        MV_MediaPlayer_Play();
        break;

    case MVIDEO_PAUSE_REQ:
        MV_MediaPlayer_Pause();
        break;
        
    case MVIDEO_SEEK_REQ:
        MV_MediaPlayer_Seek();
        break;
        
    case MVIDEO_STOP_REQ:
        MV_MediaPlayer_Stop();
        break;
    
    case MVIDEO_SET_PARAM_REQ:
        MV_MediaPlayer_SetPlayWnd();
        break;
        
    case MVIDEO_UPDATE_VOL_REQ:
        MV_MediaPlayer_UpdateVolume();
        break;
     
    case MVIDEO_GET_CUR_TIME_REQ:
        MV_MediaPlayer_GetCurTime();
        break;

    case MVIDEO_PDP_ACTIVE_REQ:
        MV_ActivePdpContext();
        break;

    case MVIDEO_PDP_DEACTIVE_REQ:
        MV_DeactivePdpContext();
        break;
        
    default:
        result = MMI_RESULT_FALSE;      
        break;
    }

    return result;
}

/*****************************************************************************/
// 	Description : mobile video init
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMV_Init(void)
{
    g_mv_app.ProcessMsg = HandlePsAndRefMsg;
    g_mv_app.component_type = CT_APPLICATION;
#ifndef WIN32
    MV_inittask();
#endif    
}

