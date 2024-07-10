/****************************************************************************
** File Name:      mmiwww_netfront.c                                            *
** Author:                                                                    *
** Date:           06/15/2009                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe the netfront for WWW Browser            *
*****************************************************************************
**                         Important Edit History                            *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 06/2009        wancan.you        Create
** 
****************************************************************************/
#define _MMIWWW_NETFRONT_C_  

/**-------------------------------------------------------------------------*
**                         Dependencies                                     *
**--------------------------------------------------------------------------*/
#include "mmi_app_www_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"
#include "mmi_theme.h"
#include "mmiwww_export.h"
#include "mmiwww_internal.h"
#include "mmiwww_netfront.h"
#include "mmi_position.h"
#include "gui_ucs2b_converter.h"
#include "mmi_nv.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiwww_position.h"
#include "guilcd.h"
#include "mmiwww_func.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmiwww_bookmark.h"

#ifdef BROWSER_SUPPORT_NETFRONT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                          EXTERN DECLARE                                *
 **--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Netfront Initialize
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Netfront_InitPeerData(MMIWWW_PEER_DATA_T *ppeer_data,
                                                                    MMIWWW_DEVICE_INFO_T *pdevice_info,
                                                                    int16 softkey_height);

/*****************************************************************************/
//  Description : Netfront Initialize
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Slim_Peer_Initialize(slim_byte *in_heap, slim_int in_heap_size);

/*****************************************************************************/
//  Description : Netfront Finalize
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Slim_Peer_Finalize(void);

/*****************************************************************************/
//  Description : Netfront Initialize
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int Netfront_NFBrowserNew(MMIWWW_INSTANCE_T *self);

/*****************************************************************************/
//  Description : Netfront Browser Delete
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void Netfront_NFBrowserDelete(void);

/*****************************************************************************/
//  Description : SCI Key ID to NF Key
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_bool SCI_Key_To_NF_Key(uint32 key_type, slim_int *out_keycode, slim_int *out_keychar,slim_int *out_keypress);

/*****************************************************************************/
// Description : Destroy Inline IME Memory
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
LOCAL void MMIWWW_DestroyInlineIMEMemory(void);

/*****************************************************************************/
// Description : Destroy Added Url Memory
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL void MMIWWW_DestroyAddedUrlMemory(void);

/*****************************************************************************/
// Description : Destroy add title Memory
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL void MMIWWW_DestroyAddedTitleMemory(void);

/*****************************************************************************/
// Description : Destroy Upload Memory
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
LOCAL void MMIWWW_DestroyUploadMemory(void);

