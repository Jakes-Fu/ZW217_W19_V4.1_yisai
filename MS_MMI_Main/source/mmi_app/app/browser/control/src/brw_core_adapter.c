/***********************************************************************************
说明:
     部分函数是浏览器内核调用和平台的GUI等相关的函数的包装。
***********************************************************************************/
#include "mmi_app_browser_trc.h"
#include "caf.h"
#include "guilcd.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmitheme_menu.h"
#include "guifont.h"
#include "guilcd.h"
#include "guifont.h"
#include "jpeg_interface.h"
#include "mmk_app.h"
#include "mmi_appmsg.h"
#include "window_parse.h"
#include "guiedit.h"
#include "guilistbox.h"
#include "mmi_text.h"
#include "mmi_modu_main.h"
#include "mmidisplay_data.h"
#include "guiprgbox.h"
#include "mmi_position.h"
#include "mmk_timer.h"
#include "image_proc.h"
#include "brw_plinterface.h"
#include "brw_control_api.h"
#include "brw_core_adapter.h"
#include "guistring.h"
//#include "spml_api.h"
#include "mmibrowser_id.h"
#include "guiimg.h"
#include "img_dec_interface.h"
#include "mmifilearray_export.h"
#include "mmifmm_export.h"
#include "gui_ucs2b_converter.h"
#include "mmi_custom_define.h"

#include "graphics_draw.h"//MS00200754
#include "browser_text.h"
#include "watchdog.h"
#ifdef JS_SUPPORT
#include "brw_core_api.h"
#endif
 /**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define BRW_IMAGE_DECODE_BUFFER_SIZE    (204800)
#define BRW_INPUT_MAX_LEN               1024  //输入框允许输入的byte数
#define BRW_IMAGE_ALT_FONT_SIZE          12 
#define BRW_FAIL_IMAGE_HEIGHT_MIN        14
#define BRW_FAIL_IMAGE_WIDTH_MIN         11

#ifdef MMIDORADO_BGSOUND_SUPPORT
#define BRW_BGSOUND_INFINITE 0xFFFF
#endif

//begin MS00200754
#define ZOOM_ALIGN(x,align)     (x/align * align)
//zoom in param
typedef struct 
{
    uint8           *src_buf_ptr;       //src buffer
    uint32          src_buf_size;       //src buffer size
    uint8           *dest_buf_ptr;      //dest buffer
    uint32          dest_buf_size;      //dest buffer size

    uint16          src_img_width;      //src image width
    uint16          src_img_height;     //src image height
    uint16          dest_img_width;     //dest image width
    uint16          dest_img_height;    //dest image height
    //GUI_RECT_T      clip_rect;          //clip rect
} ZOOM_INPUT_T;

//scale out param
typedef struct 
{
    uint16      img_width;      //image width
    uint16      img_height;     //image height
    uint32      dest_buf_offset;//dest buffer必须256对齐
} ZOOM_OUTPUT_T;

typedef enum
{
    ZOOM_RESULT_SUCC,                    /*!<成功*/
    ZOOM_RESULT_FAIL,                    /*!<失败*/
    ZOOM_RESULT_NO_SUPPORT,              /*!<不支持*/
    ZOOM_RESULT_MAX                      /*!<保留位*/
} ZOOM_RESULT_E;
//end MS00200754

#ifdef JS_SUPPORT
LOCAL int32 s_confirm_result = BRW_CONFIRM_NONE;
LOCAL  SWDG_OBJECT *s_watchdog_ptr = PNULL;
#endif

LOCAL MMISRV_HANDLE_T s_brw_play_handle = 0;

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

LOCAL uint16* s_select_path_ptr = PNULL;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : play gif
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
BOOLEAN BrwPlayGif(uint8 *data_ptr, uint32 data_len, IMAGE_PLAY_INFO *info_ptr, int16 x, int16 y, GUI_RECT_T *rect_ptr);

/*****************************************************************************/
//  Description : zoom in or out image
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL ZOOM_RESULT_E BrwZoomImage(
                      ZOOM_INPUT_T  *zoom_in_ptr,   //in:
                      ZOOM_OUTPUT_T *zoom_out_ptr   //in/out:
                      );

/*****************************************************************************/
//  Description : open form file win callback
//  Global resource dependence : none
//  Author: li.li
//  Note:
/*****************************************************************************/
LOCAL void XhtmlOpenFormFileWinCallBack(
                                      BOOLEAN   is_success, 
                                      FILEARRAY file_array
                                      );

#ifdef MMIDORADO_BGSOUND_SUPPORT
/*****************************************************************************/
// 	Description : Audio call back function called by audio service.
//	Global resource dependence : 
//   Author: 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleBrwAudioCallBack(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param);

/*****************************************************************************/
//  Description : Request BG play sevice.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN BRW_ReqPlaySrv(MMISRVAUD_RING_FMT_E audio_type,
                                                                uint8 *data_ptr,
                                                                uint32 data_len,
                                                                uint32 play_times);
#endif

#ifdef JS_SUPPORT
/*****************************************************************************/
//  Description : handle confirm window message
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL  MMI_RESULT_E BrwXhtmlLayoutHandlePromptWinMsg(
                                    uint32 win_id, 
                                    MMI_MESSAGE_ID_E msg_id,
                                    DPARAM param);
/*****************************************************************************/
//  Description : handle confirm window message
//  Global resource dependence : 
//  Author:jiawang.yu
//  Note: 
/*****************************************************************************/
LOCAL void WaitUserConfirm(void);

/*****************************************************************************/
//  Description : handle confirm window message
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleConfirmWinMsg(
                                    uint32 win_id, 
                                    MMI_MESSAGE_ID_E msg_id,
                                    DPARAM param);

/**--------------------------------------------------------------------------*
 **                         WINDOW DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : open an alert window for display:str_ptr
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
WINDOW_TABLE(MMIBROWSER_PROMPT_WIN_TAB) = 
{
    WIN_TITLE(TXT_EDIT),
    WIN_FUNC((uint32)BrwXhtmlLayoutHandlePromptWinMsg),    
    WIN_ID(MMIBROWSER_QUERY_INPUT_WIN_ID),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIBROWSER_PROMPT_FORM_CTRL_ID),    
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIBROWSER_PROMPT_LABEL_CTRL_ID,MMIBROWSER_PROMPT_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,100,MMIBROWSER_PROMPT_EDIT_CTRL_ID,MMIBROWSER_PROMPT_FORM_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),              
    END_WIN
};
#endif //JS_SUPPORT
/*****************************************************************************/
//  Description : convert font size to mmi
//  Global resource dependence : none
//  Author: wenmei.hua
//  Note:
/*****************************************************************************/
LOCAL uint32 ConvertWapFontSizeToMMISize( uint32 font_type )
{
#ifndef FONT_TYPE_SUPPORT_VECTOR
    if (font_type < MMI_DEFAULT_NORMAL_FONT_SIZE)
    {
        font_type = MMI_DEFAULT_SMALL_FONT_SIZE;
    }
    else if(font_type >= MMI_DEFAULT_BIG_FONT_SIZE)
    {
        font_type = MMI_DEFAULT_BIG_FONT_SIZE;
    }
    else
    {
        font_type = MMI_DEFAULT_NORMAL_FONT_SIZE;
    }
#else
    uint32 small_font = 0;
    uint32 big_font   = 0;

    big_font   = (uint32)(float)(MMIBROWSER_BIG_FONT*1.2);
    small_font = (uint32)(float)(MMIBROWSER_SMALL_FONT/1.2);

    if (font_type > big_font)
    {
        font_type = big_font;
    }
    else if (font_type < small_font)
    {
        font_type = small_font;
    }
#endif
    return font_type;
}
/*****************************************************************************/
//  Description : start a timer, one layout can create only one timer.
//  note:         only used by layout
/*****************************************************************************/
uint32 BrwTimer_StartTimer(BRW_WIN_HANDLE win_id, uint32 time_out)
{
    uint32 timer_id = MMI_NONE_TIMER_ID;

    if(win_id==0 || MMI_INVALID_ID==win_id) {
        return MMI_NONE_TIMER_ID;
    }

    if(time_out > 0) {
         timer_id = MMK_CreateWinTimer((MMI_HANDLE_T)win_id, time_out, FALSE);
         //BRWDebug_Printf:"Brw: timer=%dms start id=%d\n"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_221_112_2_18_2_7_2_508,(uint8*)"dd", time_out, timer_id);
    }
    return timer_id;
}

/******************************************************************************************
* DESCRIPTION:
*   stop a timer, called by layout
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   timer_id: [IN] timer id
*
* RETURN:
*   TRUE if timer id is not invalid
*
* NOTE:
******************************************************************************************/
BOOLEAN BrwTimer_StopTimer(uint32 timer_id)
{
    if(MMI_NONE_TIMER_ID != timer_id) {
        MMK_StopTimer((uint8)timer_id);
        //BRWDebug_Printf:"Brw: timer id=%d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_244_112_2_18_2_7_2_509,(uint8*)"d", timer_id);
        return TRUE;
    }
    return FALSE;
}
//return invalid timer id macro
uint32 BrwTimer_InvalidTimerId(void)
{
    return MMI_NONE_TIMER_ID;
}

/*****************************************************************************/
//  Description : open an alert window for display:Invalid input
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void BrwDisplayInvalidInput(void)
{
      MMIPUB_OpenAlertWarningWin(TXT_MMS_INVALID_INPUT);

}

#ifdef JS_SUPPORT
/*****************************************************************************/
//  Description : handle confirm window message
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN IsWaitUserConfirm(void)
{
    return s_confirm_result == BRW_CONFIRM_WAIT;
}

/*****************************************************************************/
//  Description : handle confirm window message
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void CancelWaitUserConfirm(void)
{
    s_confirm_result = BRW_PAGE_DESTROYED;
}

/*****************************************************************************/
//  Description : handle confirm window message
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleConfirmWinMsg(
                                    uint32 win_id, 
                                    MMI_MESSAGE_ID_E msg_id,
                                    DPARAM param)
{
    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        s_confirm_result = BRW_CONFIRM_WAIT;
        break; 
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:
		s_confirm_result = BRW_CONFIRM_OK;
		MMK_CloseWin(win_id);
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
#ifdef MMI_PDA_SUPPORT
	case MSG_KEYUP_RED:
#else		
	case MSG_APP_RED:
#endif
		s_confirm_result = BRW_CONFIRM_CANCEL;
		MMK_CloseWin(win_id);
		break;
    default:
		MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

	return TRUE;
}

LOCAL void WaitUserConfirm()
{
    MmiSignalS*        recv_sig ;
    MMI_MESSAGE_PTR_T  mmi_msg;
#ifndef WIN32
	SWDG_OBJECT     *watchdog_ptr = s_watchdog_ptr;  //监测mmi task出现死循环
    //SCI_TRACE_LOW:"APP_Task Brw confirm WaitUserConfirm : begin init, time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_323_112_2_18_2_7_2_510,(uint8*)"d", SCI_GetTickCount());
#endif
    s_confirm_result = BRW_CONFIRM_WAIT;
    //wait for user response
    while( TRUE )/*lint !e716*/
    {
		if(BRW_PAGE_DESTROYED == s_confirm_result)
		    break;

        if (MMK_GetMSGQueue(&mmi_msg)) {
            MMK_DispatchMSGQueue(mmi_msg);

            MMITHEME_UpdateRect();
            //ChangeFreqByQuickKey();
            MMK_FreeMSG(mmi_msg);
        }
        else {
            MmiReceiveSignal(P_APP, &recv_sig); 
#ifndef WIN32
            /* Fresh task tick */	
            SWDG_FreshTick(watchdog_ptr);
            /* Set task status running */
            SWDG_SetStatus(watchdog_ptr, SCI_TRUE);
#endif
			MMK_DispatchExtSig(&recv_sig);

			MMITHEME_UpdateRect();
            //ChangeFreqByQuickKey();
            MmiDestroySignal(&recv_sig);
        }

		if(BRW_CONFIRM_WAIT != s_confirm_result)
		    break;
    }
}

/*****************************************************************************/
//  Description : open an confirm window for display:str_ptr
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN BrwXhtmlLayoutOpenConfirmWin(const uint16* str_ptr)
{
    MMI_STRING_T mstr   = {0};

	if(BRW_PAGE_DESTROYED == s_confirm_result)
		return FALSE;

    mstr.wstr_ptr = (wchar*)str_ptr;
    mstr.wstr_len = BRW_WStrlen(str_ptr);
    MMIPUB_OpenQueryWinByTextPtr(&mstr, PNULL, IMAGE_PUBWIN_QUERY, PNULL,BrwHandleConfirmWinMsg); 

    WaitUserConfirm();

	if(BRW_CONFIRM_OK == s_confirm_result)
		return TRUE;
	else
		return FALSE;
}

/*****************************************************************************/
//  Description : open an prompt window for user input
//  Global resource dependence : 
// param 1: str_ptr: display in dialog
// param 2: default_str : diapaly in input
//param 3: user_data
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN BrwXhtmlLayoutOpenPromptWin(const uint16* str_ptr, const uint16 *default_str, uint16 *user_data_ptr)
{
    MMI_STRING_T text_str = {0};

	if(BRW_PAGE_DESTROYED == s_confirm_result)
		return FALSE;
    #ifdef MMI_PDA_SUPPORT
        MMK_CreatePubFormWin((uint32 *)MMIBROWSER_PROMPT_WIN_TAB, user_data_ptr);
    #else
        MMK_CreateWin((uint32 *)MMIBROWSER_PROMPT_WIN_TAB, user_data_ptr);
    #endif
        text_str.wstr_ptr = (wchar *)str_ptr;
        text_str.wstr_len  = MMIAPICOM_Wstrlen(str_ptr);    
        GUILABEL_SetText(MMIBROWSER_PROMPT_LABEL_CTRL_ID, &text_str, FALSE);
        GUIEDIT_SetString(MMIBROWSER_PROMPT_EDIT_CTRL_ID,(wchar*)default_str, MMIAPICOM_Wstrlen(default_str));
        WaitUserConfirm();

	if(BRW_CONFIRM_OK == s_confirm_result)
		return TRUE;
	else
		return FALSE;
}

/*****************************************************************************/
//  Description : handle prompt window message
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL  MMI_RESULT_E BrwXhtmlLayoutHandlePromptWinMsg(
                                                     uint32 win_id, 
                                                     MMI_MESSAGE_ID_E msg_id,
                                                     DPARAM param)
{
    MMI_RESULT_E     ret        = MMI_RESULT_TRUE;    
    MMI_CTRL_ID_T    ctrl_id    = MMIBROWSER_PROMPT_EDIT_CTRL_ID;
    uint16           *input_ptr = PNULL;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_confirm_result = BRW_CONFIRM_WAIT;
        input_ptr = MMK_GetWinAddDataPtr(win_id);
        if(PNULL == input_ptr)
        {
            //SCI_TRACE_LOW:"HandleWmlInputBoxWinMsg ::PNULL ==input_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_431_112_2_18_2_7_3_511,(uint8*)"");
            break;
        }

        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CLOSE_WINDOW:        
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
#ifdef MMI_PDA_SUPPORT
    case MSG_APP_MENU:
#endif
        {  
            MMI_STRING_T string_info = {0};
            int32        len         = 0;
            s_confirm_result = BRW_CONFIRM_OK;
            input_ptr = MMK_GetWinAddDataPtr(win_id);
            if(PNULL == input_ptr)
            {
                //SCI_TRACE_LOW:"HandleWmlInputBoxWinMsg ::PNULL ==input_ptr"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_453_112_2_18_2_7_3_512,(uint8*)"");
                break;
            }
            
          GUIEDIT_GetString(ctrl_id,&string_info);
          len = 2* BRW_WStrlen(string_info.wstr_ptr);
          len = MIN(100, len);
          BRW_Memcpy(input_ptr, (void*)string_info.wstr_ptr,len);
          *(input_ptr+len) = 0x0000;

        }
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL: 
#ifdef MMI_PDA_SUPPORT
	case MSG_KEYUP_RED:
#else		
	case MSG_APP_RED:
#endif
		s_confirm_result = BRW_CONFIRM_CANCEL;
        MMK_CloseWin(win_id);
        break;

    default:
        ret = MMI_RESULT_FALSE;
        break;
    }

    return ret;
}

/*****************************************************************************/
//  Description : handle alert window message
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleAlertWinMsg(
                                    uint32 win_id, 
                                    MMI_MESSAGE_ID_E msg_id,
                                    DPARAM param)
{
    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        s_confirm_result = BRW_CONFIRM_WAIT;
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, TXT_COMMON_OK, FALSE);
		MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break; 
    case MSG_APP_WEB:    
    case MSG_CLOSE_WINDOW:
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL: 
#ifdef MMI_PDA_SUPPORT
	case MSG_KEYUP_RED:
#else		
	case MSG_APP_RED:
#endif
		s_confirm_result = BRW_CONFIRM_OK;
		MMK_CloseWin(win_id);
        break;
		
    default:
		MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
	return TRUE;
}

/*****************************************************************************/
//  Description : open an alert window for display:str_ptr
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN BrwXhtmlLayoutOpenAlertWin(const uint16* str_ptr)
{
    MMI_STRING_T mstr = {0};
	uint32	timer_out = 0;
	MMI_WIN_ID_T	win_id = BRW_ALERT_WIN_ID;
	
    mstr.wstr_ptr = (wchar *)str_ptr;
    mstr.wstr_len = BRW_WStrlen(str_ptr);
    MMIPUB_OpenAlertWinByTextPtr(&timer_out,
            &mstr,
            NULL,
            IMAGE_PUBWIN_WARNING,
            &win_id,
            PNULL,
            MMIPUB_SOFTKEY_CUSTOMER,
            BrwHandleAlertWinMsg);

    WaitUserConfirm();

    return TRUE;
    
}

/*****************************************************************************/
//  Description : BrwSetWatchDogPtr
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
void BrwSetWatchDogPtr(void *SwdgPtr)
{
    s_watchdog_ptr = (SWDG_OBJECT *)SwdgPtr;
}

/*****************************************************************************/
//  Description : get history num
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC uint16 BrwGetCurHisNum(void)
{
    return BRW_VisitHisGetTotal();
}
#endif
/******************************************************************************************
* DESCRIPTION:
*   get a line height in browser layout
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   font_type: [IN] font type
*
* RETURN:
*   line height in browser layout
*
* NOTE:
******************************************************************************************/
PUBLIC uint16 BrwLineHeight(uint32 font_type)
{
    return GUI_GetFontHeight(ConvertWapFontSizeToMMISize(font_type), UNICODE_HANZI) + BRW_LINE_SPACE_HEIGHT;
}

