/*****************************************************************************
** File Name:                    mivp_api.c                                  *
** Author:                                                                   *
** Date:           2010/3/11                                                 *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe vp state machine            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010/3/11      apple.zhang      Create
******************************************************************************/

#define MMIVP_API_C
/*---------------------------------------------------------------------------*/
/*                          Include Files                                    */
/*---------------------------------------------------------------------------*/
#include "mmi_app_video_player_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"
#include "mmivp_export.h"
#include "mmivp_nv.h"
#include "mmivp_id.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmivp_menutable.h"
#include "mmivp_internal.h"
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         GLOBAL DEFINITION                                 */
/*---------------------------------------------------------------------------*/
MMI_APPLICATION_T g_mmivp_app={0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :SdpFile_otacalback
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
#ifdef MMI_STREAMING_PLAYER_SUPPORT
#ifdef BROWSER_SUPPORT
LOCAL void SdpFile_otacalback(MMIBROWSER_MIME_CALLBACK_PARAM_T* param_ptr);
#endif
#endif
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :SdpFile_otacalback
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
#ifdef MMI_STREAMING_PLAYER_SUPPORT
#ifdef BROWSER_SUPPORT
LOCAL void SdpFile_otacalback(MMIBROWSER_MIME_CALLBACK_PARAM_T* param_ptr)
{
    if (PNULL == param_ptr)
    {
        return;
    }
    //VP_STRACE_LOW:"[VP] SdpFile_otacalback param_ptr->mime_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_API_57_112_2_18_3_5_0_0,(uint8*)"d",param_ptr->mime_type);
    if (param_ptr->mime_type == MMIBROWSER_MIME_TYPE_SDP)
    {        
        MMIAPIVP_PlayVideoFromVideoPlayer(VP_ENTRY_FROM_NET,param_ptr->file_name,MMIAPICOM_Wstrlen(param_ptr->file_name),(uint8 *)(param_ptr->url_ptr));  
    }
}
#endif
#endif
/*****************************************************************************/
//  Description : init vp module
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_InitModule(void)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT
#ifdef BROWSER_SUPPORT
    MMIBROWSER_MIME_DOWNLOAD_PARAM_T http_sdpcallback = {0};

    http_sdpcallback.mime_type = MMIBROWSER_MIME_TYPE_SDP;
    http_sdpcallback.call_back = SdpFile_otacalback;
    MMIAPIBROWSER_RegMimeFunc(&http_sdpcallback);
#endif
#endif
    
    g_mmivp_app.ProcessMsg=MMIVP_DispatchSignalToFsmMsg;

    MMIVP_RegNv();
}

/*****************************************************************************/
//  Description : init vp
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIVP_Init(void)
{    
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
    MMIVP_RegMenuGroup();
#endif
    MMIVP_RegWinIdNameArr();
    VP_CreateFsmMutex();
}

#ifdef MMI_STREAMING_PLAYER_SUPPORT//NEWMS00164417
/*****************************************************************************/
//  Description : get  vp net win id
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPIVP_GetVPNetWinID(void )
{
    return MMIVP_NET_WIN_ID;
}
#endif

