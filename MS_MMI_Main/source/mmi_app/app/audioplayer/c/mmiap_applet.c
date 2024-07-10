/*****************************************************************************
** File Name:               mmiap_applet.c                                   *
** Author:                                                                   *
** Date:           2012/7/19                                                 *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ap control                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME              DESCRIPTION                              *
** 2012/7/19      aoke.hu           Create                                   *
******************************************************************************/

#define _MMIAP_APPLET_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_audioplayer_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#include "mmi_image.h"
#include "mmisrvmgr.h"

#include "mmi_applet_table.h"
#include "guimenu.h"
#include "mmi_modu_main.h"
#include "mmicom_data.h"
#include "guilistbox.h"


//local module
#include "mmimp3_text.h"
#include "mmiap_list.h"
#include "mmiapwin_main.h"
#include "mmimp3_id.h"
#include "mmiap_appmain.h"

#ifdef MMIAP_F_HISTORY_PLAYLIST
#include "mmiap_lastinfo_ctrl.h"
#endif

#ifdef MMIAP_MUSICMARK_SUPPORT
#include "mmiap_musicmark.h"
#endif

#include "mmiap_play.h"
#include "mmiap_list_control.h"


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : the process message function of the MP3
//  Global resource dependence :
//  Author:fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  MP3Applet_HandleEvent(
    IAPPLET_T*          iapplet_ptr,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);

typedef struct
{
    CAF_APPLET_T caf_applet;
    uint32  user_data;
} MMIMP3_APPLET_T;

//applet的数组
LOCAL const CAF_STATIC_APPLET_T s_mmimp3_applet_array[] =
{
    {
        GUID_NAME_DEF(SPRD_MP3_APPLET_ID), sizeof(MMIMP3_APPLET_T), MP3Applet_HandleEvent ,
        IMAGE_SECMENU_ICON_MULTIM_MP3, TXT_AUDIO
    },
};

//applet的信息, 被mmi_applet_table.c extern
LOCAL const CAF_STATIC_APPLET_INFO_T s_mmimp3_applet_info =
{
    (CAF_STATIC_APPLET_T*)s_mmimp3_applet_array,
    ARR_SIZE(s_mmimp3_applet_array)
};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : register MP3 module applet info
//  Global resource dependence :
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_RegAppletInfo(void)
{
    MMI_RegAppletInfo(MMI_MODULE_MP3, &s_mmimp3_applet_info);
}

/*****************************************************************************/
//  Description : the process message function of the MP3
//  Global resource dependence :
//  Author:fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  MP3Applet_HandleEvent(
    IAPPLET_T*          iapplet_ptr,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIMP3_APPLET_T* applet_ptr = (MMIMP3_APPLET_T*)iapplet_ptr;
    static BOOLEAN is_istyle = FALSE;  //NEWMS00168735   //ydsh@20120228
    static BOOLEAN is_landscape = FALSE;
    BOOLEAN is_playfile = FALSE;
#ifdef MMIAP_F_RELEASE_BG_AUDIO
    BOOLEAN ret = FALSE;
#endif

    //SCI_TRACE_LOW:"mmi MP3Applet_HandleEvent msg_id = 0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_19781_112_2_18_2_5_19_394, (uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_APPLET_START:
        {
            SCI_TRACE_LOW("BT_AV, MSG_APPLET_START");
            if(!MMIMP3_ReqPlayHandle())
		    {
			    MMIAPIMP3_StopApplet(FALSE);
			    SCI_TRACE_LOW("MMIMP3_ReqPlayHandle error!");
			    break;
		    }
		    
            is_playfile = *(BOOLEAN*)param;

            //not start bg
            if(!MMK_GetAppletState(applet_ptr->caf_applet.app_handle, MMI_APPLET_STATE_START_BG))
            {
                MMIAPMAINWIN_Enter(is_playfile);
            }

#ifdef BT_AV_CT_SUPPORT
		    InitA2dpAvrcpConnectionStatus();
		    if(!MMIAPIMp3_IsAllocAvrcpStringMemory())
		    {
			    AllocateAvrcpStringMemory();
		    }

		    if(MMIAPIMp3_IsBtMusic())
            {
                MMIAPIBT_RouteA2dpSinkAudio();
            }
#endif
        }
        break;

    case MSG_APPLET_SWITCH:
        {
            SCI_TRACE_LOW("BT_AV, MSG_APPLET_SWITCH");
#ifdef BT_AV_SNK_SUPPORT
            if(!MMIAPIBT_IsA2dpSinkConnected())
            {
                MMIAPMAINWIN_Enter(FALSE);
                MMIAP_MGR_Resume();
            }
#else
            MMIAPMAINWIN_Enter(FALSE);

            MMIAP_MGR_Resume();
#endif
        }
        break;

    case MSG_APPLET_STOP:
        SCI_TRACE_LOW("BT_AV, MSG_APPLET_STOP");