/******************************************************************************************
* DESCRIPTION:
*   get a Chinese char width
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   font_type: [IN] font type
*
* RETURN:
*   Chinese char width
*
* NOTE:
******************************************************************************************/
PUBLIC uint8 BrwFontGetHanziWidth(uint32 font_type)
{
    return GUI_GetFontWidth((GUI_FONT_T)ConvertWapFontSizeToMMISize(font_type), UNICODE_HANZI);
}
/******************************************************************************************
* DESCRIPTION:  convert big5 string to unicode string
* AUTHOR:  jiqun.li
* PARAMETERS:
*   unicode_ptr: [OUT] destnation unicode string
*   big5_ptr:  [IN] source big5 string
*   len:         [IN] source big5 string length
* RETURN:  unicode string length, number of unicode char
******************************************************************************************/
PUBLIC uint32 BrwBig5ToWstr(wchar *unicode_ptr, const uint8 *big5_ptr, uint32 len)
{
#if (defined IM_CHINESE_T_SUPPORT) ||(defined IM_TRAD_CHINESE_SUPPORT)
    return GUI_Big5ToWstr(unicode_ptr, big5_ptr, len);/*lint !e718*/
#else 
    return GUI_GBToWstr(unicode_ptr, big5_ptr, len);
#endif   
}
/******************************************************************************************
* DESCRIPTION:  convert unicode to big5 string string
* AUTHOR:   jiqun.li
* PARAMETERS:
*   unicode_ptr: [OUT] destnation unicode string
*   big5_ptr:    [IN] source big5 string
*   len:         [IN] source big5 string length
* RETURN:  unicode string length, number of unicode char
******************************************************************************************/
PUBLIC uint32 BrwWstrToBig5(uint8 *big5_ptr, const wchar *unicode_ptr, uint32 len)
{
#if (defined IM_CHINESE_T_SUPPORT) ||(defined IM_TRAD_CHINESE_SUPPORT)
    return GUI_WstrToBig5(big5_ptr, unicode_ptr, len);/*lint !e718*/
#else 
    return GUI_WstrToGB(big5_ptr, unicode_ptr, len);
#endif  
}

/******************************************************************************************
* DESCRIPTION:
*   convert windows 1251 string to unicode string
* AUTHOR:   wenmei.hua
* PARAMETERS:
*   unicode_ptr: [OUT] destnation unicode string
*   win1251_ptr: [IN] source win1251 string
*   len:         [IN] source win1251 string length
*
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwWin1251ToWstr(wchar *unicode_ptr, const uint8 *win1251_ptr, uint32 len)
{
#ifdef IM_RUSSIAN_SUPPORT
    return GUI_CP1251ToWstr(unicode_ptr, win1251_ptr, len);
#else
    return GUI_CP1252ToWstr(unicode_ptr, win1251_ptr, len);
#endif
}
/******************************************************************************************
* DESCRIPTION:
*   convert unicode  string to windows 1251 string
* AUTHOR:  wenmei.hua
* PARAMETERS:
*   win1251_ptr: [OUT] destnation win1251 string
*   unicode_ptr: [IN] source unicode string
*   len:         [IN] source unicode string length
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwWstrToWin1251( uint8 *win1251_ptr, const wchar *unicode_ptr, uint32 len)
{
#ifdef IM_RUSSIAN_SUPPORT
     return GUI_WstrToCP1251(win1251_ptr, unicode_ptr, len);
#else
     return GUI_WstrToCP1252(win1251_ptr, unicode_ptr, len);
#endif 
}

/******************************************************************************************
* DESCRIPTION:
*   convert koi8-r string to unicode string
* AUTHOR:   wenmei.hua
* PARAMETERS:
*   unicode_ptr: [OUT] destnation unicode string
*   koi8r_ptr:   [IN] source kio8r_ptr string
*   len:         [IN] source kio8r_ptr string length
*
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwKoi8rToWstr(wchar *unicode_ptr, const uint8 *koi8r_ptr, uint32 len)
{
#ifdef IM_RUSSIAN_SUPPORT
    return GUI_KOI8RToWstr(unicode_ptr, koi8r_ptr, len);
#else
    return GUI_CP1252ToWstr(unicode_ptr, koi8r_ptr, len);
#endif
}
/******************************************************************************************
* DESCRIPTION:
*   convert unicode  string to kio8r string
* AUTHOR:  wenmei.hua
* PARAMETERS:
*   koi8r_ptr: [OUT] destnation kio8r string
*   unicode_ptr: [IN] source unicode string
*   len:         [IN] source unicode string length
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwWstrToKoi8r( uint8 *koi8r_ptr, const wchar *unicode_ptr, uint32 len)
{
#ifdef IM_RUSSIAN_SUPPORT
     return GUI_WstrToKOI8R(koi8r_ptr, unicode_ptr, len);
#else
     return GUI_WstrToCP1252(koi8r_ptr, unicode_ptr, len);
#endif 
}

/******************************************************************************************
* DESCRIPTION:
*   convert windows874 string to unicode string
* AUTHOR:   wenmei.hua
* PARAMETERS:
*   unicode_ptr:      [OUT] destnation unicode string
*   cp874_ptr:        [IN] source cp874_ptr string
*   len:              [IN] source cp874_ptr string length
*
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwCP874ToWstr(wchar *unicode_ptr, const uint8 *cp874_ptr, uint32 len)
{
#ifdef IM_THAI_SUPPORT
    return GUI_CP874ToWstr(unicode_ptr, cp874_ptr, len);
#else
    return GUI_UTF8ToWstr(unicode_ptr,len, cp874_ptr, len);
#endif
}
/******************************************************************************************
* DESCRIPTION:
*   convert unicode  string to cp874 string
* AUTHOR:  wenmei.hua
* PARAMETERS:
*   cp874_ptr:   [OUT] destnation cp874 string
*   unicode_ptr: [IN] source unicode string
*   len:         [IN] source unicode string length
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwWstrToCP874( uint8 *cp874_ptr, const wchar *unicode_ptr, uint32 len)
{
#ifdef IM_THAI_SUPPORT
     return GUI_WstrToCP874(cp874_ptr, unicode_ptr, len);
#else
     return GUI_WstrToUTF8(cp874_ptr, len, unicode_ptr, len);
#endif 
}
/******************************************************************************************
* DESCRIPTION:
*   convert a unicode char to GB2312 buffer
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   gb2312_ptr: [OUT] save destnation GB2312 char
*   unicode:    [IN]  unicode char
*
* RETURN:
*   bytes of GB2312 char
*
* NOTE:
******************************************************************************************/
PUBLIC int32 BrwUnicode2gb2312(uint8 *gb2312_ptr, uint16 unicode)
{
    return GUI_WstrToGB(gb2312_ptr, &unicode, 1);
}

/******************************************************************************************
* DESCRIPTION:
*   convert gb2312 string to unicode string
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   unicode_ptr: [OUT] destnation unicode string
*   gb2312_ptr:  [IN] source GB2312 string
*   len:         [IN] source GB2312 string length
*
* RETURN:
*   unicode string length, number of unicode char
*
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwGb2312ToWstr(wchar *unicode_ptr, const uint8 *gb2312_ptr, uint32 len)
{
    return GUI_GBToWstr(unicode_ptr, gb2312_ptr, len);//@fen.xie NS00210984
}

/******************************************************************************************
* DESCRIPTION:
*   convert windows 1252 string to unicode string
*
* AUTHOR:
*   wenmei.hua
*
* PARAMETERS:
*   unicode_ptr: [OUT] destnation unicode string
*   win1252_ptr: [IN] source win1252 string
*   len:         [IN] source win1252 string length
*
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwWin1252ToWstr(wchar *unicode_ptr, const uint8 *win1252_ptr, uint32 len)
{
    return GUI_CP1252ToWstr(unicode_ptr, win1252_ptr, len);
}
/******************************************************************************************
* DESCRIPTION:
*   convert unicode  string to windows 1252 string
*
* AUTHOR:
*   wenmei.hua
*
* PARAMETERS:
*   win1252_ptr: [OUT] destnation win1252 string
*   unicode_ptr: [IN] source unicode string
*   len:         [IN] source unicode string length
*
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwWstrToWin1252( uint8 *win1252_ptr, const wchar *unicode_ptr, uint32 len)
{
    return GUI_WstrToCP1252(win1252_ptr, unicode_ptr, len);
}


/******************************************************************************************
* DESCRIPTION:
*   convert windows 1256 string to unicode string
*
* AUTHOR:
*   xianhe.yang
*
* PARAMETERS:
*   unicode_ptr: [OUT] destnation unicode string
*   win1256_ptr: [IN] source win1256 string
*   len:         [IN] source win1256 string length
*
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwWin1256ToWstr(wchar *unicode_ptr, const uint8 *win1256_ptr, uint32 len)
{
    return GUI_CP1256ToWstr(unicode_ptr, win1256_ptr, len);
}
/******************************************************************************************
* DESCRIPTION:
*   convert unicode  string to windows 1256 string
*
* AUTHOR:
*   xianhe.yang
*
* PARAMETERS:
*   win1256_ptr: [OUT] destnation win1256 string
*   unicode_ptr: [IN] source unicode string
*   len:         [IN] source unicode string length
*
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwWstrToWin1256( uint8 *win1256_ptr, const wchar *unicode_ptr, uint32 len)
{
    return GUI_WstrToCP1256(win1256_ptr, unicode_ptr, len);
}

/******************************************************************************************
* DESCRIPTION:
*   convert gb2312 string to unicode string
*
* AUTHOR:
*   sally.he
*
* PARAMETERS:
*   unicode_ptr: [OUT] destnation gb2312 string
*   gb2312_ptr:  [IN] source unicode string
*   len:         [IN] source unicode string length
*
* NOTE:
******************************************************************************************/
PUBLIC uint16 BrwWstrToGb2312(uint8  *gb2312_ptr, const wchar *wstr_ptr, uint32 len)
{
    return  GUI_WstrToGB(gb2312_ptr, wstr_ptr, len);
}

/******************************************************************************************
* DESCRIPTION:
*   draw a Unicode char width
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   font_type: [IN] font type
*   unicode: [IN] unicode char
*
* RETURN:
*   char width
*
* NOTE:
******************************************************************************************/
PUBLIC uint8 BrwFontGetCharWidth(uint32 font_type, uint16 unicode)
{
    return GUI_GetFontWidth((GUI_FONT_T)ConvertWapFontSizeToMMISize(font_type), unicode);
}

/******************************************************************************************
* DESCRIPTION:
*   draw a horizontal line
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   x1: [IN] x-coordinate start
*   x2: [IN] x-coordinate end
*   y:  [IN] y-coordinate
*   color:  [IN] RGB565 color
*   region_ptr: [IN] web page display region
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
PUBLIC void BrwGraphicsDrawHLine(int32 x1, int32 y, int32 x2, uint16 color, RECT_T *region_ptr)
{
    if(PNULL ==region_ptr)
    {
        //SCI_TRACE_LOW:"BrwGraphicsDrawHLine ::PNULL ==region_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_763_112_2_18_2_7_3_513,(uint8*)"");
        return;
    }

    if(x1>x2) {
        int32 t = x1;
        x1 = x2;
        x2 = t;
    }
    /*change to absolute coordinate in LCD*/
    x1 += region_ptr->left;
    x2 += region_ptr->left;
    y  += region_ptr->top;

    /*check if it' in display region, and clip by display region*/
    if(x2 < region_ptr->left || x1 > region_ptr->right || y < region_ptr->top || y > region_ptr->bottom) {
        return;
    }
    x1 = MAX(x1, region_ptr->left);
    x2 = MIN(x2, region_ptr->right);
    LCD_DrawHLine(&lcd_dev_info, (uint16)x1, (uint16)y, (uint16)x2, (GUI_COLOR_T)color);
}

/******************************************************************************************
* DESCRIPTION:
*   draw a vertical line
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   y1: [IN] y-coordinate start
*   y2: [IN] y-coordinate end
*   x:  [IN] x-coordinate
*   color:  [IN] RGB565 color
*   region_ptr: [IN] web page display region
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
PUBLIC void BrwGraphicsDrawVLine(int32 x, int32 y1, int32 y2, uint16 color, RECT_T *region_ptr)
{
    if(PNULL ==region_ptr)
    {
        //SCI_TRACE_LOW:"BrwGraphicsDrawVLine ::PNULL ==region_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_808_112_2_18_2_7_3_514,(uint8*)"");
        return;
    }

    if(y1>y2) 
    {
        int32 t = y1;
        y1 = y2;
        y2 = t;
    }    

    /*change to absolute coordinate in LCD*/
    x  += region_ptr->left;
    y1 += region_ptr->top;
    y2 += region_ptr->top;

    if( x < region_ptr->left || x > region_ptr->right || y2 < region_ptr->top || y1 > region_ptr->bottom) {
        return;
    }

    /*check if it' in display region, and clip by display region*/
    y1 = MAX(y1, region_ptr->top);
    y2 = MIN(y2, region_ptr->bottom);

    LCD_DrawVLine(&lcd_dev_info, (uint16)x, (uint16)y1, (uint16)y2, (GUI_COLOR_T)color);
}

/******************************************************************************************
* DESCRIPTION:
*   draw a thick line with color,style and width
*
* AUTHOR:
*   kelly.li
*
* PARAMETERS:
*   first_point[IN]     coordinate of first point
*   econd_point[IN]    coordinate of second point
*   color:[IN] color to draw
*   width:[IN]unit: pixel
*   style:[IN]line display style
*   region_ptr: [IN] web page display region
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
PUBLIC void BrwGraphicsDrawThickLine(
                                BRW_POINT_T first_point,
                                BRW_POINT_T second_point,                               
                                uint16      color,
                                int32       width,
                                BRW_PEN_STYLE_E style,
                                RECT_T          *region_ptr)
{
    int32   x1=0;
    int32   y1=0;
    int32   x2=0;
    int32   y2=0;
    int32   t =0;
    int32   tmpy =0;

    if (PNULL == region_ptr)
    {
        return;
    }
    
    if(width == 0)
    {
        return;
    }

    x1 = first_point.x;
    y1 = first_point.y;

    x2 = second_point.x;
    y2 = second_point.y;

    if(x1>x2) {
        t = x1;
        x1 = x2;
        x2 = t;
    }
    
    if(y1>y2) 
    {
        t = y1;
        y1 = y2;
        y2 = t;
    }    

    /*change to absolute coordinate in LCD*/
    x1 += region_ptr->left;
    x2 += region_ptr->left;
    y1 += region_ptr->top;
    y2 += region_ptr->top;

    if( x2 < region_ptr->left 
        || x1 > region_ptr->right 
        || y2 < region_ptr->top 
        || y1 > region_ptr->bottom     
        ) 
    {
        return;
    }
    
    //draw thick horizontal line
    if (y1==y2 && width >=2)
    {
        tmpy = y1;
        y1+= width>>1;
        y2+= width>>1;
    }

    //draw thick vertical line
    if (x1==x2 && width >2)
    {
        x1+= width>>1;
        x2+= width>>1;
    }

    /*check if it' in display region, and clip by display region*/
    y1 = MAX(y1, region_ptr->top);
    y2 = MIN(y2, region_ptr->bottom);
    
    if (y1==y2 && (tmpy+width) > region_ptr->bottom)
    {
        y2 = tmpy + ((region_ptr->bottom- tmpy)>>1);
        width = region_ptr->bottom- tmpy;
        y1 = y2;
    }
    if( x2 < region_ptr->left 
        || x1 > region_ptr->right 
        || y2 < region_ptr->top 
        || y1 > region_ptr->bottom     
        ) 
    {
        return;
    }
    LCD_DrawThickLineEx_NonCap(&lcd_dev_info, (uint16)x1, (uint16)y1, (uint16)x2,(uint16)y2, (GUI_COLOR_T)color,width,(GUILCD_PEN_STYLE_E)style);

}

/******************************************************************************************
* DESCRIPTION: brief Draw a line between 2 specific point to LCD buffer.
* AUTHOR: wenmei.hua
* PARAMETERS:
 *  \param[in]      lcd_id      ID of LCD
 *  \param[in]      x1          x coordinate of first point
 *  \param[in]      y1          y coordinate of first point
 *  \param[in]      x2          x coordinate of second point
 *  \param[in]      y2          y coordinate of second point
 *  \param[in]      color       color to draw.
******************************************************************************************/
PUBLIC void BrwGraphicsDrawLine(int32 x1, int32 y1, int32 x2, int32 y2, uint16 color, RECT_T *region_ptr)
{
    if(PNULL ==region_ptr)
    {
        //SCI_TRACE_LOW:"BrwGraphicsDrawLine ::PNULL ==region_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_956_112_2_18_2_7_4_515,(uint8*)"");
        return;
    }
    /*change to absolute coordinate in LCD*/
    x1 += region_ptr->left;
    x2 += region_ptr->left;
    y1 += region_ptr->top;
    y2 += region_ptr->top;

    if(x2 < region_ptr->left || x1 > region_ptr->right || y2 < region_ptr->top || y1 > region_ptr->bottom) {
        return;
    }
    
    /*check if it' in display region, and clip by display region*/
    if(x1 > x2)
    {
        x1 = MIN(x1, region_ptr->right);
        x2 = MAX(x2, region_ptr->left);
    }
    else
    {
        x1 = MAX(x1, region_ptr->left);
        x2 = MIN(x2, region_ptr->right);
    }
    if(y1 > y2)
    {
        y1 = MIN(y1, region_ptr->bottom);
        y2 = MAX(y2, region_ptr->top);
    }
    else
    {
        y1 = MAX(y1, region_ptr->top);
        y2 = MIN(y2, region_ptr->bottom);
    }
   
    LCD_DrawLine(&lcd_dev_info, (uint16)x1, (uint16)y1, (uint16)x2, (uint16)y2, (GUI_COLOR_T)color);
}