/*****************************************************************************/
// Description : Correct y coordinate by display mode
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL slim_bool mmiwww_CorrectYCoordinate(slim_int  *inout_y);

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
#ifndef SLIM_CONFIG_USE_PLATE_DEFAULT_STYLE
SLIM_ALIGN LOCAL const slim_byte cSkinImg[] =
{
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
    #include "style_resolve_be.h"
#else
    #include "style_resolve_le.h"
#endif
};
#endif


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Netfront Initialize
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Netfront_InitPeerData(MMIWWW_PEER_DATA_T *ppeer_data,
                                                                    MMIWWW_DEVICE_INFO_T *pdevice_info,
                                                                    int16 softkey_height)
{
    LCD_INFO_T  lcd_info = {0};

    Slim_Peer_Initialize(PNULL, 0);//TODO

    ppeer_data->fArchCharSet  = WIRELESS_CONFIG_SYSTEM_CHARSET;
    ppeer_data->fX            = 0;
    ppeer_data->fY            = MMI_STATUSBAR_HEIGHT + MMIWWW_PAGE_TITLE_HEIGHT;
    ppeer_data->fScreenWidth  = pdevice_info->screen_width;
    ppeer_data->fScreenHeight = pdevice_info->screen_height;

    //SCI_TRACE_LOW:"Netfront_InitPeerData: width=%d, height=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_156_112_2_18_3_11_42_83,(uint8*)"dd", pdevice_info->screen_width, pdevice_info->screen_height);

    ppeer_data->fSoftkeyHeight = softkey_height;
    ppeer_data->fColorDepth = pdevice_info->color_depth;
    ppeer_data->fLockScreenCount = 0;
    ppeer_data->fDevEncCharSet = pdevice_info->dw_lang;
    ppeer_data->fNoUpdateDisplay = TRUE;
    ppeer_data->fFontSetting = MMIWWW_GetFontStyle();

    if (GUILCD_GetInfo(GUI_MAIN_LCD_ID,&lcd_info))
    {
        GUI_LCD_DEV_INFO lcd_dev_info = {0};

        ppeer_data->lcd_ptr = (void*)GUIBLOCK_GetBuf(&lcd_dev_info);        
        switch(lcd_info.bits_per_pixel)
        {
        case 8:
            ppeer_data->fPixelMapType = SLIM_WWWPIXMAP_RGB8;
            break;

        case 16:
            ppeer_data->fPixelMapType = SLIM_WWWPIXMAP_RGB16;
            break;

        case 32:
            ppeer_data->fPixelMapType = SLIM_WWWPIXMAP_RGB32;
            break;

        default:
            ppeer_data->fPixelMapType = SLIM_WWWPIXMAP_RGB16;
            break;
        }
    }
    else
    {
        //SCI_TRACE_MID:"RTOS:Netfront_InitPeerData GUILCD_GetInfo fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_191_112_2_18_3_11_42_84,(uint8*)"");

        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : Netfront Initialize
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Slim_Peer_Initialize(slim_byte *in_heap, slim_int in_heap_size)
{
    slimNetInitializePeer(); //silm net peer init

    return TRUE;
}

/*****************************************************************************/
//  Description : Netfront Finalize
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Slim_Peer_Finalize(void)
{
    slimNetFinalizePeer(); //silm net peer finalize

    return TRUE;
}

/*****************************************************************************/
//  Description : Netfront Initialize
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int Netfront_NFBrowserNew(MMIWWW_INSTANCE_T *self)
{
    slim_int instance = 0;
    NFBrowserInitInfo init_info = {0}, *info = PNULL;
#ifdef SLIM_CONFIG_MOD_MEMORY
    TTankInfo tank_info = {0};
#endif /* SLIM_CONFIG_MOD_MEMORY */
    char * init_ua_ptr = PNULL;
    MMIENG_UA_T ua = {0};

    if (PNULL == self)
    {
        return SLIM_E_GENERIC;
    }
    
    info = &init_info;
#ifdef SLIM_CONFIG_MOD_MEMORY
    tank_info.fAutoExtend = FALSE;
    tank_info.fMinSubTankBytes = BRSCFG_MIN_SUBTANK_SIZE;
    tank_info.fMaxTotalBytes = (MMIWWW_MEMORY_SIZE - MMIWWW_DISPLAY_BUFFER_SIZE);
    info->fTankInfo = &tank_info;
    info->fMainHeapSize = (MMIWWW_MEMORY_SIZE - MMIWWW_DISPLAY_BUFFER_SIZE) - BRSCFG_MIN_SUBTANK_SIZE * BRSCFG_MIN_SUBTANK_NUM;
#else
    info->fMainHeapSize = MMIWWW_MEMORY_SIZE - MMIWWW_DISPLAY_BUFFER_SIZE;

#endif /* SLIM_CONFIG_MOD_MEMORY */
    //SCI_TRACE_LOW:"Netfront_NFBrowserNew mem = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_251_112_2_18_3_11_42_85,(uint8*)"d", info->fMainHeapSize);
    //SCI_TRACE_LOW:"Netfront_NFBrowserNew NF Version: %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_252_112_2_18_3_11_42_86,(uint8*)"s", (char *)NFBrowser_GetVersion());

    info->fMaxTab = MMIWWW_BROWSER_MAX_TAB;
    info->fPixelMapType = SLIM_WWWPIXMAP_RGB16;
    info->fPixelMapTypeA = SLIM_WWWPIXMAP_RGAB16;
    info->fScreenWidth  = self->peer_data.fScreenWidth;
    info->fScreenHeight = self->peer_data.fScreenHeight;

#ifndef SLIM_CONFIG_USE_PLATE_DEFAULT_STYLE
    info->fSkin = cSkinImg;
    info->fSkinSize = sizeof(cSkinImg) / sizeof(cSkinImg[0]);
#endif

    init_ua_ptr = (char *)MMIWWW_GetInitUA();

    SCI_MEMSET((char *)(info->fSettings.fUserAgent), 0, MAX_WEB_URL_LEN);
    if (PNULL != init_ua_ptr)
    {
        strcpy((char *)(info->fSettings.fUserAgent), (char *)init_ua_ptr);
    }
    else
    {
        MMIAPIENG_GetUa(&ua);
        if (0 != SCI_STRLEN((char *)ua.user_agent))
        {
            strcpy((char *)(info->fSettings.fUserAgent), (char *)ua.user_agent);
        }
        else
        {
            strcpy((char *)(info->fSettings.fUserAgent), (char *)MMIAPICOM_GetUserAgent());
        }
    }

    SCI_MEMSET((char *)(info->fSettings.fAccept), 0, MAX_WEB_URL_LEN);
    strcpy((char *)(info->fSettings.fAccept), (char *)BRSCFG_HTTP_HEADER_ACCEPT_FIELD);

    SCI_MEMSET((char *)(info->fSettings.fAcceptLanguage), 0, MAX_WEB_URL_LEN);
    strcpy((char *)(info->fSettings.fAcceptLanguage), (char *)BRSCFG_HTTP_HEADER_ACCEPT_LANGUAGE_FIELD);

    SCI_MEMSET((char *)(info->fSettings.fAcceptCharset), 0, MAX_WEB_URL_LEN);	
    strcpy((char *)(info->fSettings.fAcceptCharset), (char *)BRSCFG_HTTP_HEADER_ACCEPT_CHARSET_FIELD);

    SCI_MEMSET((char *)(info->fSettings.fVisitHistoryFile), 0, MAX_FILE_NAME_LEN);
    strcpy((char*)(info->fSettings.fVisitHistoryFile),(char *)BRSCFG_VISITHISTORY_FILENAME);

    SCI_MEMSET((char *)(info->fSettings.fCookieFile), 0, MAX_FILE_NAME_LEN);
    strcpy((char*)(info->fSettings.fCookieFile), (char *)BRSCFG_COOKIE_FILENAME);

    SCI_MEMSET((char *)(info->fSettings.fAuthFile), 0, MAX_FILE_NAME_LEN);
    strcpy((char*)(info->fSettings.fAuthFile), (char *)BRSCFG_AUTH_FILENAME);

    SCI_MEMSET((char *)(info->fSettings.fCacheDir), 0, MAX_FILE_NAME_LEN);
    strcpy((char*)(info->fSettings.fCacheDir), (char *)BRSCFG_CACHE_DIR);

    SCI_MEMSET((char *)(info->fSettings.fVisitedPageThumbnailDir), 0, MAX_FILE_NAME_LEN);
    strcpy((char*)(info->fSettings.fVisitedPageThumbnailDir), (char *)BRSCFG_VISITEDPAGE_THUMBNAIL_DIR);

    SCI_MEMSET((char *)(info->fSettings.fWebdataFile), 0, MAX_FILE_NAME_LEN);	
    strcpy((char*)(info->fSettings.fWebdataFile), (char *)BRSCFG_WEBDATA_FILENAME);

    info->fSettings.fSaveWebdata = MMIWWW_GetWebDataMode();
    info->fSettings.fDisplayTable = MMIWWW_GetTableMode();
    info->fSettings.fUseVisitedPageThumbnail = FALSE;
    info->fSettings.fEnableBGSound = MMIWWW_GetBGSoundMode();
    info->fSettings.fDisplayAnimateImage = MMIWWW_GetAnimateMode();
    info->fSettings.fViewMode = (NFBrowserViewMode)MMIWWW_GetViewMode();
    info->fSettings.fCookieMode = (NFBrowserCookieMode)MMIWWW_GetCookieMode();
    info->fSettings.fCacheMode = (NFBrowserCacheMode)MMIWWW_GetCacheMode();
    info->fSettings.fImageMode = (NFBrowserImageMode)MMIWWW_GetImageMode();
    info->fSettings.fScriptMode = (NFBrowserScriptMode)MMIWWW_GetScriptMode();
    info->fSettings.fCache_length = (uint32)BRSCFG_CACHE_SIZE;
#ifdef SLIM_CONFIG_FIX_SMARTFOCUS_MOVE_CYCLIC
    info->fSettings.fCyclicFocus = TRUE;
#endif
    MMIWWW_SetImageDisplayMode(info->fSettings.fImageMode);

    info->fCommandCallbacks.fNotifyErrorProc = MMIWWW_Netfront_NotifyErrorProc;
    info->fCommandCallbacks.fNotifyContentErrorProc = MMIWWW_Netfront_NotifyContentErrorProc;
    info->fCommandCallbacks.fRequestConnectionProc = PNULL;//MMIWWW_Netfront_RequestConnectionProc;
    info->fCommandCallbacks.fRequestExSchemeRequestProc = MMIWWW_Netfront_RequestExSchemeRequestProc;
    info->fCommandCallbacks.fRequestAuthProc = MMIWWW_Netfront_RequestAuthProc;
    info->fCommandCallbacks.fRequestCookieConfirmProc = MMIWWW_Netfront_RequestCookieConfirmProc;
    info->fCommandCallbacks.fRequestServerCertConfirmProc = PNULL;//MMIWWW_Netfront_RequestServerCertConfirmProc;
    info->fCommandCallbacks.fRequestJSAlertProc = MMIWWW_Netfront_RequestJSAlertProc;
    info->fCommandCallbacks.fRequestJSConfirmProc = MMIWWW_Netfront_RequestJSConfirmProc;
    info->fCommandCallbacks.fRequestJSPromptProc = MMIWWW_Netfront_RequestJSPromptProc;
    info->fCommandCallbacks.fRequestOpenWindowProc = MMIWWW_Netfront_RequestOpenWindowProc;
    info->fCommandCallbacks.fNotifyWindowStartProc = MMIWWW_Netfront_NotifyWindowStartProc;
    info->fCommandCallbacks.fNotifyWindowDoneProc = MMIWWW_Netfront_NotifyWindowDoneProc;
#ifdef SLIM_CONFIG_USE_WML
    info->fCommandCallbacks.fNotifyWMLErrorProc = PNULL;//MMIWWW_Netfront_NotifyWMLErrorProc;
#endif
    info->fCommandCallbacks.fNotifyInputFormatErrorProc = MMIWWW_Netfront_NotifyInputFormatErrorProc;
    info->fCommandCallbacks.fNotifyXMLSAXErrorProc = PNULL;//MMIWWW_Netfront_NotifyXMLSAXErrorProc;
#ifdef SLIM_CONFIG_USE_WMLSCRIPT
    info->fCommandCallbacks.fRequestWMLSAlertProc = MMIWWW_Netfront_RequestWMLSAlertProc;
    info->fCommandCallbacks.fRequestWMLSConfirmProc = MMIWWW_Netfront_RequestWMLSConfirmProc;
    info->fCommandCallbacks.fRequestWMLSPromptProc = MMIWWW_Netfront_RequestWMLSPromptProc;
#endif	
    info->fCommandCallbacks.fHelperProc = MMIWWW_Netfront_HelperProc;
    info->fCommandCallbacks.fHelperDoneProc = MMIWWW_Netfront_HelperDoneProc;

    info->fInlineImeCallbacks.fEnterTextControlProc = MMIWWW_EnterTextControlProc;
    info->fInlineImeCallbacks.fLeaveTextControlProc = MMIWWW_LeaveTextControlProc;
    info->fInlineImeCallbacks.fMoveTextControlProc = MMIWWW_MoveTextControlProc;
    info->fInlineImeCallbacks.fGetTextControlValueProc = MMIWWW_GetTextControlValueProc;
    info->fInlineImeCallbacks.fGetTextControlValueLengthProc = MMIWWW_GetTextControlValueLengthProc;
    info->fInlineImeCallbacks.fSetTextControlValueProc = MMIWWW_SetTextControlValueProc;
    info->fInlineImeCallbacks.fShowTextControlProc = MMIWWW_ShowTextControlProc;

    info->fFileUploadCallbacks.FileSelectEnterProc = MMIWWW_Netfront_FileSelectEnterProc;
    info->fFileUploadCallbacks.fFileSelectLeaveProc = MMIWWW_Netfront_FileSelectLeaveProc;
    info->fFileUploadCallbacks.fFileSelectQueryNameProc = MMIWWW_Netfront_FileSelectQueryNameProc;

#ifdef SLIM_CONFIG_USE_FAVICON
    info->fFaviconNotifyShowProc = PNULL;//MMIWWW_Netfront_FaviconNotifyShowProc;
#endif /* SLIM_CONFIG_USE_FAVICON */

    info->fNotifyPointerStatesProc = PNULL;//MMIWWW_Netfront_NotifyPointerStatesProc;

#ifdef MMI_AUTOTEST_SUPPORT
    info->fNotifyPointerStatesProc = MMIWWW_Netfront_NotifyPointerStatesProc;
#endif /* MMI_AUTOTEST_SUPPORT */

    info->fCopySelectedTextProc = MMIWWW_Netfront_CopySelectedTextProc;

#ifdef SLIM_CONFIG_USE_RSS
    info->fRssDiscoveryProc = PNULL;
#endif

    info->fTimerImpl.fSetTimer = MMIWWW_Netfront_SetTimer;
    info->fTimerImpl.fKillTimer = MMIWWW_Netfront_KillTimer;
    info->fTimerImpl.fUserData = PNULL;
    info->fUpdateWindowProgressProc = MMIWWW_UpdateWindowProgressProc;
#ifdef WAVE_PWS_UPDATE_WINDOW_TITLE
    info->fUpdateWindowTitleProc = MMIWWW_UpdateWinTitleProc;
#endif
    info->fNotifySecurePageProc = MMIWWW_NotifySecurePageProc;
    info->fNotifyBGSoundExistProc = MMIWWW_NotifyBGSoundExistProc;

    /* return value should be regarded as indentifier of NF browser engine, and used as argument of other APIs */
    instance = NFBrowser_New(PNULL, info);

    if ((SLIM_E_GENERIC != instance) && (PNULL != instance))
    {
        return instance;
    }
    else
    {
        return SLIM_E_GENERIC;
    }
}

/*****************************************************************************/
//  Description : Netfront Initialize
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_Netfront_Initialize(MMIWWW_INSTANCE_T *self)
{
    MMIWWW_DEVICE_INFO_T dev_info = {0};
    int16 softkey_rect_w = 0, softkey_rect_h = 0;

    if (PNULL == self)
    {
        return FALSE;
    }

    dev_info.screen_width  = WWW_PAGE_CLIENT_WIDTH;
    dev_info.screen_height = MMI_MAINSCREEN_HEIGHT;;

    softkey_rect_h = (int16)MMITHEME_GetSoftkeyHeight();
    softkey_rect_w = (int16)(MMITHEME_GetMainScreenRightPixel() / 3);
    
    Netfront_InitPeerData(&self->peer_data, &dev_info, softkey_rect_h);

    self->browser_instance = Netfront_NFBrowserNew(self);
    MMIWWW_Bookmark_Initialize(self);
    MMIWWW_Netfront_NewVisitHistory();
    MMIWWW_Netfront_NewInputURLHistory();
    MMIWWW_Netfront_NewPageMemo();
//    MMIWWW_Netfront_SetPageMemoSortType(MMIWWW_PAGEMEMO_SORT_BY_DATE);
    MMIWWW_SetIsFirstOpen(TRUE);
#ifndef MMI_WIFI_SUPPORT
    MMIWWW_SetHTTPProxy(PNULL);
#endif
     self->is_nf_init = TRUE;
    return TRUE;
}

/*****************************************************************************/
//  Description : Netfront Browser Delete
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void Netfront_NFBrowserDelete(void)
{
    slim_int self = MMIWWW_GETBROWSER;

    if (0 != self)
    {
        NFBrowser_Stop(self);
        NFBrowser_Delete(self);
    }
}

/*****************************************************************************/
//  Description : Netfront Finalize
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_Netfront_Finalize(MMIWWW_INSTANCE_T *self)
{
    if (self->is_nf_init)
    {
        MMIWWW_Netfront_DeleteVisitHistory();
        MMIWWW_Bookmark_finalize();
        MMIWWW_Netfront_DeleteInputURLHistory();
        MMIWWW_Netfront_DeletePageMemo();
        Netfront_NFBrowserDelete();

        MMIWWW_DestroyInlineIMEMemory();
        MMIWWW_DestroyUploadMemory();
        MMIWWW_DestroyAddedUrlMemory();
        MMIWWW_DestroyAddedTitleMemory();

        MMIWWW_FreeInitUA();

        MMIWWW_FreeSearchString();

        MMIWWW_FreeJSPromtString();

        MMIWWW_FreeFirstURL();
        MMIWWW_FreeAuthParam();

        Slim_Peer_Finalize();

        self->is_nf_init = FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : handle backward forward
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_HandleBackForward(slim_bool is_backward)
{
    slim_bool ret_code = TRUE;
    slim_int self = MMIWWW_GETBROWSER;

    if (is_backward)
    {
        if (NFBrowser_HistoryCanBackForward(self, -1))
        {
            NFBrowser_HistoryBackForward(self, -1);
        }
        else
        {
            ret_code = FALSE;
        }
    }
    else
    {
        if (NFBrowser_HistoryCanBackForward(self, 1))
        {
            NFBrowser_HistoryBackForward(self, 1);
        }
        else
        {
            ret_code = FALSE;
        }
    }

    return ret_code;
}

/*****************************************************************************/
//  Description : Check browser whether or not back/forward
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_BrowserCanBackForward(slim_bool  is_backward)
{
    slim_bool ret_code = TRUE;

    if (is_backward)
    {
        ret_code = NFBrowser_HistoryCanBackForward(MMIWWW_GETBROWSER, -1);
    }
    else
    {
        ret_code = NFBrowser_HistoryCanBackForward(MMIWWW_GETBROWSER, 1);
    }
    return ret_code;
}

/*****************************************************************************/
//  Description : handle msg event
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_HandleMsgEvent(uint32 msg_id, DPARAM param)
{
    slim_int key_code = 0;
    slim_int key_char = 0;
    slim_int key_press = 0;
    slim_bool recode = TRUE;
    slim_int x = 0;
    slim_int y = 0;
    NFBrowserInputParam user_input = {0};/*lint !e64*/
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL == www_instance)
    {
        //SCI_TRACE_LOW:"MMIWWW_HandleMsgEvent www_instance Is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_563_112_2_18_3_11_43_87,(uint8*)"");

        return FALSE;
    }

    //SCI_TRACE_LOW:"MMIWWW_HandleMsgEvent msg_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_568_112_2_18_3_11_43_88,(uint8*)"d", msg_id);

    SCI_MEMSET(&user_input, 0x00, sizeof(NFBrowserInputParam));

    switch(msg_id)
    {
    case MSG_KEYDOWN_STAR:
    case MSG_KEYUP_STAR:
    case MSG_KEYDOWN_HASH:
    case MSG_KEYUP_HASH:
    case MSG_KEYDOWN_UP:
    case MSG_KEYUP_UP:
    case MSG_KEYDOWN_DOWN:
    case MSG_KEYUP_DOWN:
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYUP_LEFT:
    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYUP_RIGHT:
    case MSG_KEYDOWN_WEB:
    case MSG_KEYUP_WEB:
    case MSG_KEYPRESSUP_UP:
    case MSG_KEYPRESSUP_DOWN:
    case MSG_KEYPRESSUP_LEFT:
    case MSG_KEYPRESSUP_RIGHT:
    case MSG_KEYREPEAT_UP:
    case MSG_KEYREPEAT_DOWN:
    case MSG_KEYREPEAT_LEFT:
    case MSG_KEYREPEAT_RIGHT:
        if (SCI_Key_To_NF_Key(msg_id, &key_code, &key_char, &key_press))
        {
            user_input.u.fKey.fKeyCode = key_code;
            user_input.u.fKey.fKeyChar = key_char;
            user_input.fType = (NFBrowserInputType)key_press;

            if (SLIM_E_OK != NFBrowser_UserInput(www_instance->browser_instance, &user_input))
            {
                recode = FALSE;
            }
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
    case MSG_TP_PRESS_UP:
    case MSG_TP_PRESS_MOVE:
        x = MMK_GET_TP_X(param);
        y = MMK_GET_TP_Y(param);
        if (mmiwww_CorrectYCoordinate(&y))
        {
            slim_bool is_success = slim_false;
            slim_bool is_move = slim_false;
            TRect view = {0};
            TRect document = {0};
            static slim_int move_x = 0;
            static slim_int move_y = 0;
            slim_int by_x = 0;
            slim_int by_y = 0;
            slim_int to_x = 0;
            slim_int to_y = 0;

            user_input.u.fMouse.fX = x;
            user_input.u.fMouse.fY = y;
            user_input.fType = INPUT_TYPE_MOUSE_DOWN + (msg_id - MSG_TP_PRESS_DOWN);
            user_input.u.fMouse.fButton = SLIM_MOUSEBUTTON_LEFT;

			if(MSG_TP_PRESS_MOVE == msg_id && MMIWWW_COPY_MODE_NONE != MMIWWW_GetCopyMode())
			{
			    user_input.u.fMouse.fModifiers |= SLIM_MOUSE_LEFT_MASK;
			}

            NFBrowser_UserInput(www_instance->browser_instance, &user_input);

            if (MSG_TP_PRESS_DOWN == msg_id)
            {
                move_x = x;
                move_y = y;
                is_move = slim_false;
            }
            else if (MSG_TP_PRESS_MOVE == msg_id)
            {
                is_move = slim_true;
            }
            else
            {
                move_x = 0;
                move_y = 0;
                is_move = slim_false;
            }

            if (is_move)
            {
                by_x = x - move_x;
                by_y = y - move_y;

                if (abs(by_x) > MMIWWW_MOUSE_ANTI_SHAKE_MOVE_X
                    && abs(by_y) > MMIWWW_MOUSE_ANTI_SHAKE_MOVE_Y)
                {
                    if (NFBrowser_ViewRect(www_instance->browser_instance, &view)
                        && NFBrowser_DocumentRect(www_instance->browser_instance, &document))
                    {
                        if (abs(by_x) <= abs(by_y))
                        {
                            if (by_y > 0 && view.fY <= document.fY)
                            {
                                recode = FALSE;

                                return recode;
                            }
                            else if (by_y < 0 && (view.fY + view.fHeight - 1) >= (document.fY + document.fHeight - 1))
                            {
                                recode = FALSE;

                                return recode;
                            }
                        
                            to_x = view.fX;
                            to_y = view.fY - by_y;
                        }
                        else
                        {
                            if (by_x > 0 && view.fX <= document.fX)
                            {
                                recode = FALSE;

                                return recode;
                            }
                            else if (by_x < 0 && (view.fX + view.fWidth - 1) >= (document.fX + document.fWidth- 1))
                            {
                                recode = FALSE;

                                return recode;
                            }

                            to_x = view.fX - by_x;
                            to_y = view.fY;
                        }

						if(MMIWWW_COPY_MODE_NONE == MMIWWW_GetCopyMode())
                        {
                            if (NFBrowser_ScrollBy(www_instance->browser_instance, by_x, by_y))
                            {
                                is_success = NFBrowser_ScrollTo(www_instance->browser_instance, to_x, to_y);
                            }
                        }
                    }
                }
            }
            if (!is_success)
            {
                recode = FALSE;
            }        
        }
        else
        {
            recode = FALSE;
        }

		if(MSG_TP_PRESS_UP == msg_id && MMIWWW_COPY_MODE_NONE != MMIWWW_GetCopyMode())
        {
            NFBrowser_CopySelectedText(www_instance->browser_instance);
            MMIWWW_SetCopyMode(MMIWWW_COPY_MODE_NONE);
            recode = TRUE;
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    default:
        recode = FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
// Description : Correct y coordinate by display mode
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL slim_bool mmiwww_CorrectYCoordinate(slim_int  *inout_y)
{
    slim_int     offset = 0;
    slim_bool    bret = slim_true;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL == www_instance)
    {
        return slim_false;
    }

    offset = www_instance->peer_data.fY;

    if (*inout_y > offset)
    {
        *inout_y -= offset;
    }
    else
    {
        bret = slim_false;
    }

    return bret;
}

/*****************************************************************************/
//  Description : SCI Key ID to NF Key
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_bool SCI_Key_To_NF_Key(uint32 key_type, slim_int *out_keycode, slim_int *out_keychar,slim_int *out_keypress)
{
    slim_int keycode = SLIM_VK_UNDEFINED;
    slim_int keychar = SLIM_CHAR_UNDEFINED;
    slim_int keypress = INPUT_TYPE_KEYPRESS;
    slim_bool ret_code = TRUE;

    if (KEY_LONG_RELEASED == (key_type & 0xFF00))
    {
        key_type = KEY_RELEASED | key_type & 0x00FF;
    }

    if (KEY_PRESSED == (key_type & 0xFF00))
    {
        keypress = INPUT_TYPE_KEYPRESS;
    }
    else if (KEY_RELEASED == (key_type & 0xFF00))
    {
        keypress = INPUT_TYPE_KEYRELEASE;
    }
    else if (KEY_REPEATED == (key_type & 0xFF00))
    {
        keypress = INPUT_TYPE_KEY;
    }

    if (INPUT_TYPE_KEYPRESS != keypress && INPUT_TYPE_KEYRELEASE != keypress && INPUT_TYPE_KEY != keypress)
    {
        return FALSE;
    }

    switch (key_type)
    {
    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9:
    case MSG_KEYUP_1:
    case MSG_KEYUP_2:
    case MSG_KEYUP_3:
    case MSG_KEYUP_4:
    case MSG_KEYUP_5:
    case MSG_KEYUP_6:
    case MSG_KEYUP_7:
    case MSG_KEYUP_8:
    case MSG_KEYUP_9:
        if (KEY_PRESSED == (key_type & 0xFF00))
        {
            keycode = SLIM_VK_1 + ((slim_int)key_type - MSG_KEYDOWN_1);
            keychar = '1' + ((slim_int)key_type - MSG_KEYDOWN_1);
        }
        else
        {
            keycode = SLIM_VK_1 + ((slim_int)key_type - MSG_KEYUP_1);
            keychar = '1' + ((slim_int)key_type - MSG_KEYUP_1);
        }
        break;

    case MSG_KEYDOWN_0:
    case MSG_KEYUP_0:
        keycode = SLIM_VK_0;
        keychar = '0';
        break;

    case MSG_KEYDOWN_WEB:
    case MSG_KEYUP_WEB:
        keycode = SLIM_VK_ENTER;
        keychar = SLIM_INTERNAL_NEWLINE;
        break;

    case MSG_KEYDOWN_STAR:
    case MSG_KEYUP_STAR:
        keycode = SLIM_VK_PAGE_UP;
        keychar = '*';
        break;

    case MSG_KEYDOWN_HASH:
    case MSG_KEYUP_HASH:
        keycode = SLIM_VK_PAGE_DOWN;
        keychar = '#';
        break;

    case MSG_KEYDOWN_UP:
    case MSG_KEYUP_UP:
	case MSG_KEYREPEAT_UP:
        keycode = SLIM_VK_UP;
        break;

    case MSG_KEYDOWN_DOWN:
    case MSG_KEYUP_DOWN:
	case MSG_KEYREPEAT_DOWN:
        keycode = SLIM_VK_DOWN;
        break;

    case MSG_KEYDOWN_LEFT:
    case MSG_KEYUP_LEFT:
	case MSG_KEYREPEAT_LEFT:
        keycode = SLIM_VK_LEFT;
        break;

    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYUP_RIGHT:
	case MSG_KEYREPEAT_RIGHT:
        keycode = SLIM_VK_RIGHT;
        break;

    default:
        ret_code = FALSE;
        break;
    }

    *out_keycode = keycode;
    *out_keychar = keychar;
    *out_keypress = keypress;

    //SCI_TRACE_LOW:"SCI_Key_To_NF_Key keycode = 0x%04x, keychar = 0x%04x, keypress = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_892_112_2_18_3_11_44_89,(uint8*)"ddd", keycode, keychar, keypress);
    return ret_code;
}

/*****************************************************************************/
//  Description : Netfront Start Browser
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_StartBrowser(MMIWWW_INSTANCE_T *self, NFBrowserStartType in_type, char *in_url)
{
    slim_int ret = 0;
    NFBrowserStartParam brwstart = {0};/*lint !e64*/
    uint16 url_len =0;
    
    if (PNULL == self)
    {
        return SLIM_E_GENERIC;
    }

    SCI_MEMSET(&brwstart, 0x00, sizeof(NFBrowserStartParam));

    brwstart.fStartType = in_type;

    if (PNULL != in_url)
    {
        url_len = SCI_STRLEN(in_url);
        SCI_MEMCPY((char *)brwstart.fURL, in_url, MIN(url_len, MAX_WEB_URL_LEN));
    }

    MMIWWW_SetBrowserstopFlag(FALSE);

    ret = NFBrowser_Start(self->browser_instance, &brwstart);
    //SCI_TRACE_LOW:"MMIWWW_Netfront_StartBrowser ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_924_112_2_18_3_11_44_90,(uint8*)"d", ret);

    return ret;
}

/*****************************************************************************/
//  Description : Netfront Stop Browser
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_StopBrowser(void)
{
    slim_int ret = 0;
    NFBrowserInputParam user_input = {0};

    MMIWWW_SetBrowserstopFlag(TRUE);

    user_input.u.fMouse.fX = -1;
    user_input.u.fMouse.fY = -1;
    user_input.fType = INPUT_TYPE_MOUSE_DOWN ;
    user_input.u.fMouse.fButton = SLIM_MOUSEBUTTON_LEFT;
    NFBrowser_UserInput(MMIWWW_GETBROWSER, &user_input);
    
    user_input.u.fMouse.fX = -1;
    user_input.u.fMouse.fY = -1;
    user_input.fType = INPUT_TYPE_MOUSE_UP;
    user_input.u.fMouse.fButton = SLIM_MOUSEBUTTON_LEFT;
    NFBrowser_UserInput(MMIWWW_GETBROWSER, &user_input);

    ret = NFBrowser_Stop(MMIWWW_GETBROWSER);
    //SCI_TRACE_LOW:"MMIWWW_Netfront_StopBrowser ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_953_112_2_18_3_11_44_91,(uint8*)"d", ret);

    return ret;
}

/*****************************************************************************/
//  Description : Netfront Suspend Browser
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_SuspendBrowser(void)
{
    slim_int ret = 0;

    ret = NFBrowser_Suspend(MMIWWW_GETBROWSER);
    //SCI_TRACE_LOW:"MMIWWW_Netfront_SuspendBrowser ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_967_112_2_18_3_11_44_92,(uint8*)"d", ret);

    return ret;
}

/*****************************************************************************/
//  Description : Netfront Resume Browser
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_ResumeBrowser(void)
{
    slim_int ret = 0;
    slim_int self = 0;
    slim_int act_index = 0;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL == www_instance)
    {
        return SLIM_E_GENERIC;
    }

    act_index = MMIWWW_Netfront_GetActWinIndex();
    self = www_instance->browser_instance;

    if (act_index >= MMIWWW_BROWSER_MAX_TAB)
    {
        return SLIM_E_GENERIC;
    }

    ret = NFBrowser_Resume(self);

    if (MMIWWW_GetBrowserReloadFlag(act_index))
    {
        MMIWWW_SetBrowserReloadFlag(act_index, FALSE);
        MMIWWW_Netfront_ReloadPage();
    }
    
    //SCI_TRACE_LOW:"MMIWWW_Netfront_ResumeBrowser ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_1004_112_2_18_3_11_44_93,(uint8*)"d", ret);

    return ret;
}

/*****************************************************************************/
//  Description : Netfront Leave Text Control Edit Mode
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_LeaveTextControlEditMode(slim_char *in_str)
{
    slim_bool ret = 0;

    if (PNULL != in_str)
    {
        ret = NFBrowser_LeaveTextControlEditMode(MMIWWW_GETBROWSER, in_str);
    }
    else
    {
        slim_char null_str[2] = {0};

        ret = NFBrowser_LeaveTextControlEditMode(MMIWWW_GETBROWSER, (slim_char *)null_str);
    }

    return ret;
}

/*****************************************************************************/
// Description : Reload displayed content in active browsing window
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_ReloadPage(void)
{
    slim_int    ret = SLIM_E_OK;
    MMIWWW_STATE_T state = MMIWWW_STATE_PAGE_LOADING;

    state = MMIWWW_GetWWWState(MMIWWW_Netfront_GetActWinIndex());
    //SCI_TRACE_LOW:"MMIWWW_Netfront_ReloadPage() state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_1041_112_2_18_3_11_44_94,(uint8*)"d", state);
    if (MMIWWW_STATE_PAGE_LOADING == state)
    {
        ret = NFBrowser_Abort(MMIWWW_GETBROWSER);
        //SCI_TRACE_LOW:"MMIWWW_Netfront_ReloadPage abort ret =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_1045_112_2_18_3_11_44_95,(uint8*)"d", ret);
    }

    ret = NFBrowser_Reload(MMIWWW_GETBROWSER);
    //SCI_TRACE_LOW:"MMIWWW_Netfront_ReloadPage ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_1049_112_2_18_3_11_44_96,(uint8*)"d", ret);

    return ret;
}

/*****************************************************************************/
// Description : Cancel loading content in active browsing window
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_Abort(void)
{
    slim_int    ret = SLIM_E_OK;

    ret = NFBrowser_Abort(MMIWWW_GETBROWSER);
    //SCI_TRACE_LOW:"MMIWWW_Netfront_Abort ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_1063_112_2_18_3_11_44_97,(uint8*)"d", ret);

    return ret;
}

/*****************************************************************************/
// Description : Cancel loading content in active browsing window
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_Pagepilot(void)
{
    TRect        view_rect = {0};
    TRect        doc_rect = {0};
    slim_bool    bret = slim_true;
    slim_int     ret = SLIM_E_OK;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    slim_int     in_self = 0;
    NFBrowserPixelMap  out_pixelmap = {0};

    if (PNULL == www_instance)
    {
        return slim_false;
    }

    in_self = www_instance->browser_instance;

    bret = NFBrowser_ViewRect(in_self, &view_rect);
    bret = NFBrowser_DocumentRect(in_self, &doc_rect);
    ret = NFBrowser_CreateCurrentPageThumbnail(in_self, www_instance->peer_data.fScreenWidth, 
                                               www_instance->peer_data.fScreenHeight, slim_nil,
                                               &out_pixelmap);

    return bret;
}

/*****************************************************************************/
// Description : Set zoom ratio of active browsing window
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_PageZoom(MMIWWW_ZOOM_MODE_E mode)
{
    slim_bool bret = slim_true;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    slim_int act_index = 0;
    slim_int percent = 0;
    uint32 i = 0;

    if (PNULL == www_instance)
    {
        return slim_false;
    }

    act_index = MMIWWW_Netfront_GetActWinIndex();

    //SCI_TRACE_LOW:"MMIWW_Netfront_PageZoom mode=%d, fZoom_percent=%d, act_index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_1117_112_2_18_3_11_44_98,(uint8*)"ddd", mode, www_instance->peer_data.fZoom_percent[act_index], act_index);

    if (act_index >= MMIWWW_BROWSER_MAX_TAB)
    {
        //SCI_TRACE_LOW:"MMIWWW_Netfront_PageZoom tab error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_1121_112_2_18_3_11_44_99,(uint8*)"");
        return slim_false;
    }

    switch(mode)
    {
    case MMIWWW_ZOOMIN:
        if (MMIWWW_ZOOM_MAX > www_instance->peer_data.fZoom_percent[act_index])
        {
            www_instance->peer_data.fZoom_percent[act_index]++;
        }
        else
        {
            return slim_false;
        }
        break;

      case MMIWWW_ZOOMOUT:
        if (MMIWWW_ZOOM_MIN < www_instance->peer_data.fZoom_percent[act_index])
        {
            www_instance->peer_data.fZoom_percent[act_index]--;
        }
        else
        {
            return slim_false;
        }
        break;

    case MMIWWW_ZOOMORG:
        if (MMIWWW_ZOOM_ORG != www_instance->peer_data.fZoom_percent[act_index])
        {
            www_instance->peer_data.fZoom_percent[act_index] = MMIWWW_ZOOM_ORG;
        }
        else
        {
            return slim_false;
        }
        break;

    case MMIWWW_ZOOMINIT:
        for (i = 0; i < MMIWWW_BROWSER_MAX_TAB; i++)
        {
            www_instance->peer_data.fZoom_percent[i] = MMIWWW_ZOOM_ORG;
        }
        return bret;

    default:
        break;
    }

    percent = MMIWWW_PAGE_ZOOM_PERCENT_START + (www_instance->peer_data.fZoom_percent[act_index] - MMIWWW_ZOOM_50) * MMIWWW_PAGE_ZOOM_PERCENT_STEP;
    bret = MMIWWW_Netfront_PageZoomPercent(percent);

    return bret;
}

/*****************************************************************************/
// Description : Set zoom percent browsing window
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_PageZoomPercent(slim_int percent)
{
    slim_bool bret = slim_true;

    bret = NFBrowser_SetZoom(MMIWWW_GETBROWSER, percent);

    //SCI_TRACE_LOW:"MMIWWW_Netfront_PageZoom percent=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_1187_112_2_18_3_11_44_100,(uint8*)"d", percent);
    return bret;
}

/*****************************************************************************/
// Description : Search specified string in active browsing window
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_SearchText(MMI_STRING_T *str_ptr, slim_int flag)
{
    uint16 len = 0;
    uint8 str_buf[WWW_MAX_SEARCH_STRING_LEN * 3 + 1] = {0};
    slim_bool bret = slim_true;
    slim_int in_self = MMIWWW_GETBROWSER;

    if (PNULL == str_ptr)
    {
        return slim_false;
    }

    //SCI_TRACE_LOW:"MMIWWW_Netfront_SearchText flag=0x%x, wstr_len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_1207_112_2_18_3_11_44_101,(uint8*)"dd", flag, str_ptr->wstr_len);
    len = GUI_WstrToUTF8(str_buf, sizeof(str_buf), str_ptr->wstr_ptr, MIN(str_ptr->wstr_len, WWW_MAX_SEARCH_STRING_LEN));
    bret = NFBrowser_SearchText(in_self, str_buf, (slim_int)len, SLIM_CHARSET_UTF_8, flag);

    return bret;
}

/*****************************************************************************/
// Description : Set NF browser view mode in actvie browsing window
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_ViewMode(slim_int in_mode)
{
    slim_int    in_self = MMIWWW_GETBROWSER;

    //SCI_TRACE_LOW:"MMIWWW_Netfront_ViewMode mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_1222_112_2_18_3_11_44_102,(uint8*)"d", in_mode);
    NFBrowser_SetViewMode(in_self, in_mode);
    return;
}

/*****************************************************************************/
// Description : Destroy Added Url Memory
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
LOCAL void MMIWWW_DestroyInlineIMEMemory(void)
{
    MMIWWW_INSTANCE_T *self = MMIWWW_INSTANCE();

    if (PNULL != self && PNULL != self->ime_endparam.ime_result_text)
    {
        SCI_FREE(self->ime_endparam.ime_result_text);
        self->ime_endparam.ime_result_text = PNULL;
    }
}

/*****************************************************************************/
// Description : Destroy add Url Memory
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL void MMIWWW_DestroyAddedUrlMemory(void)
{
    uint8 * added_url = PNULL;

    added_url = MMIWWW_NetfrontGetAddedUrl();
    if (PNULL != added_url)
    {
        SCI_FREE(added_url);
        MMIWWW_NetfrontSetAddedUrl(PNULL);
    }
}

/*****************************************************************************/
// Description : Destroy add title Memory
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL void MMIWWW_DestroyAddedTitleMemory(void)
{
    uint8 * added_title = PNULL;

    added_title = MMIWWW_NetfrontGetAddedTitle();
    if (PNULL != added_title)
    {
        SCI_FREE(added_title);
        MMIWWW_NetfrontSetAddedTitle(PNULL);
    }
}

/*****************************************************************************/
// Description : Destroy Upload Memory
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
LOCAL void MMIWWW_DestroyUploadMemory(void)
{
    MMIWWW_INSTANCE_T *self = MMIWWW_INSTANCE();

    if (PNULL != self && PNULL != self->upload_param.upload_path)
    {
        SCI_FREE(self->upload_param.upload_path);
        self->upload_param.upload_path = PNULL;
    }
}

/*****************************************************************************/
// Description : Set www cookie mode
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC void MMIWWW_Netfront_SetCookieMode(slim_int in_mode)
{
    slim_int self = MMIWWW_GETBROWSER;
    NFBrowser_SetCookieMode(self, in_mode);
}

/*****************************************************************************/
// Description : clear www browser cookie
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_ClearCookie(void)
{
    slim_int self = MMIWWW_GETBROWSER;
    NFBrowser_ClearCookie(self);
}

/*****************************************************************************/
// Description : Set www cookie mode
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC void MMIWWW_Netfront_SetCacheMode(slim_int in_mode)
{
    slim_int self = MMIWWW_GETBROWSER;
    NFBrowser_SetCacheMode(self, in_mode);
}

/*****************************************************************************/
// Description : clear www browser cache
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_ClearCache(void)
{
    slim_int self = MMIWWW_GETBROWSER;
    NFBrowser_ClearCache(self);
}

/*****************************************************************************/
// Description : Set www wether use disk cache
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC void MMIWWW_Netfront_SetUseDiskCacheMode(slim_int in_use)
{
    slim_int self = MMIWWW_GETBROWSER;
    NFBrowser_SetUseDiskCache(self, in_use);
}

/*****************************************************************************/
// Generates visit history instance
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_NewVisitHistory(void)
{
    slim_bool bret = slim_true;
    slim_int self = MMIWWW_GETBROWSER;
    bret = NFBrowser_NewVisitHistory(self);
    return bret;
}

/*****************************************************************************/
// Deletes visit history instance
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_DeleteVisitHistory(void)
{
    slim_int self = MMIWWW_GETBROWSER;
    NFBrowser_DeleteVisitHistory(self);
}

/*****************************************************************************/
// Gets number of visit history entries
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_GetVisitHistoryNumber(void)
{
    slim_int ret = 0;
    slim_int self = MMIWWW_GETBROWSER;
    ret = NFBrowser_GetVisitHistoryNumber(self);
    return ret;
}

/*****************************************************************************/
// Gets URL of specified visit history entry
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_GetVisitHistoryUrl( slim_int in_index, MMI_STRING_T *out_url )
{
    NFBrowserMem url = {0};/*lint !e64*/
    wchar *wstr_ptr = PNULL;
    slim_bool bret = slim_true;
    slim_int self = MMIWWW_GETBROWSER;

    if (PNULL == out_url)
    {
        return slim_false;
    }

    SCI_MEMSET(&url, 0x00, sizeof(NFBrowserMem));

    bret = NFBrowser_GetVisitHistoryUrl( self, in_index, &url );

    if (bret && url.fDataSize > 1)
    {
        wstr_ptr = SCI_ALLOCA(((uint32)url.fDataSize * sizeof(wchar)));
        if (PNULL == wstr_ptr)
        {
            return FALSE;
        }
        SCI_MEMSET(wstr_ptr, 0x00, (((uint32)url.fDataSize * sizeof(wchar))));
        out_url->wstr_len = GUI_UTF8ToWstr(wstr_ptr, (uint16)(url.fDataSize - 1), url.fData, (uint16)(url.fDataSize - 1));
        out_url->wstr_ptr = wstr_ptr;
    }
    NFBrowser_ReleaseSlimMem(&url);

    return bret;
}

/*****************************************************************************/
// Gets title of specified visit history entry
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_GetVisitHistoryTitle( slim_int in_index, MMI_STRING_T *out_title )
{
    NFBrowserMem title = {0};/*lint !e64*/
    wchar *wstr_ptr = PNULL;
    slim_bool bret = slim_true;
    slim_int self = MMIWWW_GETBROWSER;

    if (PNULL == out_title)
    {
        return slim_false;
    }

    SCI_MEMSET(&title, 0x00, sizeof(NFBrowserMem));

    bret = NFBrowser_GetVisitHistoryTitle( self, in_index, &title );

    if (title.fDataSize > 1 && bret)
    {
        wstr_ptr = SCI_ALLOCA(((uint32)title.fDataSize * sizeof(wchar)));
        if (PNULL == wstr_ptr)
        {
            return FALSE;
        }
        SCI_MEMSET(wstr_ptr, 0x00, (((uint32)title.fDataSize * sizeof(wchar))));
        out_title->wstr_len = GUI_UTF8ToWstr(wstr_ptr, (uint16)(title.fDataSize - 1), title.fData, (uint16)(title.fDataSize - 1));
        out_title->wstr_ptr = wstr_ptr;
    }
    
    NFBrowser_ReleaseSlimMem(&title);

    return bret;
}

/*****************************************************************************/
// Gets visit history time
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_GetHistoryTime(slim_int in_index, slim_time *out_time)
{
    slim_bool bret = slim_true;
    slim_int self = MMIWWW_GETBROWSER;

    bret = NFBrowser_VisitHistory_Time(self, in_index, out_time);

    return bret;
}

/*****************************************************************************/
// Deletes visit history entry specified by in_index
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RemoveVisitHistory( slim_int in_index )
{
    slim_int self = MMIWWW_GETBROWSER;
    NFBrowser_RemoveVisitHistory( self, in_index );
}

/*****************************************************************************/
// Deletes all the visit history entries
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RemoveAllVisitHistory(void )
{
    slim_int self = MMIWWW_GETBROWSER;
    NFBrowser_RemoveAllVisitHistory(self);
}

/*****************************************************************************/
// Description : Set Charset
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/ 
PUBLIC void MMIWWW_Netfront_ChangeToEncoding(int32 charset)
{
    slim_int self = MMIWWW_GETBROWSER;
    NFBrowser_ChangeToEncoding(self, charset);
}

  /*****************************************************************************/
// Description : Generates page memo instance
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_NewPageMemo(void)
{
    uint8 sort_type = 0;
    slim_int self = MMIWWW_GETBROWSER;
    slim_bool bret = slim_true;
    NFBrowserPagememoProperty property = {0};

    if( (uint8)MMIWWW_PAGEMEMO_SORT_BY_DATE == MMIWWW_NetfrontGetPagememoSortType() )
    {
        sort_type = NFBPAGEMEMO_SORTTYPE_DATE_NEWEST;
    }
    else
    {
        sort_type = NFBPAGEMEMO_SORTTYPE_TITLE_ASCENDING;
    }

    SCI_MEMSET( &property, 0x00, sizeof(NFBrowserPagememoProperty) );
    property.fFileName = (slim_char *)BRSCFG_PAGEMEMO_FILENAME;
    property.fMaxCapacity = 0;
    property.fMaxItems = 0;
    property.fMaxItemSize = 0;
    property.fMaxTitleLen = 0;
    property.fMaxURLLen = 0;
    property.fSortType = sort_type;

    bret = NFBrowser_NewPageMemo( self, &property );

    return bret;
}

/*****************************************************************************/
// Description : Deletes page memo instance
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC void MMIWWW_Netfront_DeletePageMemo(void)
{
    slim_int self = MMIWWW_GETBROWSER;
    NFBrowser_DeletePageMemo(self);
}

/*****************************************************************************/
// Description : Gets number of page memo entries
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_int MMIWWW_Netfront_GetPageMemoNumber(void)
{
    slim_int ret = 0;
    slim_int self = MMIWWW_GETBROWSER;
    ret = NFBrowser_GetPageMemoNumber(self);
    return ret;
}

/*****************************************************************************/
// Description : Gets URL of specified page memo entry
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_GetPageMemoUrl(slim_int in_index, MMI_STRING_T *out_url)
{
    NFBrowserMem url = {0};/*lint !e64*/
    wchar *wstr_ptr = PNULL;
    slim_bool bret = FALSE;
    slim_int self = MMIWWW_GETBROWSER;
    
    if (PNULL == out_url)
    {
        return slim_false;
    }

    SCI_MEMSET(&url, 0x00, sizeof(NFBrowserMem));

    bret = NFBrowser_GetPageMemoUrl(self, in_index, &url);

    if (bret && url.fDataSize > 1)
    {
        wstr_ptr = SCI_ALLOCA(((uint32)url.fDataSize * sizeof(wchar)));
        if (PNULL == wstr_ptr)
        {
            return FALSE;
        }
        SCI_MEMSET(wstr_ptr, 0x00, (((uint32)url.fDataSize * sizeof(wchar))));
        out_url->wstr_len = GUI_UTF8ToWstr(wstr_ptr, (uint16)(url.fDataSize - 1), url.fData, (uint16)(url.fDataSize - 1));
        out_url->wstr_ptr = wstr_ptr;
    }
    NFBrowser_ReleaseSlimMem(&url);

    return bret;
}

/*****************************************************************************/
// Description : Gets title of specified page memo entry
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_GetPageMemoTitle(slim_int in_index, MMI_STRING_T *out_title)
{
    NFBrowserMem title = {0};/*lint !e64*/
    wchar *wstr_ptr = PNULL;
    slim_bool bret = FALSE;
    slim_int self = MMIWWW_GETBROWSER;
    
    if (PNULL == out_title)
    {
        return slim_false;
    }

    SCI_MEMSET(&title, 0x00, sizeof(NFBrowserMem));

    bret = NFBrowser_GetPageMemoTitle(self, in_index, &title);

    if (bret && title.fDataSize > 1)
    {
        wstr_ptr = SCI_ALLOCA(((uint32)title.fDataSize * sizeof(wchar)));
        if (PNULL == wstr_ptr)
        {
            return FALSE;
        }
        SCI_MEMSET(wstr_ptr, 0x00, (((uint32)title.fDataSize * sizeof(wchar))));
        out_title->wstr_len = GUI_UTF8ToWstr(wstr_ptr, (uint16)(title.fDataSize - 1), title.fData, (uint16)(title.fDataSize - 1));
        out_title->wstr_ptr = wstr_ptr;
    }
    NFBrowser_ReleaseSlimMem(&title);

    return bret;
}

/*****************************************************************************/
// Description : Gets title of specified page memo entry
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_int MMIWWW_Netfront_GetPageMemoSortType(void)
{
    slim_int self = MMIWWW_GETBROWSER;
    slim_int sort_type = NFBPAGEMEMO_SORTTYPE_UNKNOWN;

    sort_type = NFBrowser_GetPageMemoSortType(self);

    return sort_type;
}

/*****************************************************************************/
// Description : Sets sort type of page memo instance
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_SetPageMemoSortType(slim_int in_sort_type)
{
    slim_bool bret = FALSE;
    slim_int self = MMIWWW_GETBROWSER;

    if( in_sort_type == MMIWWW_PAGEMEMO_SORT_BY_DATE )
    {
        bret = NFBrowser_SetPageMemoSortType(self, NFBPAGEMEMO_SORTTYPE_DATE_NEWEST);
    }
    else if( in_sort_type == MMIWWW_PAGEMEMO_SORT_BY_NAME )
    {
        bret = NFBrowser_SetPageMemoSortType(self, NFBPAGEMEMO_SORTTYPE_TITLE_ASCENDING);
    }

    return bret;
}

/*****************************************************************************/
// Description : Deletes page memo entry specified by in_index
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_RemovePageMemo(slim_int in_index)
{
    slim_bool bret = FALSE;
    slim_int self = MMIWWW_GETBROWSER;
    
    bret = NFBrowser_RemovePageMemo(self, in_index);

    return bret;   
}

/*****************************************************************************/
// Description : Deletes all entries of page memo
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_RemoveAllPageMemo(void)
{

    slim_bool bret = FALSE;
    slim_int self = MMIWWW_GETBROWSER;

    bret = NFBrowser_RemoveAllPageMemo(self);

    return bret;
}

/*****************************************************************************/
// Description : Saves current web page to page memo
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_AddPageMemo(MMI_STRING_T *in_title_ptr)
{
    uint8 *title_ptr = PNULL;
    slim_bool bret = FALSE;
    uint16 str_len = 0;
    slim_int self = MMIWWW_GETBROWSER;

    str_len = (uint16)(in_title_ptr->wstr_len * 3 + 1);
    
    title_ptr = SCI_ALLOCA(str_len * sizeof(uint8));

    if( PNULL == title_ptr )
    {
        return FALSE;
    }

    SCI_MEMSET(title_ptr, 0x00, (str_len * sizeof(uint8)));

    if( PNULL != in_title_ptr->wstr_ptr && in_title_ptr->wstr_len > 0 )
    {
        GUI_WstrToUTF8(title_ptr, str_len, in_title_ptr->wstr_ptr, in_title_ptr->wstr_len);
    }

    bret = NFBrowser_AddPageMemo(self, (slim_char *)title_ptr);

    SCI_FREE(title_ptr);
    title_ptr = PNULL;

    return bret;
}

/*****************************************************************************/
// Description : Loads the page memo specified by in_index
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_int MMIWWW_Netfront_LoadPageMemo(slim_int in_index)
{   
    slim_int ret = NFBPAGEMEMO_E_WOULDBLOCK;
    slim_int self = MMIWWW_GETBROWSER;

    while( ret == NFBPAGEMEMO_E_WOULDBLOCK )
    {
        ret = NFBrowser_LoadPageMemo(self, in_index);

        if(ret == SLIM_E_OK)
        {
            return 1;
        }
    }

    return 0;
}

/*****************************************************************************/
// Description : Generates input URL history instance
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_NewInputURLHistory(void)
{
    
    slim_int self = MMIWWW_GETBROWSER;
    slim_bool bret = slim_true;
    NFBrowserInputURLHistoryProperty property = {0};

    SCI_MEMSET( &property, 0x00, sizeof(NFBrowserInputURLHistoryProperty) );
    property.fFilename = (slim_char *)BRSCFG_URLHISTORY_FILENAME;
    property.fMaxItem = MMIWWW_INPUTHISTORY_URL_NUM_MAX;
    property.fMaxURLLen = MMIWWW_INPUTHISTORY_URL_LEN_MAX;

    bret = NFBrowser_NewInputURLHistory(self, &property);

    return bret;
}

/*****************************************************************************/
// Description : Generates input URL history instance
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC void MMIWWW_Netfront_DeleteInputURLHistory(void)
{
    slim_int self = MMIWWW_GETBROWSER;
    NFBrowser_DeleteInputURLHistory(self);
}

/*****************************************************************************/
// Description : Gets number of  input URL history entries
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_int MMIWWW_Netfront_GetInputURLHistoryNumber(void)
{
    slim_int ret = 0;
    slim_int self = MMIWWW_GETBROWSER;

    ret = NFBrowser_GetInputURLHistoryNumber(self);

    return ret;
}

/*****************************************************************************/
// Description : Gets URL of specified input URL history entry
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_GetInputURLHistoryUrl(slim_int in_index, MMI_STRING_T *out_url)
{
    NFBrowserMem url = {0};/*lint !e64*/
    wchar *wstr_ptr = PNULL;
    slim_bool bret = FALSE;
    slim_int self = MMIWWW_GETBROWSER;
    
    if (PNULL == out_url)
    {
        return slim_false;
    }

    SCI_MEMSET(&url, 0x00, sizeof(NFBrowserMem));

    bret = NFBrowser_GetInputURLHistoryUrl(self, in_index, &url);

    if (bret && url.fDataSize > 1)
    {
        wstr_ptr = SCI_ALLOCA(((uint32)url.fDataSize * sizeof(wchar)));
        if (PNULL == wstr_ptr)
        {
            return FALSE;
        }
        SCI_MEMSET(wstr_ptr, 0x00, (((uint32)url.fDataSize * sizeof(wchar))));
        out_url->wstr_len = GUI_UTF8ToWstr(wstr_ptr, (uint16)(url.fDataSize - 1), url.fData, (uint16)(url.fDataSize - 1));
        out_url->wstr_ptr = wstr_ptr;
    }
    NFBrowser_ReleaseSlimMem(&url);

    return TRUE;
}

/*****************************************************************************/
// Description : Deletes input URL history entry specified by in_index
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_RemoveInputURLHistory(slim_int in_index)
{
    
    slim_bool bret = FALSE;
    slim_int self = MMIWWW_GETBROWSER;

    bret = NFBrowser_RemoveInputURLHistory(self, in_index);

    return bret;
}

/*****************************************************************************/
// Description : Deletes all the input URL history entries
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_RemoveAllInputURLHistory(void)
{
    slim_bool bret = FALSE;
    slim_int self = MMIWWW_GETBROWSER;

    bret = NFBrowser_RemoveAllInputURLHistory(self);

    return bret;
}

/*****************************************************************************/
// Description : Saves new input URL
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_AddInputURLHistory(MMI_STRING_T *add_url_ptr)
{
    uint8 *url_ptr = PNULL;
    slim_bool bret = FALSE;
    uint16 str_len = 0;
    slim_int self = MMIWWW_GETBROWSER;

    str_len = (uint16)(add_url_ptr->wstr_len * 3 + 1);
    
    url_ptr = SCI_ALLOCA(str_len * sizeof(uint8));

    if (PNULL == url_ptr)
    {
        return FALSE;
    }

    SCI_MEMSET(url_ptr, 0x00, (str_len * sizeof(uint8)));

    GUI_WstrToUTF8(url_ptr, str_len, add_url_ptr->wstr_ptr, add_url_ptr->wstr_len);

    bret = NFBrowser_AddInputURLHistory(self, (slim_char *)url_ptr);

    SCI_FREE(url_ptr);
    url_ptr = PNULL;

    return bret;
}

/*****************************************************************************/
// Description : Access network by history
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC void MMIWWW_Netfront_History_GotoURL(slim_int in_index)
{
    NFBrowserMem url = {0};/*lint !e64*/
    BOOLEAN ret = FALSE;
    
    SCI_MEMSET(&url, 0x00, sizeof(NFBrowserMem));
    ret = (BOOLEAN)NFBrowser_GetVisitHistoryUrl(MMIWWW_GETBROWSER, in_index, &url);

    if (ret && url.fDataSize > 1)
    {
        MMIWWW_OpenWWWBrowserWin(BROWSER_START_BY_URL, (char *)url.fData);
    }
    NFBrowser_ReleaseSlimMem(&url);
}

/*****************************************************************************/
// Description : Get webpage title
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_GetPageTitle(MMI_STRING_T *out_title)
{
    NFBrowserMem title = {0};/*lint !e64*/
    wchar *wstr_ptr = PNULL;
    slim_bool bret = slim_false;
    slim_int self = MMIWWW_GETBROWSER;

    if (PNULL == out_title)
    {
        return slim_false;
    }

    SCI_MEMSET(&title, 0x00, sizeof(NFBrowserMem));
    bret = NFBrowser_GetPageTitle(self, &title);

    if (bret && title.fDataSize > 1)
    {
        wstr_ptr = SCI_ALLOCA(((uint32)title.fDataSize * sizeof(wchar)));
        if (PNULL == wstr_ptr)
        {
            return slim_false;
        }
        SCI_MEMSET(wstr_ptr, 0x00, (((uint32)title.fDataSize * sizeof(wchar))));
        out_title->wstr_len = GUI_UTF8ToWstr(wstr_ptr, (uint16)(title.fDataSize - 1), title.fData, (uint16)(title.fDataSize - 1));
        out_title->wstr_ptr = wstr_ptr;
    }
    NFBrowser_ReleaseSlimMem(&title);

    return bret;
}

/*****************************************************************************/
// Description : Get webpage title
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_GetPageTitleByIndex(int32 index, MMI_STRING_T *out_title)
{
    NFBrowserMem title = {0};/*lint !e64*/
    wchar *wstr_ptr = PNULL;
    slim_bool bret = slim_false;
    slim_int self = MMIWWW_GETBROWSER;

    if (PNULL == out_title)
    {
        return slim_false;
    }

    SCI_MEMSET(&title, 0x00, sizeof(NFBrowserMem));
    bret = NFBrowser_GetPageTitleByIndex(self, index, &title);

    if (bret && title.fDataSize > 1)
    {
        wstr_ptr = SCI_ALLOCA(((uint32)title.fDataSize * sizeof(wchar)));
        if (PNULL == wstr_ptr)
        {
            return slim_false;
        }
        SCI_MEMSET(wstr_ptr, 0x00, (((uint32)title.fDataSize * sizeof(wchar))));
        out_title->wstr_len = GUI_UTF8ToWstr(wstr_ptr, (uint16)(title.fDataSize - 1), title.fData, (uint16)(title.fDataSize - 1));
        out_title->wstr_ptr = wstr_ptr;
    }
    NFBrowser_ReleaseSlimMem(&title);

    return bret;
}

/*****************************************************************************/
// Description : Change size of all browsing windows
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_Resize(void)
{
    slim_int ret = 0;

    MMIWWW_PEER_DATA_T *peer_ptr = PNULL;
    TRect view_rect = {0};
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    BOOLEAN is_full = FALSE;
    uint16 screen_width = 0;
    uint16 screen_height = 0;


    if (PNULL == www_instance)
    {
        return ret;
    }

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&screen_width,&screen_height);

    is_full = MMIWWW_IsFullDisplay();
    //SCI_TRACE_LOW:"MMIWWW_Netfront_Resize is full=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_1995_112_2_18_3_11_46_103,(uint8*)"d", is_full);

    peer_ptr = &(www_instance->peer_data);

    peer_ptr->fX = 0;
    peer_ptr->fY = 0;
    peer_ptr->fScreenWidth = screen_width;
    peer_ptr->fScreenHeight = screen_height;

    if (!is_full)
    {
        peer_ptr->fY += MMI_STATUSBAR_HEIGHT + MMIWWW_PAGE_TITLE_HEIGHT;
#ifdef TOUCH_PANEL_SUPPORT
#ifdef MAINLCD_SIZE_320X480
        peer_ptr->fScreenHeight -= (MMI_STATUSBAR_HEIGHT + MMIWWW_PAGE_TITLE_HEIGHT + MMIWWW_PAGE_BUTTON_BG_HEIGHT);
#else
        peer_ptr->fScreenHeight -= (MMI_STATUSBAR_HEIGHT + MMIWWW_PAGE_TITLE_HEIGHT + MMIWWW_PAGE_BUTTON_HEIGHT);
#endif
#else
        peer_ptr->fScreenHeight -= (MMI_STATUSBAR_HEIGHT + MMIWWW_PAGE_TITLE_HEIGHT + MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT);
#endif
    }

    view_rect.fWidth  = peer_ptr->fScreenWidth;
    view_rect.fHeight = peer_ptr->fScreenHeight;

    ret = NFBrowser_Resize(MMIWWW_GETBROWSER, &view_rect);
    return ret;
}

/*****************************************************************************/
// Description : Gets number of entries in certificate database by certificate type
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_int MMIWWW_Netfront_GetX509CertNumber(slim_int in_type)
{
    slim_int ret = 0;
    slim_int self = MMIWWW_GETBROWSER;
    
    ret = NFBrowser_GetX509CertNumber(self, in_type);

    return ret;
}

/*****************************************************************************/
// Description : Gets name of issuer/subject
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_GetX509CertName(slim_int in_type, slim_int in_index, MMI_STRING_T *out_name)
{
    NFBrowserMem name = {0};/*lint !e64*/
    wchar *wstr_ptr = PNULL;
    slim_bool bret = FALSE;
    slim_int self = MMIWWW_GETBROWSER;

    if (PNULL == out_name)
    {
        return slim_false;
    }

    SCI_MEMSET(&name, 0x00, sizeof(NFBrowserMem));

    bret = NFBrowser_GetX509CertName(self, in_type, in_index, &name);

    if (bret && name.fDataSize > 1)
    {
        wstr_ptr = SCI_ALLOCA(((uint32)name.fDataSize * sizeof(wchar)));
        if (PNULL == wstr_ptr)
        {
            return slim_false;
        }
        SCI_MEMSET(wstr_ptr, 0x00, (((uint32)name.fDataSize * sizeof(wchar))));
        out_name->wstr_len = GUI_UTF8ToWstr(wstr_ptr, (uint16)(name.fDataSize -1),
                                            name.fData, (uint16)(name.fDataSize - 1));
        out_name->wstr_ptr = wstr_ptr;
    }
    NFBrowser_ReleaseSlimMem(&name);

    return slim_true;
}

/*****************************************************************************/
// Description : Gets certificate data
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC NFBrowserCertInfo *MMIWWW_Netfront_GetX509CertData(slim_int in_type, slim_int in_index)
{
    NFBrowserCertInfo *CertInfo_ptr = PNULL;
    slim_int self = MMIWWW_GETBROWSER;
    
    CertInfo_ptr = NFBrowser_GetX509CertData(self, in_type, in_index);

    return CertInfo_ptr;
}

/*****************************************************************************/
// Description : Releases certificate data information
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/ 
PUBLIC void MMIWWW_Netfront_DeleteX509CertData(NFBrowserCertInfo *CertInfo_ptr)
{
    NFBrowser_DeleteX509CertData(CertInfo_ptr);
    return;
}

/*****************************************************************************/
// Description : Send HTTP Get request to specified URL.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_SetLocation(slim_char *in_url)
{
    slim_int    ret = SLIM_E_OK;
    if (PNULL != in_url)
    {
        ret = NFBrowser_WindowSetLocation(MMIWWW_GETBROWSER, in_url);
        //SCI_TRACE_LOW:"MMIWWW_Netfront_SetLocation ret=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_2110_112_2_18_3_11_46_104,(uint8*)"d", ret);
    }
    return ret;
}

/*****************************************************************************/
// Description : Get number of browsing windows.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_GetWinNum(void)
{
    return NFBrowser_WindowNum(MMIWWW_GETBROWSER);
}

/*****************************************************************************/
// Description : Delete a browsing window.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_DelWin(slim_int  in_index)
{
    return NFBrowser_DeleteWindow(MMIWWW_GETBROWSER, in_index);
}

/*****************************************************************************/
// Description : Create a new browsing window.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_NewWin(slim_int  *out_index)
{
    if (PNULL == out_index)
    {
        return SLIM_E_GENERIC;
    }
    
    return NFBrowser_NewWindow(MMIWWW_GETBROWSER, out_index);
}

/*****************************************************************************/
// Description : Get ID of active browsing window.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_GetActWinIndex(void)
{
    return NFBrowser_ActiveWindow(MMIWWW_GETBROWSER);
}

/*****************************************************************************/
// Description : Activate a specified ID browsing window.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_ActWinByIndex(slim_int  in_index)
{
    return NFBrowser_ActivateNthWindow(MMIWWW_GETBROWSER, in_index);
}

/*****************************************************************************/
// Description : Get current background sound information.
// Global resource dependence :
// Author: jiaoyou.wu
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_GetBGSoundInfo(slim_int in_index, slim_int in_snd_id, NFBrowserBGSoundInfo *out_info)
{
    return NFBrowser_GetBGSoundInfo(MMIWWW_GETBROWSER, in_index, in_snd_id, out_info);
}

/*****************************************************************************/
// Description : Get current page size.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_GetPageSize(void)
{
    slim_int    size = 0;

    size = NFBrowser_GetPageSize(MMIWWW_GETBROWSER);
    if (size < 0) size = 0;
    return size;
}

/*****************************************************************************/
// Description : Get page MIME type.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_GetPageMIMEType(MMI_STRING_T  *out_mimetype)
{
    slim_bool        bret = slim_true;
    NFBrowserMem     mimetype = {0};/*lint !e64*/
    wchar           *wstr_ptr = PNULL;

    if (PNULL == out_mimetype)
    {
        return slim_false;
    }

    bret = NFBrowser_GetPageMIMEType(MMIWWW_GETBROWSER, &mimetype);

    if (bret && mimetype.fDataSize > 1)
    {
        wstr_ptr = SCI_ALLOCA(((uint32)mimetype.fDataSize * sizeof(wchar)));
        if (PNULL == wstr_ptr)
        {
            return slim_false;
        }
        SCI_MEMSET(wstr_ptr, 0x00, (((uint32)mimetype.fDataSize * sizeof(wchar))));
        out_mimetype->wstr_len = GUI_UTF8ToWstr(wstr_ptr, (uint16)(mimetype.fDataSize - 1),
                                               mimetype.fData, (uint16)(mimetype.fDataSize - 1));
        out_mimetype->wstr_ptr = wstr_ptr;
    }
    NFBrowser_ReleaseSlimMem(&mimetype);
    return bret;
}

/*****************************************************************************/
// Description : Get webpage address.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_GetPageAddress(MMI_STRING_T  *out_address)
{
    slim_bool        bret = slim_true;
    NFBrowserMem     address = {0};/*lint !e64*/
    wchar           *wstr_ptr = PNULL;

    if (PNULL == out_address)
    {
        return slim_false;
    }

    bret = NFBrowser_GetPageAddress(MMIWWW_GETBROWSER, &address);

    if (bret && address.fDataSize > 1)
    {
        wstr_ptr = SCI_ALLOCA(((uint32)address.fDataSize * sizeof(wchar)));
        if (PNULL == wstr_ptr)
        {
            return slim_false;
        }
        SCI_MEMSET(wstr_ptr, 0x00, (((uint32)address.fDataSize * sizeof(wchar))));
        out_address->wstr_len = GUI_UTF8ToWstr(wstr_ptr, (uint16)(address.fDataSize - 1),
                                               address.fData, (uint16)(address.fDataSize - 1));
        out_address->wstr_ptr = wstr_ptr;
    }
    NFBrowser_ReleaseSlimMem(&address);
    return bret;
}

/*****************************************************************************/
// Description : Get webpage address.
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_GetPageAddressByIndex(int32 index, MMI_STRING_T  *out_address)
{
    slim_bool        bret = slim_true;
    NFBrowserMem     address = {0};/*lint !e64*/
    wchar           *wstr_ptr = PNULL;

    if (PNULL == out_address)
    {
        return slim_false;
    }

    bret = NFBrowser_GetPageAddressByIndex(MMIWWW_GETBROWSER, index, &address);

    if (bret && address.fDataSize > 1)
    {
        wstr_ptr = SCI_ALLOCA(((uint32)address.fDataSize * sizeof(wchar)));
        if (PNULL == wstr_ptr)
        {
            return slim_false;
        }
        SCI_MEMSET(wstr_ptr, 0x00, (((uint32)address.fDataSize * sizeof(wchar))));
        out_address->wstr_len = GUI_UTF8ToWstr(wstr_ptr, (uint16)(address.fDataSize - 1),
                                               address.fData, (uint16)(address.fDataSize - 1));
        out_address->wstr_ptr = wstr_ptr;
    }
    NFBrowser_ReleaseSlimMem(&address);
    return bret;
}

/*****************************************************************************/
// Description : Set NF browser to save form data of webpages
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_SetWebData(slim_bool in_save)
{
//    NFBrowser_SetSaveWebData(MMIWWW_GETBROWSER, in_save);
}

/*****************************************************************************/
// Description : Set NF browser to save form data of webpages
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_ClearWebData(void)
{
 //   NFBrowser_ClearWebData(MMIWWW_GETBROWSER);
}

/*****************************************************************************/
// Description : Set script mode of NF browser engine in actvie browsing window.
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_SetScriptMode(slim_int in_mode)
{
    NFBrowser_SetScriptMode(MMIWWW_GETBROWSER, in_mode);
    return;
}

/*****************************************************************************/
// Description : Set NF browser engine to display animation image in actvie browsing window.
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_SetDisplayAnimateImage(slim_bool in_display)
{
    NFBrowser_SetDisplayAnimateImage(MMIWWW_GETBROWSER, in_display);
    return;
}

/*****************************************************************************/
// Description : Set image displaying mode of NF browser engine in actvie browsing window.
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_SetImageDisplayMode(slim_int in_mode)
{
    MMIWWW_SetImageDisplayMode((NFBrowserImageMode)in_mode);

    NFBrowser_SetImageDisplayMode(MMIWWW_GETBROWSER, in_mode);
    return;
}

/*****************************************************************************/
// Description : Set NF browser engine to display tables in actvie browsing window.
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_SetDisplayTable(slim_bool in_display)
{
    NFBrowser_SetDisplayTable(MMIWWW_GETBROWSER, in_display);
    return;
}

/*****************************************************************************/
// Description : Enable background soud of webpages in actvie browsing window.
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_SetEnableBGSound(slim_bool in_enable)
{
    NFBrowser_SetEnableBGSound(MMIWWW_GETBROWSER, in_enable);
    return;
}


/*****************************************************************************/
// Description : Get image number in current page.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_GetImageNumber(void)
{
    slim_int    num = 0;

    num = NFBrowser_GetImageNumber(MMIWWW_GETBROWSER);
    if (num < 0) num = 0;
    return num;
}

/*****************************************************************************/
// Description : Set browser image selection mode and select one image before
//               get the image information.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_SetImageSelectMode(slim_int  is_select)
{
    slim_bool    bret = slim_false;

    bret = NFBrowser_SetImageSelectMode(MMIWWW_GETBROWSER, is_select);
    return bret;
}

/*****************************************************************************/
// Description : Determine image saving mode.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_IsImageSelectMode(void)
{
    slim_bool    bret = slim_false;
    slim_int     active = 0;
    slim_int     self = MMIWWW_GETBROWSER;

    active = NFBrowser_ActiveWindow(self);
    bret = NFBrowser_WindowIsImageSelectMode(self, active);
    return bret;
}

/*****************************************************************************/
// Description : Get size information of the selected image in the current page.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_Netfront_GetSelectedImageSize(void)
{
    slim_int    size = 0;

    size = NFBrowser_GetSelectedImageSize(MMIWWW_GETBROWSER);
    if (size < 0) size = 0;
    return size;
}

/*****************************************************************************/
// Description : Get selected image name.
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_GetImageName(MMI_STRING_T  *out_name)
{
    slim_bool        bret = slim_true;
    NFBrowserMem     name = {0};/*lint !e64*/
    wchar           *wstr_ptr = PNULL;

    if (PNULL == out_name)
    {
        return slim_false;
    }

    bret = NFBrowser_GetSelectedImageName(MMIWWW_GETBROWSER, &name);

    if (bret && name.fDataSize > 1)
    {
        wstr_ptr = SCI_ALLOCA(((uint32)name.fDataSize * sizeof(wchar)));
        if (PNULL == wstr_ptr)
        {
            return slim_false;
        }
        SCI_MEMSET(wstr_ptr, 0x00, (((uint32)name.fDataSize * sizeof(wchar))));
        out_name->wstr_len = GUI_UTF8ToWstr(wstr_ptr, (uint16)(name.fDataSize - 1),
                                            name.fData, (uint16)(name.fDataSize - 1));
        out_name->wstr_ptr = wstr_ptr;
    }
    NFBrowser_ReleaseSlimMem(&name);
    return bret;
}

/*****************************************************************************/
// Description : Get address information of the selected image.
// Global resource dependence :
// Author: Bill.Ji
// Note: 需调用者对返回数据进行释放
/*****************************************************************************/
PUBLIC uint8 *MMIWWW_Netfront_GetImageAddress(void)
{
    slim_bool       bret = slim_true;
    NFBrowserMem    address = {0};/*lint !e64*/
    uint32          len = 0;
    uint8          *url_ptr = PNULL;

    bret = NFBrowser_GetSelectedImageAddress(MMIWWW_GETBROWSER, &address);
    if (bret && PNULL != address.fData && address.fDataSize > 0)
    {
        len = ((uint32)address.fDataSize * sizeof(uint8) + 1);
        url_ptr = SCI_ALLOCA(len);
        if (PNULL != url_ptr)
        {
            SCI_MEMSET(url_ptr, 0x00, len);
            SCI_MEMCPY(url_ptr, address.fData, len-1);
        }
        else
        {
            //SCI_TRACE_LOW:"MMIWWW_Netfront_GetImageAddress Alloc failed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_2467_112_2_18_3_11_47_105,(uint8*)"");
        }
        NFBrowser_ReleaseSlimMem(&address);
    }
    else
    {
        //SCI_TRACE_LOW:"MMIWWW_Netfront_GetImageAddress Error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_2473_112_2_18_3_11_47_106,(uint8*)"");
    }
    return url_ptr;
}

/*****************************************************************************/
// Description : Get selected image data.
// Global resource dependence :
// Author: Bill.Ji
// Note: 需调用者对返回数据进行释放
/*****************************************************************************/
PUBLIC uint32 MMIWWW_Netfront_GetImageData(uint8  **out_ptr)
{
    uint8          *image_ptr = PNULL;
    uint32          len = 0;
    slim_int        ret = slim_true;
    NFBrowserMem    data = {0};/*lint !e64*/

    if (PNULL == out_ptr)
    {
        return len;
    }

    ret = NFBrowser_GetSelectedImageData(MMIWWW_GETBROWSER, &data);
    if (SLIM_E_OK == ret && PNULL != data.fData && data.fDataSize > 0)
    {
        //SCI_TRACE_LOW:"MMIWWW_Netfront_GetImageData SUCC size = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_2497_112_2_18_3_11_47_107,(uint8*)"d",data.fDataSize);
        len = ((uint32)data.fDataSize * sizeof(uint8));
        image_ptr = SCI_ALLOCA(len);
        if (PNULL != image_ptr)
        {
            SCI_MEMSET(image_ptr, 0x00, len);
            SCI_MEMCPY(image_ptr, data.fData, len);
            *out_ptr = image_ptr;
        }
        else
        {
            len = 0;
            //SCI_TRACE_LOW:"MMIWWW_Netfront_GetImageData Alloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_2509_112_2_18_3_11_47_108,(uint8*)"");
        }
        NFBrowser_ReleaseSlimMem(&data);
    }
    else
    {
        //SCI_TRACE_LOW:"MMIWWW_Netfront_GetImageData failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_2515_112_2_18_3_11_47_109,(uint8*)"");
    }
    return len;
}

/*****************************************************************************/
// Description : Get selected image data.
// Global resource dependence :
// Author: Bill.Ji
// Note: 需调用者对返回数据进行释放
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_GetImageContentType(uint8  **content_type_ptr)
{
    uint32 len = 0;
    slim_int ret = slim_true;
    uint8 *temp_ptr = PNULL;
    NFBrowserMem mime_type = {0};/*lint !e64*/

    ret = NFBrowser_GetSelectedImageMIMEType(MMIWWW_GETBROWSER, &mime_type);
    if (ret && PNULL != mime_type.fData && mime_type.fDataSize > 1)
    {
        len = ((uint32)mime_type.fDataSize * sizeof(uint8));
        temp_ptr = SCI_ALLOCA(len);
        if (PNULL != temp_ptr)
        {
            SCI_MEMSET(temp_ptr, 0x00, len);
            SCI_MEMCPY(temp_ptr, mime_type.fData, len - 1);
            *content_type_ptr = temp_ptr;
            ret = slim_true;
        }
        else
        {
            //SCI_TRACE_LOW:"MMIWWW_Netfront_GetImageContentType Alloc failed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_2545_112_2_18_3_11_47_110,(uint8*)"");
            ret = slim_false;
        }
        NFBrowser_ReleaseSlimMem(&mime_type);
    }
    else
    {
        //SCI_TRACE_LOW:"MMIWWW_Netfront_GetImageContentType Error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_2552_112_2_18_3_11_47_111,(uint8*)"");
        ret = slim_false;
    }
    return ret;
}

/*****************************************************************************/
// Description : Sets the full path of the selected file to NF browser engine.
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_SetFileSelectFullPath(slim_int in_select, slim_char *in_path)
{
    slim_bool bret = FALSE;
    
    bret = NFBrowser_SetFileSelectFullPath(in_select, in_path);

    return bret;   
}

/*****************************************************************************/
// Description : Sets the full path of the selected file display to NF browser engine.
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/ 
PUBLIC slim_bool MMIWWW_Netfront_SetFileSelcetDisplayName(slim_int in_select, slim_char *in_name)
{
    slim_bool bret = FALSE;

    bret = NFBrowser_SetFileSelcetDisplayName(in_select, in_name);

    return bret;
}

/*****************************************************************************/
// Description : Wakeup Netfront Browser
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_Wakeup(slim_int in_slef)
{
    if (0 != in_slef)
    {
        NFBrowser_WakeUp(in_slef);
    }
}

/*****************************************************************************/
// Description : Reply Request Auth
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
PUBLIC slim_int MMIWWW_ReplyRequestAuth(slim_bool in_confirmed, MMI_STRING_T *in_user, MMI_STRING_T *in_pass)
{
    slim_char *user_ptr = PNULL;
    slim_bool is_user_alloc = FALSE;
    slim_char *pass_ptr = PNULL;
    slim_bool is_pass_alloc = FALSE;
    slim_char null_input[2] = {0};
    slim_int reply_ret = SLIM_E_OK;

    if (PNULL != in_user && PNULL != in_user->wstr_ptr && 0 != in_user->wstr_len)
    {
        user_ptr = SCI_ALLOCA(in_user->wstr_len * 3 + 1);

        if (PNULL == user_ptr)
        {
            return SLIM_E_GENERIC;
        }

        is_user_alloc = TRUE;

        SCI_MEMSET(user_ptr, 0x00, (in_user->wstr_len * 3 + 1));

        GUI_WstrToUTF8(user_ptr, (in_user->wstr_len * 3 + 1), in_user->wstr_ptr, in_user->wstr_len);
    }
    else
    {
        user_ptr = (slim_char *)null_input;
    }

    if (PNULL != in_pass && PNULL != in_pass->wstr_ptr && 0 != in_pass->wstr_len)
    {
        pass_ptr = SCI_ALLOCA(in_pass->wstr_len * 3 + 1);

        if (PNULL == pass_ptr)
        {
            if (is_user_alloc)
            {
                SCI_FREE(user_ptr);
            }
            return SLIM_E_GENERIC;
        }

        is_pass_alloc = TRUE;

        SCI_MEMSET(pass_ptr, 0x00, (in_pass->wstr_len * 3 + 1));

        GUI_WstrToUTF8(pass_ptr, (in_pass->wstr_len * 3 + 1), in_pass->wstr_ptr, in_pass->wstr_len);
    }
    else
    {
        pass_ptr = (slim_char *)null_input;
    }

    reply_ret = NFBrowser_ReplyRequestAuth(MMIWWW_GETBROWSER, in_confirmed, user_ptr, pass_ptr, TRUE, FALSE);

    if (is_user_alloc)
    {
        SCI_FREE(user_ptr);
    }

    if (is_pass_alloc)
    {
        SCI_FREE(pass_ptr);
    }

    //SCI_TRACE_LOW:"MMIWWW_ReplyRequestAuth reply_ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_NETFRONT_2665_112_2_18_3_11_47_112,(uint8*)"d", reply_ret);

    return reply_ret;
}

/*****************************************************************************/
// Description : Set Pointer Mode
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_SetPointerMode(slim_int pointer_mode)
{
    slim_int self = MMIWWW_GETBROWSER;

    if (0 < self)
    {
        NFBrowser_SetPointerMode(self, pointer_mode);
    }
}

/*****************************************************************************/
// Description : Enter copy mode
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_EnterCopyMode(void)
{
#ifdef TOUCH_PANEL_SUPPORT
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
//        NFBrowser_EnterCopyMode(www_instance->browser_instance);
        MMIWWW_SetCopyMode(MMIWWW_COPY_MODE_CHOSE_START);
    }
#endif /* TOUCH_PANEL_SUPPORT */
}

/*****************************************************************************/
// Description : Leave copy mode
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_LeaveCopyMode(void)
{
#ifdef TOUCH_PANEL_SUPPORT
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
//        NFBrowser_LeaveCopyMode(www_instance->browser_instance);
        MMIWWW_SetCopyMode(MMIWWW_COPY_MODE_NONE);
    }
#endif /* TOUCH_PANEL_SUPPORT */
}

/*****************************************************************************/
// Description : PageUp
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_PageUp(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        NFBrowser_Page_Up_Or_Down(www_instance->browser_instance, SLIM_PAGE_UP);
    }
}

/*****************************************************************************/
// Description : PageDown
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_PageDown(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        NFBrowser_Page_Up_Or_Down(www_instance->browser_instance, SLIM_PAGE_DOWN);
    }
}

#endif //#ifdef BROWSER_SUPPORT_NETFRONT


/*Edit by script, ignore 3 case. Thu Apr 26 19:01:12 2012*/ //IGNORE9527