#ifdef MMIAP_F_HISTORY_PLAYLIST
        MMIAP_SavePlayListInfo();
#endif


        MMIAPIMP3_StopAudioPlayer();
#ifdef MMIAP_MUSICMARK_SUPPORT
        MMIMP3_SaveMusicMark();
#endif
        MMIMP3_ReleaseAllLists();


        MMIMP3_ReleasePlayHandle();

#ifdef MMIAP_F_HISTORY_PLAYLIST
        MMIAP_SetBgStartFlag(FALSE);
#endif

        //销毁ap info
        MMIAP_DestroyAPInfo();

#ifdef BT_AV_CT_SUPPORT
		FreeAvrcpStringMemory();
#endif
        break;

    case MSG_APPLET_SUSPEND:
        SCI_TRACE_LOW("BT_AV, MSG_APPLET_SUSPEND");
        is_istyle = MMITHEME_IsIstyle();  //NEWMS00168735   //ydsh@20120228
        is_landscape = MMITHEME_IsMainScreenLandscape();
        MMIMP3_ResizeAllListBufferSize(TRUE);

#ifdef MMIAP_F_RELEASE_BG_AUDIO
		if(MMIMP3_IsMp3AppletSuspendByKey())
		{
			MMIAPIAP_ReleaseAudioHandle();
		}
#endif
        break;

    case MSG_APPLET_RESUME:
        SCI_TRACE_LOW("BT_AV, MSG_APPLET_RESUME");
#ifdef MMIAP_F_RELEASE_BG_AUDIO

        ret = MMIAP_RequestAudioHandle();

#ifdef BT_AV_CT_SUPPORT
        if(!ret && PNULL == GetAPSrvHandle() && !MMIAPIMp3_IsBtMusic())
        {
            MMIMP3_ReqPlayHandle();
        }
#endif
#endif
        MMIMP3_ResizeAllListBufferSize(FALSE);


        if(is_istyle != MMITHEME_IsIstyle()
                || is_landscape != MMITHEME_IsMainScreenLandscape()
          )
        {
            MMIAPMAINWIN_ResumeMainWin();
        }

#ifdef BT_AV_CT_SUPPORT
        if(MMIAPIMp3_IsBtMusic())
        {
            MMIAPIBT_RouteA2dpSinkAudio();
        }
#endif

        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : Stop MP3 applet
//  Global resource dependence :
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_StopApplet(BOOLEAN is_minimize)
{
    if(is_minimize)
    {
        //MMK_SetAppletState(applet_handle, MMI_APPLET_STATE_BG_HIDE_WIN, TRUE);
        MMIAPMAINWIN_ReleaseAnimCtrlInfo();
        MMK_HideWinByApplet(SPRD_MP3_APPLET_ID);
    }
    else
    {
        MMK_CloseApplet(SPRD_MP3_APPLET_ID);
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIAPAPPLET_CreateWinTable(
    uint32*            win_table_ptr,
    ADD_DATA           add_data_ptr
)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = SPRD_MP3_APPLET_ID;
    win_table_create.win_table_ptr = win_table_ptr;
    win_table_create.add_data_ptr = add_data_ptr;

    return MMK_CreateWinTable(&win_table_create);
}