/******************************************************************************************
* DESCRIPTION:
*   Draw trapmf with RGB565 color
*
* AUTHOR:
*   sally.he
*
* TOP:

   x1, y1                                        x2,y2
    ---------------------------------------------
     .                                        .     width
      ----------------------------------------
                                           
  BOTTOM:
  
      x1,y1                                  x2,y2
      ----------------------------------------
     .                                        .     width
    ---------------------------------------------
                                              

  LEFT:            Right

  x1,y1                  
  |                   x1,y1 |  
  | .                     . |            
  |  |                   |  |
  |  |                   |  |
  |  |                   |  |
  |  |                   |  |
  | .               x2,y2 . |
  |width              width |      
  x2,y2                  

*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/

PUBLIC void BrwGraphicsDrawTrampmfLine(int32            pos[4],     //x1, y1, x2, y2
                                       double           scale1,     //slope(x)
                                       double           scale2,     //slope(y)
                                       int32            width,      //line width
                                       uint16           color,      //rgb565
                                       BRW_TRAMPMF_LINE_DIRECT_E  direct,     //0:top, 1:bottom, 2:left, 3:right
                                       RECT_T           *region_ptr //layout rect
                                       )
{
    int32 x1     = 0;
    int32 y1     = 0;
    int32 x2     = 0;
    int32 y2     = 0;
    double value = 0;
    int32  i     = 0;
    //pos[0] pos[1] pos[2] pos[3] 
    // x1      y1    x2      y2   
    
    switch(direct)
    {
    case BRW_TRAMPMF_LINE_TOP: //top
        for(i = 0; i < width ; i++)
        {
            x1 = pos[0] + (int32)(i*scale1);
            value = i*scale2;
            if(value == (int32)value)
            {
                x2 = pos[2] - (int32)value;
            }
            else
            {
                x2 = pos[2] - (int32)value + 1;
            }
            y1 = pos[1] + i;
            y2 = y1;
            BrwGraphicsDrawLine(x1, y1, x2, y2, color, region_ptr);  
        }
        break;
    case BRW_TRAMPMF_LINE_BOTTOM://bottom
        for(i = 0; i < width ; i++)
        {
            x1 = pos[0] - (int32)(i*scale1);
            value = i*scale2;
            if(value == (int32)value)
            {
                x2 = pos[2] + (int32)value;
            }
            else
            {
                x2 = pos[2] + (int32)value + 1;
            }
            y1 = pos[1] + i;
            y2 = y1;
            BrwGraphicsDrawLine(x1, y1, x2, y2, color, region_ptr);  
        }
        break;
    case BRW_TRAMPMF_LINE_LEFT://left
        for(i = 0; i < width ; i++)
        {
            x1 = pos[0] + i;
            x2 = x1;
            value = i*scale1;
            if(value == (int32)value)
            {
                y1 = pos[1] + (int32)value;
            }
            else
            {
                y1 = pos[1] + (int32)value - 1;
            }
            value = i* scale2;
            if(value == (int32)value)
            {
                y2 = pos[3] - (int32)value;
            }
            else
            {
                y2 = pos[3] - (int32)value + 1;
            }
            BrwGraphicsDrawLine(x1, y1, x2, y2, color, region_ptr);  
        }
        break;
    case BRW_TRAMPMF_LINE_RIGHT://right
        for(i = 0; i < width ; i++)
        {
            x1 = pos[0] + i;
            x2 = x1;
            value = i*scale1;
            if(value == (int32)value)
            {
                y1 = pos[1] - (int32)value;
            }
            else
            {
                y1 = pos[1] - (int32)value - 1;
            }
            value = i*scale2;
            if(value == (int32)value)
            {
                y2 = pos[3] + (int32)value;
            }
            else
            {
                y2 = pos[3] + (int32)value + 1;
            }
            BrwGraphicsDrawLine(x1, y1, x2, y2, color, region_ptr);  
        }
        break;
    default:
        break;
    }
}

/******************************************************************************************
* DESCRIPTION:
*   draw a rectangle
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   rect_ptr: [IN] rectangle pointer
*   color:  [IN] RGB565 color
*   region_ptr: [IN] web page display region
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
PUBLIC void BrwGraphicsDrawRect(RECT_T *rect_ptr, uint16 color, RECT_T *region_ptr)
{
    if(rect_ptr) {
        BrwGraphicsDrawHLine(rect_ptr->left, rect_ptr->top, rect_ptr->right, color, region_ptr);
        BrwGraphicsDrawHLine(rect_ptr->left, rect_ptr->bottom, rect_ptr->right, color, region_ptr);
        BrwGraphicsDrawVLine(rect_ptr->left, rect_ptr->top, rect_ptr->bottom, color, region_ptr);
        BrwGraphicsDrawVLine(rect_ptr->right, rect_ptr->top, rect_ptr->bottom, color, region_ptr);
    }
}

/******************************************************************************************
* DESCRIPTION:
*   draw rectangle with extended mode
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   rect_ptr: [IN] rectangle
*   rgb16:    [IN] RGB565 color
*   is_xor:   [IN] TRUE: XOR operation with background
*   region_ptr:[IN] clip rectangle
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
PUBLIC void BrwGraphicsDrawRectExt(RECT_T *rect_ptr, uint16 rgb16, BOOLEAN is_xor, RECT_T *region_ptr)
{
    if(!rect_ptr) {
        return;
    }
    BrwGraphicsDrawRect(rect_ptr, rgb16, region_ptr);
}

/******************************************************************************************
* DESCRIPTION:
*   clip and fill rectangle with RGB565 color
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   rect_ptr: [IN] rectangle
*   rgb16:    [IN] RGB565 color
*   region_ptr:[IN] clip rectangle
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
PUBLIC void BrwGraphicsFillRect(RECT_T *rect_ptr,uint16 rgb16, RECT_T *region_ptr)
{
    GUI_RECT_T  win_rect = {0};

    if(PNULL ==region_ptr)
    {
        //SCI_TRACE_LOW:"BrwGraphicsFillRect ::PNULL ==region_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_1221_112_2_18_2_7_4_516,(uint8*)"");
        return;
    }
    if( rect_ptr->left > rect_ptr->right ||
        rect_ptr->top > rect_ptr->bottom ||
        rect_ptr->top > region_ptr->bottom){
        return;
    }
    win_rect = GUI_CreateRect((uint16)rect_ptr->left, (uint16)rect_ptr->top,(uint16)rect_ptr->right, (uint16)rect_ptr->bottom);
    win_rect.left      += region_ptr->left;
    win_rect.right     += region_ptr->left;
    win_rect.top       += region_ptr->top;
    win_rect.bottom    += region_ptr->top;

    if( win_rect.left > region_ptr->right ||
        win_rect.top > region_ptr->bottom ||
        win_rect.right < region_ptr->left ||
        win_rect.bottom < region_ptr->top ) {
        return;
    }
    win_rect.left  = MAX(win_rect.left, region_ptr->left);
    win_rect.right = MIN(win_rect.right, region_ptr->right);
    win_rect.top   = MAX(win_rect.top, region_ptr->top);
    win_rect.bottom= MIN(win_rect.bottom, region_ptr->bottom);
    GUI_FillRect(&lcd_dev_info, win_rect, rgb16);
}

/******************************************************************************************
* DESCRIPTION:
*   fill rectangle with RGB565 color
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   rect_ptr: [IN] rectangle
*   rgb16:    [IN] RGB565 color
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
PUBLIC void BrwFillLcdRect(RECT_T *rect_ptr,uint16 rgb16)
{
    GUI_RECT_T  win_rect = {0};

    if(PNULL ==rect_ptr)
    {
        //SCI_TRACE_LOW:"BrwFillLcdRect ::PNULL ==rect_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_1269_112_2_18_2_7_4_517,(uint8*)"");
        return;
    }
    
    if( rect_ptr->left > rect_ptr->right ||
        rect_ptr->top > rect_ptr->bottom) {
        return;
    }
    win_rect = GUI_CreateRect((uint16)rect_ptr->left, (uint16)rect_ptr->top,(uint16)rect_ptr->right, (uint16)rect_ptr->bottom);
    GUI_FillRect(&lcd_dev_info, win_rect, rgb16);
}

/******************************************************************************************
* DESCRIPTION:
*   get unicode string width
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   unocode_ptr: [IN] unicode string
*   len:  [IN] unicode char number in unicode string, -1 means unicode string has Zero end
*   font_type:   [IN] font type
*
* RETURN:
*   Chinese char width
*
* NOTE:
******************************************************************************************/
PUBLIC uint16 BrwFontGetStringWidth(uint16 *unicode_ptr,uint32 len, FONT_TYPE_STATE_T font_type_state)
{
    uint16 width=0;
    MMI_STRING_T    text_str= {0};
    GUISTR_STYLE_T  text_style= {0};
    GUISTR_STATE_T  text_state= 0;

    if(!unicode_ptr||!len) {
        return 0;
    }
    if((int32)len < 0) {
        len = BRW_WStrlen(unicode_ptr);
    }
    if((int32)len<=0) {
        return 0;
    }

    text_str.wstr_ptr = unicode_ptr;
    text_str.wstr_len = len;
    text_style.font = ConvertWapFontSizeToMMISize(font_type_state.font_type);

    if(font_type_state.is_font_bold)
    {
        text_state |= GUISTR_STATE_BOLD;
    }
    if (font_type_state.is_font_italic)
    {
        text_state |= GUISTR_STATE_ITALIC;
    }
    if (font_type_state.is_font_underline)
    {
        text_state |= GUISTR_STATE_UNDERLINE;
    }

    width = GUISTR_GetStringWidth(&text_style, &text_str, text_state);
    return width;
}

/******************************************************************************************
* DESCRIPTION:
*   get num of lines when showing unicode string
*
* AUTHOR:
*   sally.he
*
* PARAMETERS:
*   pixelnum:    [IN] pixel number
*   unocode_ptr: [IN] unicode string
*   len:  [IN] unicode char number in unicode string, -1 means unicode string has Zero end
*   font_type:   [IN] font type
*
* RETURN:
*   Chinese char width
*
* NOTE:
******************************************************************************************/
PUBLIC uint16 BrwtFontGetStringLines(
                                     uint16    pixelnum,    
                                     uint16    *unicode_ptr, 
                                     uint32    len, 
                                     uint32    font_type
                                     )
{
    uint16 line_num = 0;
    
    if( unicode_ptr == NULL || len == 0) 
    {
        //SCI_TRACE_LOW:"BrwDisplayDownloadingImage ::PNULL ==unicode_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_1363_112_2_18_2_7_4_518,(uint8*)"");
        return 0;
    }
    
    if((int32)len < 0) 
    {
        len = BRW_WStrlen(unicode_ptr);
    }
    if((int32)len <= 0) 
    {
        return 0;
    }

    line_num = GUI_CalculateStringLinesByPixelNum(
                                                  pixelnum, 
                                                  unicode_ptr, 
                                                  len,
                                                  ConvertWapFontSizeToMMISize(font_type), 
                                                  0 ,
                                                  FALSE);

    return line_num;
}

/******************************************************************************************
* DESCRIPTION:
*   draw a unicode string to screen
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   rect_ptr: [IN] pointer to text display region, in logical coordinates.
*   wap_str_ptr: [IN] pointer to the string info to be drawn.
*   color: [IN] specifies the color.
*   font_type: [IN] specifies the font type.
*   region_ptr: [IN] specifies the display region, in physical coordinates.
*   is_line_single:[IN] show text in one line or multiple lines.
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
PUBLIC void BrwDrawText(RECT_T *rect_ptr, WAP_STRING_FONT_TEXT_T *wap_str_ptr, uint16 color, 
                                            RECT_T *region_ptr, BOOLEAN is_line_single)
{
    GUI_RECT_T      rect        = {0};
	GUI_RECT_T      region      = {0};
    MMI_STRING_T    string      = {0};
    GUISTR_STYLE_T  text_style  = {0};
    GUISTR_STATE_T  state       = 0;

    if(rect_ptr == NULL || wap_str_ptr == NULL || wap_str_ptr->str_len == 0
        || rect_ptr->left > rect_ptr->right || rect_ptr->top > rect_ptr->bottom 
        || rect_ptr->top > region_ptr->bottom)
    {
        return;
    }
    
    if( wap_str_ptr->str_len == (uint16)-1) { // -1 means wstr has NULL at tail
        wap_str_ptr->str_len = BRW_WStrlen(wap_str_ptr->str_ptr);
    }
    
    rect.left   =(int16)( rect_ptr->left   + region_ptr->left);
    rect.right  =(int16)( rect_ptr->right  + region_ptr->left);
    rect.top    =(int16)( rect_ptr->top    + region_ptr->top);
    rect.bottom =(int16)( rect_ptr->bottom + region_ptr->top);

	region.left = (int16)region_ptr->left;
	region.right = (int16)region_ptr->right;
	region.top = (int16)region_ptr->top;
	region.bottom = (int16)region_ptr->bottom;

    string.wstr_ptr = (wchar *)wap_str_ptr->str_ptr;
    string.wstr_len = wap_str_ptr->str_len;

    text_style.font_color = (GUI_COLOR_T)color;
    text_style.font       = (GUI_FONT_T)ConvertWapFontSizeToMMISize(wap_str_ptr->font.font_type);
    text_style.line_color = (GUI_COLOR_T)color;
    text_style.word_space =  wap_str_ptr->text_style.word_space;

    if(IS_WAP_BIT_RTL(wap_str_ptr->str_info)) {
        text_style.align = ALIGN_RIGHT;
    }

    if(is_line_single)
    {
        state = GUISTR_STATE_SINGLE_LINE;
    }
        
    if (wap_str_ptr->font.is_font_bold)
    {
        state |= GUISTR_STATE_BOLD;
    }
    if (wap_str_ptr->font.is_font_italic)
    {
        state |= GUISTR_STATE_ITALIC;
    }
    if (wap_str_ptr->font.is_font_underline)
    {
        state |= GUISTR_STATE_UNDERLINE;
    }
    if (wap_str_ptr->text_style.is_space_valid)
    {
        state |= GUISTR_STATE_WORDSPACE;
    }      
    state |= GUISTR_STATE_WORDBREAK;

    GUISTR_DrawTextToLCDInRect (
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T *)&rect,
            (const GUI_RECT_T *)&region,
            (const MMI_STRING_T*) &string,
            &text_style,
            state,
            IS_WAP_BIT_RTL(wap_str_ptr->str_info) ? 1/*TEXT_DIR_RTL*/ : 0/*TEXT_DIR_LTR*/
            );
}

//正在下载的箭头图片的宽度
uint16 BrwGetImageDownloadingWidth(void)
{
    return 12;
}
//正在下载的箭头图片的高度
uint16 BrwGetImageDownloadingHeight(void)
{
    return 13;
}
//显示正在下载的箭头图片
void BrwDisplayDownloadingImage(RECT_T *rect_ptr, RECT_T *region_ptr, uint16 *alt_ptr)
{
     int32   left = 0;
     int32   top  = 0;
     int32   bottom = 0;
     
    if(PNULL == rect_ptr)
    {
        //SCI_TRACE_LOW:"BrwDisplayDownloadingImage ::PNULL ==rect_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_1501_112_2_18_2_7_5_519,(uint8*)"");
        return;
    }
    
     BrwGraphicsFillRect(rect_ptr, BRW_RGB16_BACKGROUND, region_ptr);
     left = rect_ptr->left;
     top  = rect_ptr->top;
     bottom = rect_ptr->bottom;
     //draw sqaure
     BrwGraphicsDrawVLine((left),(top),(bottom), BRW_RGB16_BUTTON_BORDER, region_ptr);
     BrwGraphicsDrawVLine((rect_ptr->right),(top),(bottom), BRW_RGB16_BUTTON_BORDER, region_ptr);
     BrwGraphicsDrawHLine((left),(top),(rect_ptr->right), BRW_RGB16_BUTTON_BORDER, region_ptr);
     BrwGraphicsDrawHLine((left),(bottom),(rect_ptr->right), BRW_RGB16_BUTTON_BORDER, region_ptr);

     BrwGraphicsDrawVLine((left+2), top+2, (top+6), BRW_RGB16_RED, region_ptr);
     BrwGraphicsDrawVLine((left+3), top+2, (top+6), BRW_RGB16_RED, region_ptr);
     BrwGraphicsDrawVLine((left+4), top+2, (top+6), BRW_RGB16_RED, region_ptr);
     BrwGraphicsDrawVLine((left+5), top+2, (top+6), BRW_RGB16_RED, region_ptr);
     BrwGraphicsDrawVLine((left+6), top+2, (top+6), BRW_RGB16_RED, region_ptr);
     BrwGraphicsDrawVLine((left+7), top+2, (top+6), BRW_RGB16_BUTTON_DARK_BORDER, region_ptr);
     BrwGraphicsDrawHLine((left+1), (top+7), (left+7), BRW_RGB16_RED, region_ptr);
     BrwGraphicsDrawHLine((left+2), (top+8), (left+6), BRW_RGB16_RED, region_ptr);
     BrwGraphicsDrawHLine((left+3), (top+9), (left+5), BRW_RGB16_RED, region_ptr);
     BrwGraphicsDrawHLine((left+4), (top+10),(left+4), BRW_RGB16_RED, region_ptr);     
     BrwGraphicsDrawLine(left+8, top+7, left+5, top+10, BRW_RGB16_RED, region_ptr );
     
     if (PNULL != alt_ptr)
     {
         RECT_T rect = {0};
         WAP_STRING_FONT_TEXT_T wap_str = {0};
         rect.left   = rect_ptr->left    + 10;
         rect.top    = rect_ptr->top     + 2;
         rect.right  = rect_ptr->right;
         rect.bottom = bottom;
         
         wap_str.str_ptr= alt_ptr;
         wap_str.str_len= BRW_WStrlen(alt_ptr);
         wap_str.font.font_type= ConvertWapFontSizeToMMISize(BRW_IMAGE_ALT_FONT_SIZE);		 
         BrwDrawText(&rect, &wap_str, BRW_RGB16_BUTTON_BORDER, region_ptr, FALSE);
     }
}

//返回下载失败的红X图片的宽度
uint16 BrwGetImageFailedWidth(void)
{
    return 14;
}
//返回下载失败的红X图片的高度
uint16 BrwGetImageFailedHeight(void)
{
    return BrwLineHeight(ConvertWapFontSizeToMMISize(BRW_IMAGE_ALT_FONT_SIZE));
}
//显示下载失败的红X图片
void BrwDisplayFailedImage(RECT_T *rect_ptr, RECT_T *region_ptr, uint16 *alt_ptr)
{
    int32   left = 0;
    int32   top = 0; 
    int32   bottom = 0;

    //BRWDebug_Printf:"[BRW]BrwDisplayFailedImage"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_1560_112_2_18_2_7_5_520,(uint8*)"");
    
    
    if(PNULL ==region_ptr)
    {
        //SCI_TRACE_LOW:"BrwDisplayFailedImage ::PNULL ==region_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_1565_112_2_18_2_7_5_521,(uint8*)"");
        return;
    }
    
    BrwGraphicsFillRect(rect_ptr, BRW_RGB16_BACKGROUND, region_ptr);
    left = rect_ptr->left;
    top  = rect_ptr->top;
    bottom = rect_ptr->bottom;
    /*显示正方形*/
    BrwGraphicsDrawVLine((left+0),(top+0),(bottom), BRW_RGB16_BUTTON_BORDER, region_ptr);
    BrwGraphicsDrawVLine((rect_ptr->right),(top+0),(bottom), BRW_RGB16_BUTTON_BORDER, region_ptr);
    BrwGraphicsDrawHLine((left+0),(top+0),(rect_ptr->right), BRW_RGB16_BUTTON_BORDER, region_ptr);
    BrwGraphicsDrawHLine((left+0),(bottom),(rect_ptr->right), BRW_RGB16_BUTTON_BORDER, region_ptr);
   
    if ((rect_ptr->bottom - rect_ptr->top) < BRW_FAIL_IMAGE_HEIGHT_MIN || (rect_ptr->right - rect_ptr->left) < BRW_FAIL_IMAGE_WIDTH_MIN)
    {
        BrwGraphicsDrawLine(left, top, rect_ptr->right, bottom, BRW_RGB16_RED, region_ptr );
        BrwGraphicsDrawLine(left, bottom, rect_ptr->right, top, BRW_RGB16_RED, region_ptr ); 
    }
    else
    {
        BrwGraphicsDrawVLine((left+2),(top+3),(top+12), BRW_RGB16_BUTTON_BORDER, region_ptr);
        BrwGraphicsDrawVLine((left+11),(top+3),(top+12), BRW_RGB16_BUTTON_BORDER, region_ptr);
        BrwGraphicsDrawHLine((left+2),(top+3),(left+11), BRW_RGB16_BUTTON_BORDER, region_ptr);
        BrwGraphicsDrawHLine((left+2),(top+12),(left+11), BRW_RGB16_BUTTON_BORDER, region_ptr);
        
        /*显示一个红X*/
        BrwGraphicsDrawLine(left+3, top+4, left+10, top+11, BRW_RGB16_RED, region_ptr );
        BrwGraphicsDrawLine(left+3, top+11, left+10, top+4, BRW_RGB16_RED, region_ptr );               
        
        
        if (PNULL != alt_ptr)
        {
            RECT_T rect = {0};
            WAP_STRING_FONT_TEXT_T wap_str = {0};
            rect.left   = rect_ptr->left    + 13;
            rect.top    = rect_ptr->top     + 2;
            rect.right  = rect_ptr->right;
            rect.bottom = bottom;
            
            wap_str.str_ptr= alt_ptr;
            wap_str.str_len= BRW_WStrlen(alt_ptr);
            wap_str.font.font_type= ConvertWapFontSizeToMMISize(BRW_IMAGE_ALT_FONT_SIZE);	
            BrwDrawText(&rect, &wap_str, BRW_RGB16_BUTTON_BORDER, region_ptr, FALSE);
        }        
    }
}

/************************************************************************/
//  Description : calc image width and height
//  return: FALSE: image unknow or decode error; TRUE: success
/************************************************************************/
PUBLIC BOOLEAN BrwImageCalc( uint8 *data_ptr,           //[IN] image data
                             uint32 data_len,           //[IN] image data length
                             BRW_MIME_TYPE_E mime_type, //[IN] image mime type
                             uint16 *width,             //[OUT]image width
                             uint16 *height)            //[OUT]image height
{
    BOOLEAN      ret    = FALSE;
    if(!data_ptr || !data_len || !width || !height) {
        return FALSE;
    }

    switch(mime_type) {
    case BRW_MIME_IMG_WBMP:
        {
            GUIIMG_INFO_T   wbmp_t  = {0};
            if(GUIIMG_GetWbmpInfo(data_ptr, data_len, &wbmp_t)) {
                *width  = (uint16)wbmp_t.image_width;
                *height = (uint16)wbmp_t.image_height;
                ret = TRUE;
            }
        }
        break;
    case BRW_MIME_IMG_PNG:
        {
            GUIIMG_INFO_T  png_info = {0};
            if(GUIIMG_GetPngInfo(data_ptr, data_len, &png_info)) {
                *width  = (uint16)png_info.image_width;
                *height = (uint16)png_info.image_height;
                ret = TRUE;
            }
        }
        break;
    case BRW_MIME_IMG_JPEG:
        {
            JINF_GET_INFO_IN_T  jpg_in   = {0};
            JINF_INFO_T         jpg_info = {0};
            jpg_in.jpeg_buf_ptr  = data_ptr;
            jpg_in.jpeg_buf_size = data_len;
            if(JINF_SUCCESS==IMGJPEG_GetInfo(&jpg_in, &jpg_info)) {
                 *width  = (uint16)jpg_info.image_width;
                 *height = (uint16)jpg_info.image_height;
                 ret = TRUE;
            }
        }
        break;
    case BRW_MIME_IMG_BMP:
        {
            GUIIMG_INFO_T   bmp_t  = {0};
            if(GUIIMG_GetBmpInfo(data_ptr, data_len, &bmp_t)) {
                *width  = (uint16)bmp_t.image_width;
                *height = (uint16)bmp_t.image_height;
                ret = TRUE;
            }
        }
        break;
    case BRW_MIME_IMG_GIF:
        {
            GUIIMG_INFO_T  gif_info = {0};
            if(GUIIMG_GetGifInfo(data_ptr, data_len, &gif_info)) {
                *width  = (uint16)gif_info.image_width;
                *height = (uint16)gif_info.image_height;
                ret = TRUE;
            }
        }
        break;
    case BRW_MIME_IMG_PCX:
        break;
    case BRW_MIME_IMG_TIF:
        break;
    case BRW_MIME_IMG_IEF:
        break;
    default:
        break;
    }

    //BRWDebug_Printf:"[BRW]BrwImageCalc:ret = %d, width =%d,height =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_1690_112_2_18_2_7_5_522,(uint8*)"ddd",ret,width,height);

    return ret;
}

/*****************************************************************************/
//  Description : play image (jpeg, bmp, wbmp, png)
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN BrwImageDisplayEx(uint8 *data_ptr, uint32 data_len, IMAGE_PLAY_INFO *info_ptr, int16 x, int16 y, GUI_RECT_T *rect_ptr)
{
    GUI_POINT_T                 dis_point = {0};
    IMG_DEC_RET_E               decode_ret = IMG_DEC_RET_SUCCESS ;
    IMG_DEC_FRAME_IN_PARAM_T    in_param = {0};
    IMG_DEC_FRAME_OUT_PARAM_T   out_param = {0};
    IMG_DEC_SRC_T   img_dec_src = {0};
    BOOLEAN is_need_zoom_in = FALSE;
    IMG_DEC_INFO_T  img_info_param ={0};
    uint32  target_width = 0;
    uint32  target_height = 0;
    uint32  original_width = 0;
    uint32  original_height = 0;
    BOOLEAN result = FALSE;
    uint32  handle = 0;

    if ((PNULL == info_ptr) || (PNULL == data_ptr) || (PNULL == rect_ptr))
    {
        //BRWDebug_Printf:"[BRW]W BrwImageDisplayEx input invalid!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_1717_112_2_18_2_7_5_523,(uint8*)"");
        return FALSE;
    }
    
    target_width = info_ptr->width;
    target_height = info_ptr->height;
    if ((0 == target_width) || (0 == target_height))
    {
        //BRWDebug_Printf:"[BRW]W BrwImageDisplayEx:target_width =%d, target_height =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_1725_112_2_18_2_7_5_524,(uint8*)"dd", target_width, target_height);
        return FALSE;
    }

    img_dec_src.src_file_size = data_len;
    img_dec_src.src_ptr = data_ptr;
    
    IMG_DEC_GetInfo(&img_dec_src, &img_info_param);//get gif info
    switch (info_ptr->mime_type) 
    {
    case BRW_MIME_IMG_JPEG:
        original_width = img_info_param.img_detail_info.jpeg_info.img_width;
        original_height= img_info_param.img_detail_info.jpeg_info.img_heigth;
        break;
        
    case BRW_MIME_IMG_PNG:
        original_width = img_info_param.img_detail_info.png_info.img_width;
        original_height= img_info_param.img_detail_info.png_info.img_height;
        break;
        
    case BRW_MIME_IMG_WBMP:
        original_width = img_info_param.img_detail_info.wbmp_info.img_width;
        original_height= img_info_param.img_detail_info.wbmp_info.img_height;
        break;
        
    case BRW_MIME_IMG_BMP:
        original_width = img_info_param.img_detail_info.bmp_info.img_width;
        original_height= img_info_param.img_detail_info.bmp_info.img_height;
        break;
        
    default:
        //BRWDebug_Printf:"[BRW]W BrwImageDisplayEx:info_ptr->mime_type =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_1756_112_2_18_2_7_5_525,(uint8*)"d", info_ptr->mime_type);
        return FALSE;
    }

    decode_ret = IMG_DEC_Create(&img_dec_src, &handle);
    if (IMG_DEC_RET_SUCCESS == decode_ret)
    {
        in_param.callback = PNULL;
        in_param.write_data = PNULL;
        in_param.handle = handle;
        in_param.data_format = IMG_DEC_RGB565;
        in_param.target_width = target_width;
        in_param.target_height = target_height;
        in_param.target_buf_size = in_param.target_width * in_param.target_height * 2;
        in_param.target_ptr = BRWMem_Alloc(in_param.target_buf_size);
        if (PNULL == in_param.target_ptr) 
        {
            IMG_DEC_Destroy(handle);
            //BRWDebug_Printf:"[BRW]W BrwImageDisplayEx:alloc fail, in_param.target_buf_size =%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_1773_112_2_18_2_7_5_526,(uint8*)"d", in_param.target_buf_size);
            return FALSE;
        }    
        
        decode_ret = IMG_DEC_GetFrame(&in_param, &out_param);//get decode data:IMG_DEC_GetFrame can zoom out,can't zoom in
        if (IMG_DEC_RET_SUCCESS == decode_ret) 
        {
            GUIIMG_BITMAP_T  bitmap_handle = {0};
            ZOOM_INPUT_T    zoom_in_data = {0};
            ZOOM_OUTPUT_T   zoom_out_data = {0};
            BOOLEAN is_zoom_success = FALSE;

            if (!info_ptr->is_not_need_zoom_in)
            {
                if ((target_width > original_width) || (target_height > original_height))
                {
                    is_need_zoom_in = TRUE;//target > original
                }
                //BRWDebug_Printf:"[BRW]BrwImageDisplayEx:is_need_zoom_in = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_1789_112_2_18_2_7_5_527,(uint8*)"d", is_need_zoom_in);
            }

            if (!is_need_zoom_in)
            {
                bitmap_handle.bit_ptr = in_param.target_ptr;
                bitmap_handle.img_width  = out_param.real_width;
                bitmap_handle.img_height = out_param.real_height;
            }
            else
            {            
                zoom_in_data.src_buf_ptr = in_param.target_ptr;
                zoom_in_data.src_buf_size = in_param.target_buf_size;
                zoom_in_data.src_img_width = out_param.real_width;
                zoom_in_data.src_img_height= out_param.real_height;
                zoom_in_data.dest_img_width = target_width;
                zoom_in_data.dest_img_height = target_height;
                zoom_in_data.dest_buf_size = (zoom_in_data.dest_img_width * zoom_in_data.dest_img_height * 2 + 255);
                zoom_in_data.dest_buf_ptr = BRWMem_Alloc(zoom_in_data.dest_buf_size);
                if (PNULL != zoom_in_data.dest_buf_ptr)
                {
                    if (ZOOM_RESULT_SUCC == BrwZoomImage(&zoom_in_data, &zoom_out_data))
                    {
                        is_zoom_success = TRUE;
                    }
                    else
                    {
                        is_zoom_success = FALSE;
                    }
                }
                else
                {
                    //BRWDebug_Printf:"[BRW]W BrwImageDisplayEx:ZOOM Alloc fail,zoom_in_data.dest_buf_size = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_1820_112_2_18_2_7_5_528,(uint8*)"d", zoom_in_data.dest_buf_size);
                    is_zoom_success = FALSE;
                }    

                if (is_zoom_success)
                {
                    bitmap_handle.bit_ptr    = (zoom_in_data.dest_buf_ptr + zoom_out_data.dest_buf_offset);
                    bitmap_handle.img_width  = zoom_out_data.img_width;
                    bitmap_handle.img_height = zoom_out_data.img_height;
                }
                else//zoom fail, use the original image to display
                {
                    bitmap_handle.bit_ptr    = in_param.target_ptr;
                    bitmap_handle.img_width  = out_param.real_width;
                    bitmap_handle.img_height = out_param.real_height;
                }
            }

            dis_point.x = x;
            dis_point.y = y;
            result = GUIIMG_DisplayBmp(FALSE, rect_ptr, &dis_point, &bitmap_handle, &lcd_dev_info);

            if (PNULL != zoom_in_data.dest_buf_ptr)
            {
                BRWMem_Free(zoom_in_data.dest_buf_ptr);
                zoom_in_data.dest_buf_ptr = PNULL;
            }
        } 
        else
        {
            //BRWDebug_Printf:"[BRW]W BrwImageDisplayEx:IMG_DEC_GetFrame  decode_ret= %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_1850_112_2_18_2_7_5_529,(uint8*)"d", decode_ret);
        }
    }
    else
    {
        //BRWDebug_Printf:"[BRW]W BrwImageDisplayEx:IMG_DEC_Create decode_ret =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_1855_112_2_18_2_7_5_530,(uint8*)"d", decode_ret);
    }

    decode_ret = IMG_DEC_Destroy(handle);
    if (IMG_DEC_RET_SUCCESS != decode_ret)
    {
        //BRWDebug_Printf:"[BRW]W BrwImageDisplayEx:IMG_DEC_Destroy decode_ret =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_1861_112_2_18_2_7_5_531,(uint8*)"d", decode_ret);
    }

    if (PNULL != in_param.target_ptr)
    {
        BRWMem_Free(in_param.target_ptr);
        in_param.target_ptr = PNULL;
    }
    
    return result;
}

/************************************************************************/
//  Description : display image
//  return: FALSE: image unknown or decode error; TRUE: success
/************************************************************************/
PUBLIC BOOLEAN BrwImageDisplay(IMAGE_PLAY_INFO *play_ptr)
{
    GUI_RECT_T      show_rect    = {0};
    BOOLEAN         result       = FALSE;
    int16           xstart       = 0;
    int16           ystart       = 0;
    RECT_T          *rect_ptr    = PNULL;
    uint8           *data_ptr    = PNULL;
    uint32          data_len     = 0;
    RECT_T          *region_ptr  = PNULL;

    if (PNULL == play_ptr)
    {
        return FALSE;
    }
    rect_ptr        = play_ptr->rect_ptr;
    data_ptr        = play_ptr->data_ptr;
    data_len        = play_ptr->data_len;
    region_ptr      = play_ptr->region_ptr;

    if ((PNULL == region_ptr) || (PNULL == rect_ptr) || (PNULL == data_ptr) 
        || (0 == data_len) || (0 == play_ptr->width) || (0 == play_ptr->height))
    {
        return FALSE;
    }

    xstart =(int16)( rect_ptr->left + region_ptr->left);
    ystart = (int16)(rect_ptr->top  + region_ptr->top);
    if (!play_ptr->is_not_need_zoom_in)
    {
        show_rect.left   = (int16)region_ptr->left;
        show_rect.top    = (int16)region_ptr->top;
        show_rect.right  = (int16)region_ptr->right;
        show_rect.bottom = (int16)region_ptr->bottom;
    }
    else
    {
        show_rect.left   = (int16)(rect_ptr->left + region_ptr->left);
        show_rect.top    = (int16)(rect_ptr->top  + region_ptr->top);
        show_rect.right  = (int16)(rect_ptr->right  + region_ptr->left);
        show_rect.bottom = (int16)(rect_ptr->bottom  + region_ptr->top);

        if ((show_rect.left < region_ptr->left) || (show_rect.left > region_ptr->right))
        {
            show_rect.left   = (int16)region_ptr->left;
        }
        if ((show_rect.right < region_ptr->left) || (show_rect.right > region_ptr->right))
        {
            show_rect.right  = (int16)region_ptr->right;
        }
        if ((show_rect.top < region_ptr->top) || (show_rect.top > region_ptr->bottom))
        {
            show_rect.top    = (int16)region_ptr->top;
        }
        if ((show_rect.bottom < region_ptr->top) || (show_rect.bottom > region_ptr->bottom))
        {
            show_rect.bottom    = (int16)region_ptr->bottom;
        }
    }

    switch(play_ptr->mime_type) 
    {
    case BRW_MIME_IMG_JPEG:  //fall throuth
    case BRW_MIME_IMG_PNG:   //fall throuth
    case BRW_MIME_IMG_WBMP:  //fall throuth
    case BRW_MIME_IMG_BMP:
        result = BrwImageDisplayEx(data_ptr, data_len, play_ptr, xstart, ystart, &show_rect);
        break;
        
    case BRW_MIME_IMG_GIF:
        result = BrwPlayGif(data_ptr, data_len, play_ptr, xstart, ystart, &show_rect);        
        break;
        
    case BRW_MIME_IMG_PCX:
        break;
        
    case BRW_MIME_IMG_TIF:
        break;
        
    case BRW_MIME_IMG_IEF:
        break;
        
    default:
        break;
    }

    //BRWDebug_Printf:"[BRW]BrwImageDisplay:result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_1934_112_2_18_2_7_6_532,(uint8*)"d", result);
    
    return (result);
}

PUBLIC void BrwImageDisplayFocus(RECT_T *rect_ptr, int32 width, int32 height, RECT_T *region_ptr, BOOLEAN is_erase)
{
    RECT_T rt = {0};
    if(!rect_ptr) {
        return;
    }
    rt.left     = rect_ptr->left    ;
    rt.top      = rect_ptr->top     ;
    rt.right    = rect_ptr->right   ;
    rt.bottom   = rect_ptr->bottom  ;
    if(is_erase) {
        BrwGraphicsDrawRectExt(&rt, BRW_RGB16_BACKGROUND, TRUE, region_ptr);
    } else {
        BrwGraphicsDrawRectExt(&rt, BRW_RGB16_FOCUSED_BACKGROUND, TRUE, region_ptr);//BRW_RGB16_TEXT
    }
}
//设置预选项
LOCAL void WmlPreSelectListControl(MMI_CTRL_ID_T ctrl_id, void *select_ptr)
{
    int32  index = 0;
    int32  item_num = 0;
    WML_LAYOUT_OPTION_T *option_ptr = NULL;

    if(PNULL ==select_ptr)
    {
        //SCI_TRACE_LOW:"WmlPreSelectListControl ::PNULL ==select_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_1964_112_2_18_2_7_6_533,(uint8*)"");
        return;
    }
    
    item_num = WmlGetSelectItemNum(select_ptr);

    for( index=0; index < item_num; index++ ) {
        option_ptr = WmlSelectFindOptionByIndex(select_ptr, index);
        if(!option_ptr) {
            return;
        }
        if(WmlOptionIsChecked(option_ptr)) {
            GUILIST_SetSelectedItem(ctrl_id, index, TRUE);
        } else {
            GUILIST_SetSelectedItem(ctrl_id, index, FALSE);
        }
    }
}

/*
设置WML select控件的列表
*/
LOCAL void SetupWmlListControl(MMI_CTRL_ID_T ctrl_id, void *select_ptr)
{
    int32  index = 0;
    int32  item_num = 0;
    
    if(PNULL ==select_ptr)
    {
        //SCI_TRACE_LOW:"SetupWmlListControl ::PNULL ==select_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_1992_112_2_18_2_7_6_534,(uint8*)"");
        return;
    }

    item_num = WmlGetSelectItemNum(select_ptr);

    GUILIST_SetMaxItem(ctrl_id, WmlGetSelectItemNum(select_ptr), FALSE);
    if(WmlSelectIsMultiple(select_ptr)) {
        GUILIST_ChangeDisplayType(ctrl_id, GUILIST_CHECKLIST_E,FALSE);
        GUILIST_SetMaxSelectedItem(ctrl_id, WmlGetSelectItemNum(select_ptr));
    } else {
        GUILIST_ChangeDisplayType(ctrl_id, GUILIST_RADIOLIST_E,FALSE);
    }
    //添加list item
    for(index=0; index < item_num; index++) {
        WML_LAYOUT_OPTION_T *option_ptr = NULL;
        uint16              *vdata_ptr  = NULL;
        GUILIST_ITEM_T      item_t      = {0};
        GUILIST_ITEM_DATA_T item_data   = {0};
        uint32              len;
        option_ptr = WmlSelectFindOptionByIndex(select_ptr, index);
        if(!option_ptr) {
            break;
        }
        item_t.item_data_ptr = &item_data;

        if(WmlSelectIsMultiple(select_ptr)) {
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_CHECK;
            item_data.softkey_id[0] = (MMI_TEXT_ID_T)STXT_SELECT;
            item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;//STXT_OK;
            item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;
        } else {
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
            item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_COMMON_OK;
            item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
            item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;
        }
        WmlCopyOptionContent(&vdata_ptr,option_ptr);
        len = BRW_WStrlen(vdata_ptr);
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = vdata_ptr;
        if(len>0) {
            //len = MIN(len, GUILIST_STRING_MAX_NUM); //james 不需要截断, list内部会截断
        } else {
            len = BRW_WStrlen(WmlGetOptionValue(option_ptr));
            if(len>0) {
                //len = MIN(len, GUILIST_STRING_MAX_NUM);
            } else {
                len = 1;
                item_data.item_content[0].item_data.text_buffer.wstr_ptr = L" ";
            }
        }
        //SCI_ASSERT(len<=GUILIST_STRING_MAX_NUM );
        item_data.item_content[0].item_data.text_buffer.wstr_len = (uint16)len;
        item_data.item_content[0].item_data_type                 = GUIITEM_DATA_TEXT_BUFFER;
        GUILIST_AppendItem(ctrl_id, &item_t);
        BRWMem_Free(vdata_ptr);
        vdata_ptr = NULL;
    }
    WmlPreSelectListControl(ctrl_id, select_ptr);
}
/*
WML 选择完毕
*/
LOCAL BOOLEAN WmlHandleSelectedFinish(MMI_CTRL_ID_T ctrl_id,void *select_ptr )
{
    int32              index       = 0;
    WML_LAYOUT_OPTION_T *option_ptr = NULL;
    int32                   item_num    = 0;
    const GUILIST_ITEM_T    *item_ptr   = NULL;

    if(PNULL ==select_ptr)
    {
        //SCI_TRACE_LOW:"WmlHandleSelectedFinish ::PNULL ==select_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2063_112_2_18_2_7_6_535,(uint8*)"");
        return FALSE;
    }

    item_num = WmlGetSelectItemNum(select_ptr);
    for(index=0; index<item_num; index++) 
    {
        item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id,index);
        if(PNULL ==item_ptr)
        {
            //SCI_TRACE_LOW:"WmlHandleSelectedFinish ::PNULL ==item_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2073_112_2_18_2_7_6_536,(uint8*)"");
            return FALSE;
        }
        option_ptr = WmlSelectFindOptionByIndex(select_ptr, index);
        if(option_ptr) 
        {
            if(GUILIST_GetItemState(item_ptr,GUIITEM_STATE_SELECTED)) 
            {
                WmlSetOptionChecked(option_ptr, TRUE);
            } 
            else 
            {
                WmlSetOptionChecked(option_ptr, FALSE);
            }
        }
    }

    if(!WmlSelectIsMultiple(select_ptr)) 
    {
        index       = GUILIST_GetCurItemIndex(ctrl_id);
        option_ptr  = WmlSelectFindOptionByIndex(select_ptr, index);
        if(WmlProcessOnpickEvent(option_ptr, select_ptr) ) 
        {
            //the handle is done in WmlProcessOnpickEvent
            return TRUE;
        }
    }
    
    WmlSetSelectResult(select_ptr);//set finish
    return TRUE;
}

/*
WML选择窗口的回调函数
*/
LOCAL MMI_RESULT_E WmlHandleSelectWinMsg(uint32 win_id,MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
    MMI_RESULT_E        recode      = MMI_RESULT_TRUE;
    WML_SELECT_WIN_INFO_T   *select_win_ptr = PNULL;
    MMI_CTRL_ID_T       ctrl_id     = BRW_SELECT_CTRL_ID;
    
    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        select_win_ptr = MMK_GetWinAddDataPtr(win_id);
        if(PNULL == select_win_ptr)
        {
            SCI_TRACE_LOW("[BRW]W WmlHandleSelectWinMsg :PNULL==select_win_ptr");
        }
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    {
        select_win_ptr = MMK_GetWinAddDataPtr(win_id);
        if ((PNULL ==select_win_ptr) || (PNULL == select_win_ptr->select_ptr))
        {
            //SCI_TRACE_LOW:"WmlHandleSelectWinMsg ::PNULL ==select_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2129_112_2_18_2_7_6_537,(uint8*)"");
            MMK_CloseWin(win_id);//error,返回上一级
            break;
        }

        if (!BrwIsLayoutBelongToCurDisplay((void*)(select_win_ptr->layout_ctrl_ptr)))
        {
           SCI_TRACE_LOW("[BRW]W WmlHandleSelectWinMsg :layout_ctrl_ptr error");
           MMK_CloseWin(win_id);//error,返回上一级
           break;
        } 

        if(WmlSelectIsMultiple(select_win_ptr->select_ptr)) 
        {
            //Multiple select:do nothing
        }
        else
        {
            //excuting the setting for single check
            WmlHandleSelectedFinish(ctrl_id, select_win_ptr->select_ptr);
            MMK_CloseWin(win_id);
        }
    }
        break;
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    {
        MMI_CTRL_ID_T       src_ctrl_id = 0;

        if (PNULL != param)
        {
            src_ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;

            if (src_ctrl_id == BRW_SELECT_CTRL_ID)
            {
                select_win_ptr = MMK_GetWinAddDataPtr(win_id);
                if ((PNULL ==select_win_ptr) || (PNULL == select_win_ptr->select_ptr))
                {
                    //SCI_TRACE_LOW:"WmlHandleSelectWinMsg ::PNULL ==select_ptr"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2160_112_2_18_2_7_6_538,(uint8*)"");
                    MMK_CloseWin(win_id);//error,返回上一级
                    break;
                }

                if (!BrwIsLayoutBelongToCurDisplay((void*)(select_win_ptr->layout_ctrl_ptr)))
                {
                   SCI_TRACE_LOW("[BRW]W WmlHandleSelectWinMsg :layout_ctrl_ptr error");
                   MMK_CloseWin(win_id);//error,返回上一级
                   break;
                } 

                if(!WmlSelectIsMultiple(select_win_ptr->select_ptr))
                {
                    //excuting the setting for single check
                    WmlHandleSelectedFinish(ctrl_id, select_win_ptr->select_ptr);
                    MMK_CloseWin(win_id);
                }
            }
        }
    }
        break;
#endif

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        do
        {
            select_win_ptr = MMK_GetWinAddDataPtr(win_id);
            if ((PNULL == select_win_ptr) || (PNULL == select_win_ptr->select_ptr))
            {
                //SCI_TRACE_LOW:"WmlHandleSelectWinMsg ::PNULL ==select_ptr"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2180_112_2_18_2_7_6_539,(uint8*)"");
    	        break;
    	    }

            if (!BrwIsLayoutBelongToCurDisplay((void*)(select_win_ptr->layout_ctrl_ptr)))
            {
                SCI_TRACE_LOW("[BRW]W XhtmlHandleSelectWinMsg :layout_ctrl_ptr error");
                break;
            } 

            if(WmlSelectIsMultiple(select_win_ptr->select_ptr)) 
            {
                //for multiple check, cancel means excuting the setting
                WmlHandleSelectedFinish(ctrl_id, select_win_ptr->select_ptr);
            }
            else
            {
                //for single check, cancel means do nothing
            }
        }while(0);
        
        MMK_CloseWin(win_id);//返回上一级
        break;
        
    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

//设置预选项
LOCAL void XhtmlPreSelectListControl(MMI_CTRL_ID_T ctrl_id, XHTML_LAYOUT_T *select_ptr)
{
    int32          item_num    = 0;
    int32          index       = 0;
    XHTML_LAYOUT_T *option_ptr  = NULL;

    if(PNULL ==select_ptr)
    {
        //SCI_TRACE_LOW:"XhtmlPreSelectListControl ::PNULL ==select_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2213_112_2_18_2_7_6_540,(uint8*)"");
        return;
    }

    item_num = XhtmlGetSelectItemNum(select_ptr);
    for( index=0; index<item_num; index++ ) {
        option_ptr = XhtmlSelectFindOptionByIndex(select_ptr, index);
        if(!option_ptr) {
            return;
        }
        if(XhtmlOptionIsChecked(option_ptr)) {
            GUILIST_SetSelectedItem(ctrl_id, index, TRUE);
        } else {
            GUILIST_SetSelectedItem(ctrl_id, index, FALSE);
        }
    }
}

/*
设置XHTML select窗口的列表
*/
LOCAL void SetupXhtmlListControl(MMI_CTRL_ID_T ctrl_id, XHTML_LAYOUT_T *layout_ptr)
{
    int32              index       = 0;
    int32              item_num    = 0;
    
    if(PNULL ==layout_ptr)
    {
        //SCI_TRACE_LOW:"SetupXhtmlListControl ::PNULL ==layout_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2240_112_2_18_2_7_6_541,(uint8*)"");
        return;
    }

    item_num = XhtmlGetSelectItemNum(layout_ptr);
    GUILIST_SetMaxItem(ctrl_id, XhtmlGetSelectItemNum(layout_ptr), FALSE);

    if(XhtmlSelectIsMultiple(layout_ptr)) {
        GUILIST_ChangeDisplayType(ctrl_id, GUILIST_CHECKLIST_E,FALSE);
        GUILIST_SetMaxSelectedItem(ctrl_id, item_num);
    } else {
        GUILIST_ChangeDisplayType(ctrl_id, GUILIST_RADIOLIST_E,FALSE);
    }
    //添加list item
    for(index=0; index<item_num; index++) {
        XHTML_LAYOUT_T      *option_ptr = NULL;
        uint16              *vdata_ptr  = NULL;
        GUILIST_ITEM_T      item_t      = {0};
        GUILIST_ITEM_DATA_T item_data   = {0};
        uint32              len         = 0;
        option_ptr = XhtmlSelectFindOptionByIndex(layout_ptr, index);
        
        if(PNULL ==option_ptr)
        {
            //SCI_TRACE_LOW:"SetupXhtmlListControl ::PNULL ==option_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2264_112_2_18_2_7_6_542,(uint8*)"");
            return;
        }
        
        item_t.item_data_ptr = &item_data;

        if(XhtmlSelectIsMultiple(layout_ptr)) {
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_CHECK;
            item_data.softkey_id[0] = (MMI_TEXT_ID_T)STXT_SELECT;
            item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;//TXT_NULL;
            item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;
        } else {
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
            item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_COMMON_OK;
            item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
            item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;
        }
        XhtmlCopyOptionContent(&vdata_ptr,option_ptr);
        len = BRW_WStrlen(vdata_ptr);
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = vdata_ptr;
        if(len>0) {
            //len = MIN(len, GUILIST_STRING_MAX_NUM);
        } else {
            len = 1;
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = L" ";
        }
        //SCI_ASSERT(len<=GUILIST_STRING_MAX_NUM );
        item_data.item_content[0].item_data.text_buffer.wstr_len = (uint16)len;
        item_data.item_content[0].item_data_type                 = GUIITEM_DATA_TEXT_BUFFER;
        if (XhtmlOptionIsDisabled(option_ptr))
        {
            item_t.item_state = GUIITEM_STATE_INVALID;
        }
        GUILIST_AppendItem(ctrl_id, &item_t);
        BRWMem_Free(vdata_ptr);
        vdata_ptr = NULL;
    }
    XhtmlPreSelectListControl(ctrl_id, layout_ptr);
}
/*
WML 选择完毕
*/
LOCAL BOOLEAN XhtmlHandleSelectedFinish(MMI_CTRL_ID_T ctrl_id,void *select_ptr, BOOLEAN *is_select_changed_ptr )
{
    int32                  item_num    = 0;
    int32                  index       = 0;
    const GUILIST_ITEM_T    *item_ptr   = NULL;
    XHTML_LAYOUT_T          *option_ptr = NULL;
    
    if (PNULL ==select_ptr || is_select_changed_ptr == NULL)
    {
        //SCI_TRACE_LOW:"XhtmlHandleSelectWinMsg ::PNULL ==select_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2314_112_2_18_2_7_6_543,(uint8*)"");
        return FALSE;
    }

    item_num = XhtmlGetSelectItemNum(select_ptr);
    for(index=0; index<item_num; index++) 
    {
        item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id,index);
        if(PNULL == item_ptr)
        {
            //SCI_TRACE_LOW:"XhtmlHandleSelectWinMsg ::PNULL ==item_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2324_112_2_18_2_7_6_544,(uint8*)"");
            return FALSE;
        }

        option_ptr = XhtmlSelectFindOptionByIndex(select_ptr, index);
        if (option_ptr != PNULL) 
        {
            if(GUILIST_GetItemState(item_ptr,GUIITEM_STATE_SELECTED)) 
            {
                if (!XhtmlOptionIsChecked(option_ptr))
                {
                    *is_select_changed_ptr = TRUE;
                }
                XhtmlSetOptionChecked(option_ptr, TRUE);
                if(!XhtmlSelectIsMultiple(select_ptr)) 
                {
                    XhtmlSetSelectNum(select_ptr,index);
                }
            } 
            else 
            {
                if (XhtmlOptionIsChecked(option_ptr))
                {
                    *is_select_changed_ptr = TRUE;
                }
                XhtmlSetOptionChecked(option_ptr, FALSE);
            }
        }
    }
    XhtmlSetSelectFinish(select_ptr);

    return TRUE;
}
/*
XHTML选择窗口的回调函数
*/
LOCAL MMI_RESULT_E XhtmlHandleSelectWinMsg(uint32 win_id,MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
    MMI_RESULT_E             recode      = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T            ctrl_id     = BRW_SELECT_CTRL_ID;
    XHTML_SELECT_WIN_INFO_T  *select_win_ptr = NULL;
    BOOLEAN                  is_select_changed = FALSE;

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        select_win_ptr = MMK_GetWinAddDataPtr(win_id);
        if(PNULL == select_win_ptr)
        {
            //SCI_TRACE_LOW:"[BRW]W XhtmlHandleSelectWinMsg ::PNULL==select_win_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2372_112_2_18_2_7_6_545,(uint8*)"");
        }
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    {
        select_win_ptr = (XHTML_SELECT_WIN_INFO_T  *)MMK_GetWinAddDataPtr(win_id);
    
        if ((PNULL ==select_win_ptr) || (PNULL == select_win_ptr->select_ptr))
        {
            //SCI_TRACE_LOW:"XhtmlHandleSelectWinMsg ::PNULL ==select_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2388_112_2_18_2_7_7_546,(uint8*)"");
            MMK_CloseWin(win_id);//error,返回上一级
            break;
        }

        if (!BrwIsLayoutBelongToCurDisplay((void*)(select_win_ptr->layout_ctrl_ptr)))
        {
           SCI_TRACE_LOW("[BRW]W XhtmlHandleSelectWinMsg :layout_ctrl_ptr error");
           MMK_CloseWin(win_id);//error,返回上一级
           break;
        } 

        if(XhtmlSelectIsMultiple(select_win_ptr->select_ptr)) 
        {
            //Multiple select:do nothing
        }
        else
        {
            //excuting the setting for single check
            XhtmlHandleSelectedFinish(ctrl_id, select_win_ptr->select_ptr, &is_select_changed);
            
#ifdef JS_SUPPORT
            if (is_select_changed) // onchange event happened
            {
                XhtmlProcSelectChangeEvent(select_win_ptr->layout_ctrl_ptr, select_win_ptr->select_ptr);
            }
#endif
            MMK_CloseWin(win_id);//返回上一级
        }		
    }
        break;
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    {
        MMI_CTRL_ID_T       src_ctrl_id = 0;

        if (PNULL != param)
        {
            src_ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;

            if (src_ctrl_id == BRW_SELECT_CTRL_ID)
            {
                select_win_ptr = MMK_GetWinAddDataPtr(win_id);
                if ((PNULL ==select_win_ptr) || (PNULL == select_win_ptr->select_ptr))
                {
                    //SCI_TRACE_LOW:"XhtmlHandleSelectWinMsg ::PNULL ==select_ptr"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2425_112_2_18_2_7_7_547,(uint8*)"");
                    MMK_CloseWin(win_id);//error,返回上一级
                    break;
                }

                if (!BrwIsLayoutBelongToCurDisplay((void*)(select_win_ptr->layout_ctrl_ptr)))
                {
                   SCI_TRACE_LOW("[BRW]W XhtmlHandleSelectWinMsg :layout_ctrl_ptr error");
                   MMK_CloseWin(win_id);//error,返回上一级
                   break;
                } 

                if(!XhtmlSelectIsMultiple(select_win_ptr->select_ptr))
                {
                    //excuting the setting for single check
                    XhtmlHandleSelectedFinish(ctrl_id, select_win_ptr->select_ptr, &is_select_changed);
#ifdef JS_SUPPORT
                    if (is_select_changed) // onchange event happened
                    {
                        XhtmlProcSelectChangeEvent(select_win_ptr->layout_ctrl_ptr, select_win_ptr->select_ptr);
                    }
#endif              
                    MMK_CloseWin(win_id);
                }
            }
        }
    }
    break;
#endif

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        do
        {
            select_win_ptr = MMK_GetWinAddDataPtr(win_id);
            if(PNULL == select_win_ptr || select_win_ptr->select_ptr == PNULL)
            {
                //SCI_TRACE_LOW:"XhtmlHandleSelectWinMsg ::PNULL ==select_ptr"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2451_112_2_18_2_7_7_548,(uint8*)"");
                break;
            }

            if (!BrwIsLayoutBelongToCurDisplay((void*)(select_win_ptr->layout_ctrl_ptr)))
            {
               SCI_TRACE_LOW("[BRW]W XhtmlHandleSelectWinMsg :layout_ctrl_ptr error");
               break;
            } 

            if(XhtmlSelectIsMultiple(select_win_ptr->select_ptr)) 
            {
                //for multiple check, cancel means excuting the setting
                XhtmlHandleSelectedFinish(ctrl_id, select_win_ptr->select_ptr, &is_select_changed);
            }
        }while(0);
        
        MMK_CloseWin(win_id);//返回上一级
        break;
        
    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************
设置编辑器的属性
format: input元素的format属性
method: [OUT]允许的输入法
input_mode: [out] 当前的输入模式，输入法中的一种(却不是同一个宏)
*****************************************************************************/
LOCAL uint32  ReadInputFormat(const uint8 *format,GUIIM_TYPE_T *method, GUIIM_TYPE_T *input_mode_ptr)
{
    uint32              len          = 0;
    GUIIM_TYPE_T        input_method = 0;
    GUIIM_TYPE_T        input_mode   = 0;
    if(PNULL ==method ||PNULL == input_mode_ptr)
    {
        //SCI_TRACE_LOW:"ReadInputFormat ::PNULL ==method ||PNULL == input_mode_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2484_112_2_18_2_7_7_549,(uint8*)"");
        return 0;
    }

    if(!format||*format=='\0') {
        len = BRW_INPUT_MAX_LEN;
        *method         = GUIIM_TYPE_SET_ALL;
        *input_mode_ptr = GUIIM_TYPE_SMART;
        return len;
    }
    while(1) {/*lint !e716*/
        if(*format=='\0') {
            break;
        } else if(*format=='*') {
            len = BRW_INPUT_MAX_LEN;
        } else if(*format=='A'||*format=='a') {
            input_method |= GUIIM_TYPE_ABC;
            input_mode = GUIIM_TYPE_ABC;
        } else if(*format=='N') {
            input_method |= GUIIM_TYPE_DIGITAL;
            input_mode = GUIIM_TYPE_DIGITAL;
        } else if(*format=='n') {
            input_method |= GUIIM_TYPE_DIGITAL | GUIIM_TYPE_ABC;/*lint !e655*/
            input_mode = GUIIM_TYPE_ABC;
        } else if(*format=='X'||*format=='x') {
            input_method |= GUIIM_TYPE_DIGITAL|GUIIM_TYPE_ABC;/*lint !e655*/
            input_mode = GUIIM_TYPE_ABC;
        } else if(*format=='M'||*format=='m') {
            input_method |= GUIIM_TYPE_SET_ALL;
            input_mode = GUIIM_TYPE_SMART;
        } else if(*format>='0' && *format<='9') {
            len = len*10 + (*format -'0');
        } else {
            input_method = GUIIM_TYPE_SET_ALL;
            input_mode   = GUIIM_TYPE_MULTITAP;
        }
        format++;       
    }
    if(len==0||len>BRW_INPUT_MAX_LEN) {
        len = BRW_INPUT_MAX_LEN;
    }
    if(!input_method || !input_mode) {
        input_method = GUIIM_TYPE_SET_ALL;
        input_mode   = GUIIM_TYPE_SMART;
    }
    *method         = input_method;
    *input_mode_ptr = input_mode;
    return len;
}
/*
WML网页输入窗口的回调函数
*/
LOCAL MMI_RESULT_E HandleWmlInputBoxWinMsg(uint32 win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_CTRL_ID_T           ctrl_id     = BRW_EDITBOX_CTRL_ID;
    MMI_RESULT_E            recode      = MMI_RESULT_TRUE;
    WML_INPUT_WIN_INFO_T      *input_win_ptr  = NULL;
    
    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
    {
        uint32  len = 0;
        
        input_win_ptr = MMK_GetWinAddDataPtr(win_id);
        if((PNULL == input_win_ptr) || (PNULL == input_win_ptr->input_ptr))
        {
            //SCI_TRACE_LOW:"HandleWmlInputBoxWinMsg ::PNULL ==input_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2547_112_2_18_2_7_7_550,(uint8*)"");
            break;
        }

        len = BRW_WStrlen(WmlGetInputTitle(input_win_ptr->input_ptr));
        if (0 == len) 
        {
            GUIWIN_SetTitleTextId(win_id, TXT_EDIT, FALSE);
        } 
        else 
        {
            GUIWIN_SetTitleText(win_id,WmlGetInputTitle(input_win_ptr->input_ptr),(uint16)len,FALSE);
        }
        MMK_SetAtvCtrl(win_id,ctrl_id);
    }
        break;
        
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
    {
        MMI_CTRL_ID_T       src_ctrl_id = 0;

        if (PNULL != param)
        {
            src_ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
            {
                MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
            }
            else if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
            {
                MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
            }
            else if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
            {
                MMK_PostMsg(win_id, MSG_CTL_MIDSK, PNULL, 0);
            }
       }
    }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#endif /* MMI_PDA_SUPPORT */

#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527    
#endif
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
       do
        {
            MMI_STRING_T  string_info = {0};
            
            input_win_ptr = MMK_GetWinAddDataPtr(win_id);
            if((PNULL == input_win_ptr) || (PNULL == input_win_ptr->input_ptr))
            {
                //SCI_TRACE_LOW:"HandleWmlInputBoxWinMsg :MSG_CTL_OK:PNULL ==input_ptr"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2600_112_2_18_2_7_7_551,(uint8*)"");
                break;
            }
                
            if (!BrwIsLayoutBelongToCurDisplay((void*)(input_win_ptr->layout_ctrl_ptr)))
            {
               SCI_TRACE_LOW("[BRW]W HandleWmlInputBoxWinMsg :layout_ctrl_ptr error");
               break;
            } 
            
            GUIEDIT_GetString(ctrl_id,&string_info);
            WmlSetInputValue(input_win_ptr->input_ptr, string_info.wstr_ptr);
        }while(0);
       
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}
/*
XHTML网页输入窗口的回调函数
*/
LOCAL MMI_RESULT_E HandleXhtmlInputBoxWinMsg(uint32 win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_CTRL_ID_T       ctrl_id     = BRW_EDITBOX_CTRL_ID;
    MMI_RESULT_E        recode      = MMI_RESULT_TRUE;
    XHTML_INPUT_WIN_INFO_T    *input_win_ptr  = PNULL;

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        input_win_ptr = MMK_GetWinAddDataPtr(win_id);
        if(PNULL == input_win_ptr)
        {
            //SCI_TRACE_LOW:"[BRW]W HandleXhtmlInputBoxWinMsg ::PNULL ==input_win_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2638_112_2_18_2_7_7_552,(uint8*)"");
        }
        GUIWIN_SetTitleTextId(win_id, TXT_EDIT, FALSE);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
    {
        MMI_CTRL_ID_T       src_ctrl_id = 0;

        if (PNULL != param)
        {
            src_ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
            {
                MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
            }
            else if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
            {
                MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
            }
            else if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
            {
                MMK_PostMsg(win_id, MSG_CTL_MIDSK, PNULL, 0);
            }
       }
    }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#endif /* MMI_PDA_SUPPORT */

#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527    
#endif
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        do 
        {
            MMI_STRING_T string_info = {0};
#ifdef JS_SUPPORT
            BOOLEAN is_value_equal = FALSE;
#endif

            input_win_ptr = MMK_GetWinAddDataPtr(win_id);
           
            if((PNULL == input_win_ptr) || (PNULL == input_win_ptr->layout_ptr))
            {
               //SCI_TRACE_LOW:"HandleXhtmlInputBoxWinMsg :MSG_CTL_OK:PNULL ==input_win_ptr"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2683_112_2_18_2_7_7_553,(uint8*)"");
               break;
            }
            
            if (!BrwIsLayoutBelongToCurDisplay((void*)(input_win_ptr->layout_ctrl_ptr)))
            {
               SCI_TRACE_LOW("[BRW]W HandleXhtmlInputBoxWinMsg :layout_ctrl_ptr error");
               break;
            } 

            if (XhtmlInputIsDisable(input_win_ptr->layout_ptr) || XhtmlInputIsReadOnly(input_win_ptr->layout_ptr)) 
            {
                break;
            }
            GUIEDIT_GetString(ctrl_id,&string_info);
#ifdef JS_SUPPORT
            is_value_equal = XhtmlCmpInputValue(input_win_ptr->layout_ptr,string_info.wstr_ptr);
#endif
            XhtmlSetInputValue(input_win_ptr->layout_ptr, string_info.wstr_ptr);

            //todo: isindex的要发出GET请求
#ifdef JS_SUPPORT
            if (is_value_equal) // onchange event happened
            {
                XhtmlProcInputChangeEvent(input_win_ptr->layout_ctrl_ptr, input_win_ptr->layout_ptr);
            }
#endif
        } while(0);

        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData(win_id);
        break;
            
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/******************************************************************************************
* DESCRIPTION:
*   create WML edit windows for <input> element
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   input_ptr: [IN] Handle to WML <input> layout
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
PUBLIC void WmlLayoutCreateEditWindow(WML_LAYOUT_INPUT_T *input_ptr, LAYOUT_CTRL_T  *layout_ctrl_ptr)
{
    MMI_HANDLE_T    handle          = MMI_INVALID_ID;
    uint32          len             = 0;
    MMI_CTRL_ID_T   ctrl_id         = BRW_EDITBOX_CTRL_ID;
    MMI_WIN_ID_T    win_id          = BRW_EDITBOX_WIN_ID;
    IGUICTRL_T      *ctrl_ptr       = NULL;
    GUIIM_TYPE_T    allow_im        = 0;
    GUIIM_TYPE_T    init_im         = 0;
    WML_INPUT_WIN_INFO_T  *input_win_ptr = NULL;
	
    if (PNULL == input_ptr || PNULL == layout_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"WmlLayoutCreateEditWindow ::PNULL ==input_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2751_112_2_18_2_7_7_554,(uint8*)"");
        return;
    }
    input_win_ptr = SCI_ALLOCAZ(sizeof(WML_INPUT_WIN_INFO_T));//the add_data_ptr of window,use SCI_***
    if (PNULL == input_win_ptr)
    {
        SCI_TRACE_LOW("WmlLayoutCreateEditWindow ::input_win_ptr ALLOC FAIL");
        return;
    }
    input_win_ptr->input_ptr = input_ptr;
    input_win_ptr->layout_ctrl_ptr = layout_ctrl_ptr;

    if(!MMK_IsOpenWin(win_id)) 
    {
        WINDOW_TABLE(WIN_TABLE)     = {
                                        WIN_FUNC((uint32)HandleWmlInputBoxWinMsg),
                                        WIN_ID(BRW_EDITBOX_WIN_ID),
                                        WIN_TITLE(TXT_EDIT),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif /* MMI_PDA_SUPPORT */
                                        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
                                        END_WIN};

        MMI_CONTROL_CREATE_T    create    = {0};
        GUIEDIT_INIT_DATA_T     init_data = {0};
        uint16 *value_ptr = PNULL;
        uint32 value_len = 0;

        handle = MMK_CreateWin((uint32*)WIN_TABLE, input_win_ptr);
        if(!handle||(MMI_INVALID_ID==handle)) 
        {
            return;
        }
        len = ReadInputFormat(WmlGetInputFormat(input_ptr), &allow_im, &init_im);
        if(WmlGetInputMaxLength(input_ptr)>0) 
        {
            len = WmlGetInputMaxLength(input_ptr);
            len = MIN(len,BRW_INPUT_MAX_LEN);
        }

        init_data.type = WmlInputIsPasswordType(input_ptr) ? GUIEDIT_TYPE_PASSWORD : GUIEDIT_TYPE_TEXT;
        init_data.both_rect = MMITHEME_GetWinClientBothRect(handle);//MS00200232
        init_data.str_max_len = (uint16)len;

        create.ctrl_id           = ctrl_id;
        create.guid              = SPRD_GUI_EDITBOX_ID;
        create.parent_win_handle = handle;
        create.init_data_ptr     = &init_data;

        ctrl_ptr = MMK_CreateControl(&create);

        if(PNULL == ctrl_ptr) 
        {
            MMK_CloseWin(handle);
            return;
        }

        //reset the allow im and init im for password
        if ((GUIEDIT_TYPE_PASSWORD == init_data.type) && (PNULL == WmlGetInputFormat(input_ptr)))
        {
            allow_im = GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;/*lint !e655*/
            init_im = GUIIM_TYPE_ABC;
        }
        GUIEDIT_SetIm(ctrl_id, allow_im, init_im);
        GUIEDIT_SetAlign(ctrl_id, ALIGN_LEFT);//MS00253994 靠左上起
        
        value_len = (uint32)BRW_WStrlen(WmlGetInputValue(input_ptr));
        if(value_len > 0) 
        {
            if(value_len > len) 
            {
                value_len = len;
            }
            value_ptr = (uint16*)WmlGetInputValue(input_ptr);
            GUIEDIT_SetString(ctrl_id, value_ptr,(uint16)value_len);
        }

        //GUIEDITBOX_SetEditBoxStyle( ctrl_id, MMI_DEFAULT_TEXT_FONT, MMI_WHITE_COLOR, MMI_BACKGROUND_COLOR);
    }
}

/******************************************************************************************
* DESCRIPTION:
*   create HTML edit windows for <input> element
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   input_ptr: [IN] Handle to HTML <input> layout
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
PUBLIC void XhtmlLayoutCreateEditWindow(XHTML_LAYOUT_T *layout_ptr, LAYOUT_CTRL_T  *layout_ctrl_ptr)
{
    MMI_HANDLE_T  handle        = MMI_INVALID_ID;
    uint32        len = 0;
    MMI_CTRL_ID_T ctrl_id       = BRW_EDITBOX_CTRL_ID;
    MMI_WIN_ID_T  win_id        = BRW_EDITBOX_WIN_ID;
    IGUICTRL_T    *ctrl_ptr     = PNULL;
    GUIIM_TYPE_T  allow_im      = 0;
    GUIIM_TYPE_T  init_im       = 0;
    XHTML_INPUT_WIN_INFO_T  *input_win_ptr = NULL;

    if (PNULL == layout_ptr || PNULL == layout_ctrl_ptr)
    {
        //BRWDebug_Printf:"[BRW]W XhtmlLayoutCreateEditWindow:param error, layout_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2846_112_2_18_2_7_7_555,(uint8*)"d", layout_ptr);
        return;
    }
    input_win_ptr = SCI_ALLOCAZ(sizeof(XHTML_INPUT_WIN_INFO_T));//the add_data_ptr of window,use SCI_***
    if (PNULL == input_win_ptr)
    {
        //BRWDebug_Printf:"[BRW]W XhtmlLayoutCreateEditWindow ::input_win_ptr ALLOC FAIL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2852_112_2_18_2_7_7_556,(uint8*)"");
        return;
    }
    input_win_ptr->layout_ptr = layout_ptr;
    input_win_ptr->layout_ctrl_ptr = layout_ctrl_ptr;

    if(!MMK_IsOpenWin(win_id)) 
    {
        WINDOW_TABLE(WIN_TABLE) = {
                    WIN_FUNC((uint32)HandleXhtmlInputBoxWinMsg),
                    WIN_ID(BRW_EDITBOX_WIN_ID),
                    WIN_TITLE(TXT_EDIT),
#ifdef MMI_PDA_SUPPORT
                    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif /* MMI_PDA_SUPPORT */
                    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
                    END_WIN
                };
        MMI_CONTROL_CREATE_T    create    = {0};
        GUIEDIT_INIT_DATA_T     init_data = {0};
        uint16 *value_ptr = PNULL;
        uint32 value_len = 0;

        handle = MMK_CreateWin((uint32*)WIN_TABLE, input_win_ptr);
        if (!handle || (MMI_INVALID_ID == handle)) 
        {
            //BRWDebug_Printf:"[BRW]W XhtmlLayoutCreateEditWindow:handle error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2878_112_2_18_2_7_7_557,(uint8*)"");
            SCI_FREE(input_win_ptr);
            return;
        }
        len = ReadInputFormat(XhtmlGetInputFormat(layout_ptr), &allow_im, &init_im);
        if (XhtmlGetInputMaxLength(layout_ptr)) 
        {
            len = XhtmlGetInputMaxLength(layout_ptr);
            len = MIN(len, BRW_INPUT_MAX_LEN);
        }

        init_data.type = XhtmlInputIsPasswordType(layout_ptr) ? GUIEDIT_TYPE_PASSWORD: GUIEDIT_TYPE_TEXT;
        init_data.both_rect = MMITHEME_GetWinClientBothRect(handle);//MS00200232
        init_data.str_max_len = len;

        create.ctrl_id           = ctrl_id;
        create.guid              = SPRD_GUI_EDITBOX_ID;
        create.parent_win_handle = handle;
        create.init_data_ptr     = &init_data;

        ctrl_ptr = MMK_CreateControl(&create);

        if(PNULL == ctrl_ptr) 
        {
            MMK_CloseWin(handle);//free input_win_ptr in msg_close_window
            return;
        }
        
        //reset the allow im and init im for password
        if ((GUIEDIT_TYPE_PASSWORD == init_data.type) && (PNULL == XhtmlGetInputFormat(layout_ptr)))
        {
            allow_im = GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;/*lint !e655*/
            init_im = GUIIM_TYPE_ABC;
        }
        GUIEDIT_SetIm(ctrl_id, allow_im, init_im);
        GUIEDIT_SetAlign(ctrl_id, ALIGN_LEFT);//MS00253994 靠左上起

        value_len = (uint32)BRW_WStrlen(XhtmlGetInputValue(layout_ptr));
        if(value_len > 0) 
        {
            if(value_len > len) 
            {
                value_len = len;
            }
            value_ptr = (uint16*)XhtmlGetInputValue(layout_ptr);
            GUIEDIT_SetString(ctrl_id,value_ptr,(uint16)value_len);
        }

        //GUIEDITBOX_SetEditBoxStyle( ctrl_id, MMI_DEFAULT_TEXT_FONT, MMI_WHITE_COLOR, MMI_BACKGROUND_COLOR);
    }
    else
    {
        SCI_FREE(input_win_ptr);
    }
}
/*****************************************************************************/
//  Description : open form file win callback
//  Global resource dependence : none
//  Author: li.li
//  Note:
/*****************************************************************************/
LOCAL void XhtmlOpenFormFileWinCallBack(
                                      BOOLEAN   is_success, 
                                      FILEARRAY file_array
                                      )
{
    if (is_success)
    {
        if (0 < MMIAPIFILEARRAY_GetArraySize(file_array))
        {
            
            FILEARRAY_DATA_T          file_array_data = {0};
            
            //read user selected dir
            MMIAPIFILEARRAY_Read( file_array, 0, &file_array_data );

			if(s_select_path_ptr != PNULL)
			{
				SCI_MEMSET(s_select_path_ptr,0,SFS_MAX_PATH + 1);
            	MMIAPICOM_Wstrncpy(s_select_path_ptr,file_array_data.filename,MIN(file_array_data.name_len,SFS_MAX_PATH + 1));
				s_select_path_ptr = PNULL;
			}
        }
        else
        {
            //SCI_TRACE_LOW:"XhtmlOpenFormFileWinCallBack array size == 0"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2950_112_2_18_2_7_8_558,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"XhtmlOpenFormFileWinCallBack failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_2955_112_2_18_2_7_8_559,(uint8*)"");
    }

    MMIAPIFILEARRAY_Destroy(&file_array);
    file_array = PNULL;
}

/*****************************************************************************/
//  Description : open form path win
//  Global resource dependence : none
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC void XhtmlOpenFormFileWin(uint16   *selected_path_ptr)
{
    FILEARRAY                   ret_array    = MMIAPIFILEARRAY_Create();
    MMIFMM_FILTER_T             filter       = {0};

    //set list enable mark
    SCI_MEMCPY(filter.filter_str, "*", sizeof("*"));

	s_select_path_ptr = selected_path_ptr;
	
    if(FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(PNULL,
        0,
        &filter, FUNC_FIND_FILE_AND_ALLFOLER,
        FALSE,
        PNULL,
        ret_array,
        XhtmlOpenFormFileWinCallBack,
        FALSE))
    {
        MMIAPIFILEARRAY_Destroy(&ret_array);
    }
   
}


/******************************************************************************************
* DESCRIPTION:
*   create a window for user to selecting item in WML web page.
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   select_ptr: [IN] Handle to the WML <select> layout.
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
PUBLIC void WmlLayoutCreateSelectWindow(WML_LAYOUT_SELECT_T *select_ptr, LAYOUT_CTRL_T  *layout_ctrl_ptr)
{
    MMI_CTRL_ID_T ctrl_id = BRW_SELECT_CTRL_ID;
    WML_SELECT_WIN_INFO_T  *select_win_ptr = PNULL;
   
    WINDOW_TABLE(SELECT_WIN_TABLE) = {
            WIN_FUNC((uint32)WmlHandleSelectWinMsg),
            WIN_ID(BRW_SELECT_WIN_ID),
            WIN_TITLE(STXT_SELECT),
#ifdef MMI_PDA_SUPPORT
    //WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
            WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else    
            WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif /* MMI_PDA_SUPPORT */
            END_WIN
    };

    if(PNULL == select_ptr || PNULL == layout_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"XhtmlLayoutCreateEditWindow ::PNULL ==select_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_3030_112_2_18_2_7_8_560,(uint8*)"");
        return;
    }

	select_win_ptr = SCI_ALLOCAZ(sizeof(WML_SELECT_WIN_INFO_T));//the add_data_ptr of window,use SCI_***
    if (PNULL == select_win_ptr )
    {
        SCI_TRACE_LOW("WmlLayoutCreateSelectWindow ::PNULL ==select_ptr");
        return;
    }
    select_win_ptr->layout_ctrl_ptr = layout_ctrl_ptr;
    select_win_ptr->select_ptr = select_ptr;
	
    if(!MMK_IsOpenWin(BRW_SELECT_WIN_ID)) 
    {
        uint16              title_len       = 0;
        MMI_HANDLE_T        handle          = MMI_INVALID_ID;
        GUILIST_INIT_DATA_T     init_data   = {0};

        handle = MMK_CreateWin((uint32*)&SELECT_WIN_TABLE, select_win_ptr);

        if(!handle||MMI_INVALID_ID==handle)
        {
            return;
        }

        /*multiple or single list*/
        if(WmlSelectIsMultiple(select_ptr)) 
        {
            init_data.type = GUILIST_CHECKLIST_E;
        }
        else 
        {
            init_data.type = GUILIST_RADIOLIST_E;
        }
        /*init data*/
        init_data.both_rect = MMITHEME_GetWinClientBothRect(handle);//MS00200232
        init_data.is_dynamic_item_data  = TRUE;
        init_data.max_item_num          = WmlGetSelectItemNum(select_ptr);
        init_data.max_selected_num      = init_data.max_item_num;
        
        if(0 == GUILIST_CreateListBox(handle, 0, ctrl_id, &init_data)) 
        {
            MMK_CloseWin(handle);
            return;
        }

        /*setup list item*/
        SetupWmlListControl(ctrl_id,select_ptr);

        /*set list title*/
        title_len = (uint16)BRW_WStrlen(WmlGetSelectTitle(select_ptr));
        if(title_len) 
        {
            GUIWIN_SetTitleText(handle, WmlGetSelectTitle(select_ptr),title_len,FALSE);
        }
    }
}

/******************************************************************************************
* DESCRIPTION:
*   page create a window for user to selecting item in HTML web.
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   select_ptr: [IN] Handle to the HTML <select> layout.
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
PUBLIC void XhtmlLayoutCreateSelectWindow(XHTML_LAYOUT_T *select_ptr, LAYOUT_CTRL_T  *layout_ctrl_ptr)
{
    MMI_WIN_ID_T  win_id  = BRW_SELECT_WIN_ID;
    MMI_CTRL_ID_T ctrl_id = BRW_SELECT_CTRL_ID;
    XHTML_SELECT_WIN_INFO_T  *select_win_ptr = PNULL;

    WINDOW_TABLE(SELECT_WIN_TABLE) = {
            WIN_FUNC((uint32)XhtmlHandleSelectWinMsg),
            WIN_ID(BRW_SELECT_WIN_ID),
            WIN_TITLE(STXT_SELECT),
#ifdef MMI_PDA_SUPPORT
            //WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
            WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
            WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif /* MMI_PDA_SUPPORT */
            END_WIN
    };

    if(PNULL == select_ptr || layout_ctrl_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"XhtmlLayoutCreateSelectWindow ::PNULL ==select_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_3115_112_2_18_2_7_8_561,(uint8*)"");
        return;
    }

    select_win_ptr = SCI_ALLOCAZ(sizeof(XHTML_SELECT_WIN_INFO_T));//the add_data_ptr of window,use SCI_***
    if (PNULL == select_win_ptr )
    {
        //BRWDebug_Printf:"[BRW]W XhtmlLayoutCreateSelectWindow ::select_win_ptr ALLOC FAIL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_3122_112_2_18_2_7_8_562,(uint8*)"");
        return;
    }

    select_win_ptr->layout_ctrl_ptr = layout_ctrl_ptr;
    select_win_ptr->select_ptr = select_ptr;

    if(!MMK_IsOpenWin(win_id)) 
    {
        MMI_HANDLE_T        handle          = MMI_INVALID_ID;
        GUILIST_INIT_DATA_T     init_data   = {0};
        

        handle = MMK_CreateWin((uint32*)&SELECT_WIN_TABLE, select_win_ptr);

        if(!handle||MMI_INVALID_ID==handle) 
        {
            //BRWDebug_Printf:"[BRW]W XhtmlLayoutCreateSelectWindow:handle error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_3139_112_2_18_2_7_8_563,(uint8*)"");
            SCI_FREE(select_win_ptr);
            return;
        }
        /*multiple or single select*/
        if(XhtmlSelectIsMultiple(select_ptr)) 
        {
            init_data.type = GUILIST_CHECKLIST_E;
        } 
        else
        {
            init_data.type = GUILIST_RADIOLIST_E;
        }

        init_data.both_rect = MMITHEME_GetWinClientBothRect(handle);//MS00200232
        init_data.is_dynamic_item_data  = TRUE;
        init_data.max_item_num          = XhtmlGetSelectItemNum(select_ptr);
        init_data.max_selected_num      = init_data.max_item_num;
        
        if(0 == GUILIST_CreateListBox(handle, 0, ctrl_id, &init_data)) 
        {
            //BRWDebug_Printf:"[BRW]W XhtmlLayoutCreateSelectWindow:GUILIST_CreateListBox error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_3160_112_2_18_2_7_8_564,(uint8*)"");
            MMK_CloseWin(win_id);//free input_win_ptr in msg_close_window
            return;
        }
        /*setup list item*/
        SetupXhtmlListControl(ctrl_id,select_ptr);
    }
    else
    {
        SCI_FREE(select_win_ptr);
    }
}

/*****************************************************************************/
//  Description : play gif
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
BOOLEAN BrwPlayGif(uint8 *data_ptr, uint32 data_len, IMAGE_PLAY_INFO *info_ptr, int16 x, int16 y, GUI_RECT_T *rect_ptr)
{
    BOOLEAN                     is_first_decode = FALSE;
    GUI_POINT_T                 dis_point = {0};
    IMG_DEC_RET_E               gif_ret = IMG_DEC_RET_SUCCESS ;
    IMG_DEC_FRAME_IN_PARAM_T    in_param = {0};
    IMG_DEC_FRAME_OUT_PARAM_T   out_param = {0};
    IMG_DEC_SRC_T   img_dec_src = {0};
    BOOLEAN is_need_zoom_in = FALSE;
    IMG_DEC_INFO_T  img_info_param ={0};
    uint32  target_width = 0;
    uint32  target_height = 0;
    uint32  original_width = 0;
    uint32  original_height = 0;
    BOOLEAN result = FALSE;

    if ((PNULL == info_ptr) || (PNULL == data_ptr) || (PNULL == rect_ptr))
    {
        //BRWDebug_Printf:"[BRW]W BrwPlayGif input invalid!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_3195_112_2_18_2_7_8_565,(uint8*)"");
        return FALSE;
    }
    
    target_width = info_ptr->width;
    target_height = info_ptr->height;
    if ((0 == target_width) || (0 == target_height))
    {
        //BRWDebug_Printf:"[BRW]W BrwPlayGif:target_width =%d, target_height =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_3203_112_2_18_2_7_8_566,(uint8*)"dd", target_width, target_height);
        return FALSE;
    }
    
    img_dec_src.src_file_size = data_len;
    img_dec_src.src_ptr = data_ptr;
    IMG_DEC_GetInfo(&img_dec_src, &img_info_param);//get gif info
    original_width = img_info_param.img_detail_info.gif_info.img_width;
    original_height = img_info_param.img_detail_info.gif_info.img_height;

    if (0 == info_ptr->gif_handle ) 
    {
        if (IMG_DEC_RET_SUCCESS != IMG_DEC_Create(&img_dec_src, &info_ptr->gif_handle))
        {
            IMG_DEC_Destroy(info_ptr->gif_handle);
            //BRWDebug_Printf:"[BRW]W BrwPlayGif:IMG_DEC_Create fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_3217_112_2_18_2_7_8_567,(uint8*)"");
            return FALSE;            
        }
        is_first_decode = TRUE;
    }
    
    in_param.callback = PNULL;
    in_param.write_data = PNULL;
    in_param.handle = info_ptr->gif_handle;
    in_param.data_format = IMG_DEC_RGB565;
    in_param.target_width = target_width;
    in_param.target_height = target_height;
    in_param.target_buf_size = in_param.target_width * in_param.target_height * 2;
    in_param.target_ptr = BRWMem_Alloc(in_param.target_buf_size);
    if (PNULL == in_param.target_ptr) 
    {
        IMG_DEC_Destroy(info_ptr->gif_handle);
        //BRWDebug_Printf:"[BRW]W BrwPlayGif:target_ptr alloc fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_3233_112_2_18_2_7_8_568,(uint8*)"");
       return FALSE;
    }    
    
    gif_ret = IMG_DEC_GetFrame(&in_param, &out_param);//get gif one frame decode data:IMG_DEC_GetFrame can zoom out,can't zoom in

    //NEWMS00179769:某帧错误则结束，重新解码播放
    if ((IMG_DEC_RET_SUCCESS != gif_ret) && !is_first_decode)
    {
        BOOLEAN is_finish = FALSE;
        
        IMG_DEC_Destroy(info_ptr->gif_handle);
        info_ptr->gif_handle = PNULL;

        if (IMG_DEC_RET_SUCCESS == IMG_DEC_Create(&img_dec_src, &info_ptr->gif_handle))
        {
            is_finish = (2 == out_param.current_frame)? TRUE: FALSE;//第二帧就错误，则认为本gif只有一帧
            
            is_first_decode = TRUE;
            in_param.handle = info_ptr->gif_handle;
            gif_ret = IMG_DEC_GetFrame(&in_param, &out_param);
            
            if (is_finish)
            {
                out_param.is_decode_finished = TRUE;
            }
        }
        else
        {
            //BRWDebug_Printf:"[BRW]W BrwPlayGif:target_ptr alloc fail2"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_3262_112_2_18_2_7_8_569,(uint8*)"");
        }
    }

    if (IMG_DEC_RET_SUCCESS == gif_ret) 
    {
        GUIIMG_BITMAP_T  handle = {0};
        ZOOM_INPUT_T    zoom_in_data = {0};
        ZOOM_OUTPUT_T   zoom_out_data = {0};
        BOOLEAN is_zoom_success = FALSE;

        if (!info_ptr->is_not_need_zoom_in)
        {
            if ((target_width > original_width) || (target_height > original_height))
            {
                is_need_zoom_in = TRUE;//target > original
            }
            //BRWDebug_Printf:"[BRW]BrwPlayGif:is_need_zoom_in = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_3277_112_2_18_2_7_8_570,(uint8*)"d", is_need_zoom_in);
        }
            
        if (!is_need_zoom_in)
        {
            handle.bit_ptr    = in_param.target_ptr;
            handle.img_width  = out_param.real_width;
            handle.img_height = out_param.real_height;
        }
        else
        {            
            zoom_in_data.src_buf_ptr = in_param.target_ptr;
            zoom_in_data.src_buf_size = in_param.target_buf_size;
            zoom_in_data.src_img_width = out_param.real_width;
            zoom_in_data.src_img_height= out_param.real_height;
            zoom_in_data.dest_img_width = target_width;
            zoom_in_data.dest_img_height = target_height;
            zoom_in_data.dest_buf_size = (zoom_in_data.dest_img_width * zoom_in_data.dest_img_height * 2 + 255);
            zoom_in_data.dest_buf_ptr = BRWMem_Alloc(zoom_in_data.dest_buf_size);
            if (PNULL != zoom_in_data.dest_buf_ptr)
            {
                if (ZOOM_RESULT_SUCC == BrwZoomImage(&zoom_in_data, &zoom_out_data))
                {
                    is_zoom_success = TRUE;
                }
                else
                {
                    is_zoom_success = FALSE;
                }
            }
            else
            {
                //BRWDebug_Printf:"[BRW]W BrwPlayGif:ZOOM Alloc fail,zoom_in_data.dest_buf_size = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_3308_112_2_18_2_7_8_571,(uint8*)"d", zoom_in_data.dest_buf_size);
                is_zoom_success = FALSE;
            }    

            if (is_zoom_success)
            {
                handle.bit_ptr    = (zoom_in_data.dest_buf_ptr + zoom_out_data.dest_buf_offset);
                handle.img_width  = zoom_out_data.img_width;
                handle.img_height = zoom_out_data.img_height;
            }
            else//zoom fail, use the original image to display
            {
                handle.bit_ptr    = in_param.target_ptr;
                handle.img_width  = out_param.real_width;
                handle.img_height = out_param.real_height;
            }
        }

        dis_point.x = x;
        dis_point.y = y;
        result = GUIIMG_DisplayBmp(FALSE, rect_ptr, &dis_point, &handle, &lcd_dev_info);

        BRWMem_Free(zoom_in_data.dest_buf_ptr);
        zoom_in_data.dest_buf_ptr = PNULL;
    }
    else
    {
        if (is_first_decode)
        {
            result = FALSE; 
        }
        else
        {
            result = TRUE;/*for NEWMS00124655,中间遇到某fram错误,仍应继续处理*/
        }
    }
    
    if(is_first_decode)
    {
        info_ptr->frame_delay = (uint16)out_param.delay_time;
        if (out_param.is_decode_finished)
        {
            //single frame, do not start timer for GIF playing, set delay timer to invalid value.
            info_ptr->frame_delay = 0;
        }
        else if(0 == info_ptr->frame_delay)
        {
            info_ptr->frame_delay = 200;//protect for when the gif delay timer is 0
        }
    }
    
    //decode finish
    if (gif_ret != IMG_DEC_RET_SUCCESS 
        || out_param.is_decode_finished
        )
    {
        IMG_DEC_Destroy(info_ptr->gif_handle);
        info_ptr->gif_handle = PNULL;
    }
    
    BRWMem_Free(in_param.target_ptr);
    in_param.target_ptr = PNULL;

    return result;
}

void BrwStopGif(uint32 handle)
{
#ifdef GIF_DEC_SUPPORT_GIF_SW        
    if(handle) {
        IMG_DEC_Destroy(handle);
    }
#endif
}

/************************************************************************
                   scroll bar for layout
************************************************************************/
#if 0
/************************************************************************
* function:
      move scroll bar to new position, only use for Arab
* input:
      scroll_handle: scroll handle
      rect_ptr: the new rector region of the scroll bar
************************************************************************/
void  BrwMoveScrollBarWindow(void *scroll_handle, RECT_T *rect_ptr)
{
    GUI_RECT_T  rect    = {0};
    if(!scroll_handle||!rect_ptr) {
        return;
    }

    GUIPRGBOX_GetRectByPtr( scroll_handle, &rect);
    GUI_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    rect.left   = (int16)rect_ptr->left;
    rect.top    = (int16)rect_ptr->top;
    rect.right  = (int16)rect_ptr->right;
    rect.bottom = (int16)rect_ptr->bottom;
    GUIPRGBOX_ResizeRectByPtr(scroll_handle, rect);
    GUIPRGBOX_UpdateByPtr(scroll_handle);
}
#endif
/************************************************************************
* function:
      set scroll bar new scroll info
* input:
      total_item_num: total item numbers in scroll bar
      page_item_num:  one page item numbers in scroll bar
************************************************************************/
void  BrwSetScrollBarInfo(void *scroll_handle, uint32 total_item_num, uint32 page_item_num, uint32 pos)
{
//    GUI_RECT_T  rect = {0};

    if(!scroll_handle) {
        return;
    }

    if (0 == total_item_num)
    {
        total_item_num = page_item_num;
    }
    page_item_num = MIN(total_item_num, page_item_num);

//     GUIPRGBOX_GetRectByPtr(scroll_handle, &rect);
//     GUI_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    if (total_item_num > page_item_num)
    {
        GUIPRGBOX_SetParamByPtr(scroll_handle,total_item_num,page_item_num);
        GUIPRGBOX_SetPosByPtr(TRUE, 0, pos,scroll_handle);
    }
    else
    {
        GUIPRGBOX_SetParamByPtr(scroll_handle,0,0);
        GUIPRGBOX_SetPosByPtr(TRUE, 0, 0,scroll_handle);
    }
}

/******************************************************************************************
* DESCRIPTION:
*   Get string information in line with direction, only encapsulation of GUI function
*
* AUTHOR:
*   jiqun.li
*
* RETURN:
*   BOOLEAN
*
* NOTE:
******************************************************************************************/
BOOLEAN BrwGetStringInfoInLineWithDir(WAP_STRING_FONT_TEXT_T str,int32 width, uint16 *char_num_ptr, uint16 *pixel_num_ptr, BOOLEAN is_byword)
{
    int8 dir = IS_WAP_BIT_RTL(str.str_info) ? 1/*TEXT_DIR_RTL*/ : 0/*TEXT_DIR_LTR*/;
    GUISTR_STYLE_T   style = {0};
    GUISTR_STATE_T   state = 0;
    BOOLEAN  result = FALSE;
    style.font = (GUI_FONT_T)ConvertWapFontSizeToMMISize(str.font.font_type);
    style.word_space = str.text_style.word_space;
    if (str.font.is_font_italic)
    {
        state = GUISTR_STATE_ITALIC;
    }
    if (str.font.is_font_bold)
    {
        state |= GUISTR_STATE_BOLD;
    }
    if (str.font.is_font_underline)
    {
        state |= GUISTR_STATE_UNDERLINE;
    }
    if (str.text_style.is_space_valid)
    {
        state |= GUISTR_STATE_WORDSPACE;
    }
    if (is_byword)
    {
        state |= GUISTR_STATE_WORDBREAK;
    }
     
    result = GUISTR_LineBreak(dir, str.str_ptr, str.str_len, &style,state,(uint16)width, char_num_ptr, pixel_num_ptr);
    //add by zhewei 
    if(result)
    {
        MMI_STRING_T string = {0};
        uint32             re_width = 0; 
        string.wstr_ptr = str.str_ptr;
        string.wstr_len = *char_num_ptr;
        re_width = GUISTR_GetStringWidth(&style,&string,state);
        if (re_width >width)
        {
            SCI_TRACE_LOW("re_width=%d width=%d,len= %d,pixel:%d",re_width,width,*char_num_ptr,*pixel_num_ptr);
        }
        
    }
    return result;
}

#if 0
/******************************************************************************************
* DESCRIPTION:
*   Test BrwDisplayXXX & BrwGraphicsDrawXXX
*
* AUTHOR:
*   bin.ji
*
* PARAMETERS:
*   
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
PUBLIC void BrwDisplayTest(
                            int32 select_num
                            )
{
    RECT_T region = {0, 0, 319, 379};
    
    switch (select_num)
    {
    case 0:
        {
            int16 x1 = 0;
            int16 x2 = 319;
            int16 y1 = 0;

            for (y1 = 0; y1 < 300; y1+=10)
            {
                BrwGraphicsDrawHLine(x1, y1, x2, MMI_RED_COLOR, &region);
            }
        }
        break;
        
    case 1:
        {
            int16 x1 = 0;
            int16 y1 = 0;
            int16 y2 = 319;

            for (x1 = 0; x1 < 300; x1+=10)
            {
                BrwGraphicsDrawVLine(x1, y1, y2, MMI_RED_COLOR, &region);
            }
        }
        break;
        
    case 2:
        {
            int16 x1 = 0;
            int16 y1 = 0;
            int16 x2 = 100;
            int16 y2 = 300;

            for (; x1 < 200; x1+=10, x2+=10)
            {
                BrwGraphicsDrawLine(x1, y1, x2, y2, MMI_RED_COLOR, &region);
            }
        }
        break;
        
    case 3:
        {
            BRW_POINT_T first_point = {0, 0};
            BRW_POINT_T second_point = {100, 300};
            BRW_PEN_STYLE_E style = BRW_PEN_STYLE_DEFAULT;

            for (; first_point.x < 200; first_point.x+=10,second_point.x+=10)
            {
                BrwGraphicsDrawThickLine(first_point, second_point, MMI_RED_COLOR, 5, style, &region);
            }
        }
        break; 

    case 4:
        {
            RECT_T rect = {0, 0, 50, 50};
            
            BrwDisplayDownloadingImage(&rect, &region, PNULL);
        }
        break;

    case 5:
        {
            RECT_T rect = {0, 0, 50, 50};

            BrwDisplayFailedImage(&rect, &region, PNULL);
        }
        break;

    default:
        break;
    }
}
#endif


/*****************************************************************************/
//  Description : zoom in or out image
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL ZOOM_RESULT_E BrwZoomImage(
                      ZOOM_INPUT_T  *zoom_in_ptr,   //in:
                      ZOOM_OUTPUT_T *zoom_out_ptr   //in/out:
                      )
{
    uint32              scale_result = 0;
    ZOOM_RESULT_E    result = ZOOM_RESULT_FAIL;
    SCALE_IMAGE_IN_T    scale_in = {0};
    SCALE_IMAGE_OUT_T   scale_out = {0};

    //SCI_ASSERT(PNULL != zoom_in_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != zoom_out_ptr); /*assert verified*/
    if(PNULL == zoom_in_ptr ||PNULL == zoom_out_ptr )
    {
        //SCI_TRACE_LOW:"BrwZoomImage zoom_in_ptr || zoom_out_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_3581_112_2_18_2_7_9_572,(uint8*)"");
        return result;
    }
    //set src width and height
    scale_in.src_size.w = zoom_in_ptr->src_img_width;
    scale_in.src_size.h = zoom_in_ptr->src_img_height;

    //set target width and height
    scale_in.target_size.w = zoom_in_ptr->dest_img_width;
    scale_in.target_size.h = zoom_in_ptr->dest_img_height;

    //w 4对齐，h 2对齐
    scale_in.target_size.w = (uint16)ZOOM_ALIGN(scale_in.target_size.w,4);
    scale_in.target_size.h = (uint16)ZOOM_ALIGN(scale_in.target_size.h,2);

#if 0    
    //set trim rect
    scale_in.src_trim_rect.x = zoom_in_ptr->clip_rect.left;
    scale_in.src_trim_rect.y = zoom_in_ptr->clip_rect.top;
    scale_in.src_trim_rect.w = (uint16)MIN((zoom_in_ptr->clip_rect.right - zoom_in_ptr->clip_rect.left + 1),(zoom_in_ptr->src_img_width - zoom_in_ptr->clip_rect.left));
    scale_in.src_trim_rect.h = (uint16)MIN((zoom_in_ptr->clip_rect.bottom - zoom_in_ptr->clip_rect.top + 1),(zoom_in_ptr->src_img_height - zoom_in_ptr->clip_rect.top));

    //check scaling up trim rect
	if ((scale_in.target_size.w > scale_in.src_trim_rect.w) && 
        (scale_in.src_trim_rect.w << 2) < scale_in.target_size.w)
	{
        scale_in.src_trim_rect.w = (uint16)(scale_in.target_size.w >> 2);
	}
    if ((scale_in.target_size.h > scale_in.src_trim_rect.h) && 
        (scale_in.src_trim_rect.h << 2) < scale_in.target_size.h)
	{
        scale_in.src_trim_rect.h = (uint16)(scale_in.target_size.h >> 2);
	}
#endif
    //原图裁剪
    scale_in.src_trim_rect.x = 0;
    scale_in.src_trim_rect.y = 0;
    scale_in.src_trim_rect.w = zoom_in_ptr->src_img_width;
    scale_in.src_trim_rect.h = zoom_in_ptr->src_img_height;

    //set src format
    scale_in.src_format = IMGREF_FORMAT_RGB565;

    //set src buffer
    scale_in.src_chn.chn0.ptr = zoom_in_ptr->src_buf_ptr;
    scale_in.src_chn.chn0.size = zoom_in_ptr->src_buf_size;

    //set target format
    scale_in.target_format = IMGREF_FORMAT_RGB565;

    //set target buffer
    scale_in.target_buf.ptr  = zoom_in_ptr->dest_buf_ptr;
    scale_in.target_buf.size = zoom_in_ptr->dest_buf_size;

    //scale
    scale_result = GRAPH_ScaleImage(&scale_in,&scale_out);
    //BRWDebug_Printf:"[BRW]BrwZoomImage:zoom result is %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_3637_112_2_18_2_7_9_573,(uint8*)"d",scale_result);
    switch (scale_result)
    {
    case SCI_SUCCESS:
        //set image width and height
        zoom_out_ptr->img_width  = scale_out.output_size.w;
        zoom_out_ptr->img_height = scale_out.output_size.h;

        //set target buffer offset
        zoom_out_ptr->dest_buf_offset = (uint32)(scale_out.output_chn.chn0.ptr) - (uint32)(scale_in.target_buf.ptr);
        result = ZOOM_RESULT_SUCC;
        break;
    
    case GRAPH_ERR_SIZE_UNSUPPORT:
        result = ZOOM_RESULT_NO_SUPPORT;
        break;

    default:
        result = ZOOM_RESULT_FAIL;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set cookie adapter
//  Global resource dependence : none
//  Author: xianhe.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BrwSetCookieAdapter(uint32 http_context_id, uint8 *cookie_ptr)
{

    if (NULL == cookie_ptr)
    {
        return FALSE;
    }

    BRWDebug_Printf("[brw]BrwSetCookieAdapter http_context_id=%d,cookie_ptr=%d",http_context_id,cookie_ptr);

    return BrwSetCookie(http_context_id, cookie_ptr);
}

#ifdef MMIDORADO_BGSOUND_SUPPORT
/*****************************************************************************/
//  Description : Get audio type by dorado mime type
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMISRVAUD_RING_FMT_E BrwGetAudioTypeByMime(BRW_MIME_TYPE_E mime_type)
{
    MMISRVAUD_RING_FMT_E audio_type = MMISRVAUD_RING_FMT_MAX;

    //SCI_TRACE_LOW:"[BRW]BrwGetAudioTypeByMime mime_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_3671_112_2_18_2_7_9_574,(uint8*)"d", mime_type);

    switch (mime_type)
    {
    case BRW_MIME_AUDIO_MID:
    case BRW_MIME_AUDIO_MIDI:
        audio_type = MMISRVAUD_RING_FMT_MIDI;
        break;

    case BRW_MIME_AUDIO_WAV:
        audio_type = MMISRVAUD_RING_FMT_WAVE;
        break;

    case BRW_MIME_AUDIO_MPEG:
        audio_type = MMISRVAUD_RING_FMT_MP3;
        break;

    case BRW_MIME_AUDIO_AMR:
        audio_type = MMISRVAUD_RING_FMT_AMR;
        break;

#ifdef AAC_SUPPORT
    case BRW_MIME_AUDIO_X_AAC:
        audio_type = MMISRVAUD_RING_FMT_AAC;
        break;
#endif

    case BRW_MIME_AUDIO_WMA:
        audio_type = MMISRVAUD_RING_FMT_WMA;
        break;

    default:
        audio_type = MMISRVAUD_RING_FMT_MAX;
        break;
    }
    return audio_type;
}

/*****************************************************************************/
//  Description :Play Background sound callback
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwPlayBgsoundCallBack(MMISRVAUD_REPORT_RESULT_E result, DPARAM param)
{
    //SCI_TRACE_LOW:"[BRW]BrwPlayBgsoundCallBack, result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_3715_112_2_18_2_7_9_575,(uint8*)"d", result);

    MMK_PostMsg(VIRTUAL_WIN_ID,MSG_BRW_STOP_BGSOUND,0,0);    
}

/*****************************************************************************/
//  Description : Play BG sound
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BrwPlayBGsound(BGSOUND_INFO *bgsound_ptr)
{
    BOOLEAN ret = FALSE;
    uint32 play_times = 0;
    MMISRVAUD_RING_FMT_E audio_type = MMISRVAUD_RING_FMT_MAX;

    if (bgsound_ptr != PNULL)
    {
        //SCI_TRACE_LOW:"[BRW]BrwPlayBGsound, loop=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_3732_112_2_18_2_7_9_576,(uint8*)"d", bgsound_ptr->loop);
        play_times = (0 > bgsound_ptr->loop)? BRW_BGSOUND_INFINITE: bgsound_ptr->loop;
        
        audio_type = BrwGetAudioTypeByMime(bgsound_ptr->bgsound_type);

        //SCI_TRACE_LOW:"[BRW]BrwPlayBGsound, play_times=%d, audio_type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_3737_112_2_18_2_7_9_577,(uint8*)"dd",play_times,audio_type);
        if (MMISRVAUD_RING_FMT_MAX != audio_type)
        {
            if (MMIAPISET_IsPermitPlayRing(MMISET_RING_TYPE_OTHER))
            {   
                ret = BRW_ReqPlaySrv(audio_type, bgsound_ptr->data_ptr, bgsound_ptr->data_len, play_times);
            }
            else
            {
                //SCI_TRACE_LOW:"[BRW]BrwPlayBGsound, MMIAPISET_IsPermitPlayRing not!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_3754_112_2_18_2_7_9_578,(uint8*)"");
            }
        }
    }

    //SCI_TRACE_LOW:"[BRW]BrwPlayBGsound:ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_3759_112_2_18_2_7_9_579,(uint8*)"d", ret);
    return ret;
}

/*****************************************************************************/
//  Description : stop BG sound
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BrwStopBGsound(void)
{
    //SCI_TRACE_LOW:"[BRW]BrwStopBGsound"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CORE_ADAPTER_3769_112_2_18_2_7_9_580,(uint8*)"");
    //MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);

    if (0 != s_brw_play_handle)
    {
        MMISRVMGR_Free(s_brw_play_handle);
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : Audio call back function called by audio service.
//	Global resource dependence : 
//   Author: 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleBrwAudioCallBack(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;

    SCI_TRACE_LOW("HandleBrwAudioCallBack() entry, handle=0x%x, param=0x%x", handle, param);
    
    if (PNULL != param && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        
        if (PNULL != report_ptr)
        {            
            SCI_TRACE_LOW("HandleBrwAudioCallBack report=%d", report_ptr->report);
        
            switch(report_ptr->report)
            {
            case MMISRVAUD_REPORT_END:
                s_brw_play_handle = 0;

                BrwPlayBgsoundCallBack(MMISRVAUD_REPORT_RESULT_STOP, param);
                break;

            default:
                break;
            }
        }
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : Request BG play sevice.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN BRW_ReqPlaySrv(MMISRVAUD_RING_FMT_E audio_type,
                                                                uint8 *data_ptr,
                                                                uint32 data_len,
                                                                uint32 play_times)
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;
    BOOLEAN res = FALSE;

    if (0 != s_brw_play_handle)
    {
        MMISRVMGR_Free(s_brw_play_handle);
    }

    req.is_auto_free = TRUE;
    req.pri = MMISRVAUD_PRI_NORMAL;
    req.notify = HandleBrwAudioCallBack;
    audio_srv.volume = MMIAPISET_GetMultimVolume();
    audio_srv.volume_type = MMISRVAUD_VOLUME_TYPE_MED;
    audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE;
    audio_srv.play_times = play_times;
    audio_srv.info.ring_buf.type = MMISRVAUD_TYPE_RING_BUF;
    audio_srv.info.ring_buf.fmt = audio_type;
    audio_srv.info.ring_buf.data = data_ptr;
    audio_srv.info.ring_buf.data_len = data_len;
    handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);

    if (handle > 0)
    {
        s_brw_play_handle = handle;

        if (MMISRVAUD_Play(s_brw_play_handle, 0))
        {
            res = TRUE;
        }
        else
        {
            MMISRVMGR_Free(s_brw_play_handle);
            s_brw_play_handle = 0;

            BrwPlayBgsoundCallBack(MMISRVAUD_REPORT_RESULT_ERROR, PNULL);
        }

        res = TRUE;
    }
    else
    {
        MMISRVMGR_Free(s_brw_play_handle);
        s_brw_play_handle = 0;

        BrwPlayBgsoundCallBack(MMISRVAUD_REPORT_RESULT_ERROR, PNULL);
    }

    SCI_TRACE_LOW("BRW_ReqPlaySrv handle=%d", handle);

    return res;
}
#endif



/*Edit by script, ignore 8 case. Fri Apr 27 09:38:48 2012*/ //IGNORE9527
