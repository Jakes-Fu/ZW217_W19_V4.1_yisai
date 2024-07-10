#include "mmi_app_accessory_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "dal_time.h"
#include "window_parse.h"
#include "mmk_timer.h"
#include "guibutton.h"
#include "mmidisplay_data.h"
#include "mmi_appmsg.h"
#include "mmi_default.h"
#include "mmiudisk_export.h"
#include "mmisd_export.h"

#include "mmi_default.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmiacc_id.h"
#include "mmiacc_image.h"
#include "mmiacc_text.h"
#include "mmicountedtime_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_id.h"
#include "mmiidle_export.h"
#include "guires.h"
#include "mmikl_export.h"
#include "guistring.h"
#include "mmi_theme.h"

#include "mmi_timer_export.h"

#include "guictrl_api.h"

#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif

#include "zdt_app.h"
#include "mmiset_export.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "zdt_net.h"
#include "video_call_id.h"
#include "video_call_text.h"
#include "zdt_win_export.h"
#include "watch_commonwin_export.h"
#include "agora_video_call.h"
#include "video_call.h"
#include "watch_common.h"
//#include "video_call_demo.h"

uint8 video_call_waiting_timer_id = 0;
uint8 video_call_show_hangup_timer_id = 0;
BOOLEAN g_is_makecallout = 0;
BOOLEAN g_is_incomeing = FALSE;
BOOLEAN g_is_inVideo = FALSE; //视频通话中
BOOLEAN g_is_video_call_show_hangup = FALSE;
BOOLEAN g_is_activeVideo = FALSE; //视频通话正在呼叫

#define VIDEO_CALL_TIME_OUT 30000  //30秒
#define VIDEO_CALL_OUT_TIME_OUT 3000
#define VIDEO_CALL_HANGUP_SHOW_TIME_OUT 5000

typedef enum
{
    MMI_VVIDEO_CALL_STATE_NULL     = 0,
    MMI_VIDEO_CALL_STATE_ACTIVE   = 1,
    MMI_VIDEO_CALL_STATE_HOLDING  = 2,
    MMI_VIDEO_CALL_STATE_DIALING  = 3,                                           /* MO call only */
    MMI_VIDEO_CALL_STATE_ALERTING = 4,                                           /* MO call only */
    MMI_VIDEO_CALL_STATE_INCOMING = 5,                                           /* MT call only */
    MMI_VIDEO_CALL_STATE_WAITING  = 6,                                           /* MT call only */
    MMI_VIDEO_CALL_STATE_END  = 6,  
    MMI_VIDEO_CALL_STATE_INVALID  = 0xFF
} MMI_VIDEO_CALL_STATE_E;

LOCAL GUI_LCD_DEV_INFO  s_video_call_ui_layer_handle = {0,UILAYER_NULL_HANDLE};




//还未接通时, APP 远端挂断, 不在视频中时的远端挂断标志
BOOLEAN g_video_call_remote_hangup_but_not_invideo = FALSE;

LOCAL void Video_Call_Close(MMI_WIN_ID_T win_id);
LOCAL BOOLEAN Video_Call_Reject_Or_Hangup(MMI_WIN_ID_T win_id);




#ifdef WIN32
int avc_osd_blend(avc_osd_id_e osd_id,bool enable,uint8_t *rgba8888,uint16_t left,uint16_t top,uint16_t right,uint16_t bottom){return 0;};
int avc_hangup_button(bool show,uint16_t left,uint16_t top,uint16_t right,uint16_t bottom){return 0;};
#endif

LOCAL void hangup_button_show(bool show,uint16_t left,uint16_t top,uint16_t right,uint16_t bottom)
{
    avc_hangup_button(show, left, top, right, bottom);
}

LOCAL void Video_Call_Hangup_Show_TimeOut_CallBack(uint8 timer_id, uint32 param);

LOCAL void video_call_backlight(BOOLEAN is_alway_on)
{
    if(is_alway_on)
    {
        MMIDEFAULT_SetAlwaysHalfOnBackLight(FALSE);
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        MMIDEFAULT_TurnOnBackLight();
    } else {
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        MMIDEFAULT_SetAlwaysHalfOnBackLight(FALSE);
    }
}

LOCAL void Video_Call_Show_Toast(MMI_TEXT_ID_T text_id)
{
    MMI_WIN_ID_T    winId   = WATCH_VIDEO_CALL_TOAST_WIN_ID;
    MMI_STRING_T    noteStr = {0};
    WATCH_SOFTKEY_TEXT_ID_T softKey = {0};
    MMI_GetLabelTextByLang(text_id, &noteStr);
    WatchCOM_NoteWin_1Line_Enter(winId, &noteStr, NULL, softKey, NULL);
}

//unsigned char gImage_icon_videocall_connect_ringoffBtn[];
LOCAL void Get_hangup_image()
{
    GUIRES_IMG_SRC_T    img_src = {0};

    //check image id
    if (MMITHEME_CheckImageID(IMAGE_CALL_HANGUP))
    {
        //get resource pointer and size
        img_src.data_ptr = (uint8 *)MMI_GetLabelImage(IMAGE_CALL_HANGUP,WATCH_VIDEO_CALL_INCOMING_WIN_ID,&(img_src.data_size));
        if (PNULL != img_src.data_ptr && 0 < img_src.data_size)
        {
            //get image info
        }
    }
}

LOCAL void Stop_Video_Call_Waiting_Timer()
{
    if( video_call_waiting_timer_id != 0)
    {
        MMK_StopTimer(video_call_waiting_timer_id);
        video_call_waiting_timer_id = 0;
    }
}

LOCAL void Video_Call_TimeOut_CallBack(uint8 timer_id, uint32 param)
{
    MMI_WIN_ID_T winId  = (MMI_WIN_ID_T)param;
    if(winId == WATCH_VIDEO_CALL_INCOMING_WIN_ID) //app call设备超时不接听
    {
        VIDEO_CALL_INFO *video_call_info = NULL;
        video_call_info = (VIDEO_CALL_INFO *) MMK_GetWinUserData(winId);
        if(video_call_info != NULL)
        {
            YX_Net_Send_UPWATCHHANGUP(video_call_info->video_id);
        }
        Video_Call_Show_Toast(TXT_VIDEO_CALL_ACCEPT_TIMEOUT);
        MMK_CloseWin(winId);
    }
    else if(winId == WATCH_VIDEO_CALL_OUT_WIN_ID) //设备call app 超时无人接听或者网络原因没有送达
    {
        MMK_CloseWin(winId);
        Video_Call_Show_Toast(TXT_VIDEO_CALL_CONNECT_TIMEOUT);
    }
    MMIAPISET_StopRing(MMISET_RING_TYPE_CALL);
}

LOCAL void Start_Video_Call_Waiting_Timer(MMI_WIN_ID_T win_id, uint32 time_out)
{
    Stop_Video_Call_Waiting_Timer();
    video_call_waiting_timer_id = MMK_CreateTimerCallback(time_out,Video_Call_TimeOut_CallBack,(uint32)win_id, FALSE);
}

LOCAL void Stop_Hangup_Show_Timer()
{
    if( video_call_show_hangup_timer_id != 0)
    {
        MMK_StopTimer(video_call_show_hangup_timer_id);
        video_call_show_hangup_timer_id = 0;
    }
}

LOCAL void Start_Hangup_Show_Timer()
{
    Stop_Hangup_Show_Timer();
    video_call_show_hangup_timer_id = MMK_CreateTimerCallback(VIDEO_CALL_HANGUP_SHOW_TIME_OUT,Video_Call_Hangup_Show_TimeOut_CallBack,NULL, FALSE);
}

/*****************************************/
//在视频通话中显示或隐藏挂断图标
/*****************************************/
LOCAL void Video_Call_Hangup_Show(BOOLEAN show)
{
    if(g_is_inVideo)
    {
        GUI_RECT_T hangup_in_vodeo_rect = VIDEO_CALL_IN_VIDEO_HANGUP_RECT;
        hangup_button_show(show,hangup_in_vodeo_rect.left, hangup_in_vodeo_rect.top, hangup_in_vodeo_rect.right, hangup_in_vodeo_rect.bottom);
        if(show)
        {
            Start_Hangup_Show_Timer();
        }
        g_is_video_call_show_hangup = show;
    }
}

LOCAL void Video_Call_Hangup_Show_TimeOut_CallBack(uint8 timer_id, uint32 param)
{
    Video_Call_Hangup_Show(FALSE);
}

//没有用到
LOCAL void Create_Video_Call_Ui_Layer(MMI_WIN_ID_T win_id)
{
    uint16   layer_width = 0;
    uint16   layer_height = 0;
    UILAYER_CREATE_T    create_info = {0};
    UILAYER_APPEND_BLT_T        append_layer = {0};
    UILAYER_RESULT_E ret = 0;

    if (s_video_call_ui_layer_handle.lcd_id != UILAYER_NULL_HANDLE)    
    {
        //creat layer
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = 240;
        create_info.height = 284;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = TRUE; 
        ret = UILAYER_CreateLayer(&create_info, &s_video_call_ui_layer_handle);
      
        if (UILAYER_RESULT_SUCCESS == ret)
        {
            append_layer.layer_level = UILAYER_LEVEL_HIGH;
            append_layer.lcd_dev_info = s_video_call_ui_layer_handle;
            UILAYER_SetLayerColorKey(&s_video_call_ui_layer_handle, TRUE, MMI_BLACK_COLOR);
            UILAYER_WeakLayerAlpha(&s_video_call_ui_layer_handle, 0x80);
        }
        else
        {
            SCI_TRACE_LOW("[%s]:fail to create uilayer. ret:%d",__FUNCTION__,ret);
        }
    }
    if (UILAYER_IsLayerActive(&s_video_call_ui_layer_handle))
    {
        UILAYER_Clear(&s_video_call_ui_layer_handle);
    }

}

LOCAL void ReleaseVideo_Call_Ui_Layer()
{
    if (UILAYER_NULL_HANDLE == s_video_call_ui_layer_handle.lcd_id)
    {
        UILAYER_RELEASELAYER(&s_video_call_ui_layer_handle);    /*lint !e506 !e774*/

    }
    s_video_call_ui_layer_handle.block_id = 0;
    s_video_call_ui_layer_handle.lcd_id = UILAYER_NULL_HANDLE;
}

/*************************************/
//显示头像和名字
/*************************************/
LOCAL void draw_head_image_and_name(MMI_WIN_ID_T win_id, GUISTR_STYLE_T text_style, GUI_LCD_DEV_INFO lcd_dev_info,MMI_VIDEO_CALL_STATE_E type)
{
    uint16 img_width = 0;
    uint16 img_height = 0;
    MMI_IMAGE_ID_T image_id = NULL;
    MMI_STRING_T name_string = {0};
    GUI_POINT_T point = {0,VIDEO_CALL_HEAD_IMG_Y};
    GUI_RECT_T name_rect = VIDEO_CALL_NAME_RECT;
    if(MMI_VIDEO_CALL_STATE_DIALING == type)
    {
        VIDEO_USER_INFO *user_info  = (VIDEO_USER_INFO *) MMK_GetWinUserData(win_id); 
        if(user_info != NULL)
        {
            name_string.wstr_ptr = user_info->family_relative;
            name_string.wstr_len = MMIAPICOM_Wstrlen(name_string.wstr_ptr); 
        #ifdef ZTE_WATCH
            image_id = WATCHCOM_GetAvaterBigImageId(in_str);
        #else
            image_id = WATCHCOM_GetAvaterImageId(name_string.wstr_ptr);
        #endif

            if(GUIRES_GetImgWidthHeight(&img_width, &img_height, image_id, win_id))
            {
                point.x = (MMI_MAINSCREEN_WIDTH-img_width)/2;
                name_rect.top = point.y + img_height;
                name_rect.bottom += name_rect.top;
            }
        }
    }
    else if(MMI_VIDEO_CALL_STATE_INCOMING == type)
    {
        VIDEO_CALL_INFO *video_call_info = (VIDEO_CALL_INFO *) MMK_GetWinUserData(win_id);
        if(video_call_info != NULL)
        {
            name_string.wstr_ptr = video_call_info->family_relative;
            name_string.wstr_len = MMIAPICOM_Wstrlen(name_string.wstr_ptr); 
        #ifdef ZTE_WATCH
            image_id = WATCHCOM_GetAvaterBigImageId(in_str);
        #else
            image_id = WATCHCOM_GetAvaterImageId(name_string.wstr_ptr);
        #endif

            if(GUIRES_GetImgWidthHeight(&img_width, &img_height, image_id, win_id))
            {
                point.x = (MMI_MAINSCREEN_WIDTH-img_width)/2;
                name_rect.top = point.y + img_height;
                name_rect.bottom += name_rect.top;
            }
        }
    }
    GUIRES_DisplayImg(&point, PNULL, PNULL, win_id, image_id, &lcd_dev_info);
    GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &name_rect,
        &name_rect,
        &name_string,
        &text_style,
        GUISTR_STATE_ALIGN,
        GUISTR_TEXT_DIR_AUTO
        );
}

LOCAL void Video_Call_Display_video_type(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO lcd_dev_info, MMI_VIDEO_CALL_STATE_E type)
{
    GUI_RECT_T state_rect = VIDEO_CALL_STATE_RECT;
    GUI_RECT_T name_rect = VIDEO_CALL_NAME_RECT;
    GUI_RECT_T out_hangup_rect = VIDEO_CALL_OUT_HANGUP_RECT;
    GUI_RECT_T hangup1_rect = VIDEO_CALL_INCOMING_HANGUP_RECT;
    GUI_RECT_T answer1_rect = VIDEO_CALL_INCOMING_ACCRPT_RECT;
    GUI_RECT_T hangup_in_vodeo_rect = VIDEO_CALL_IN_VIDEO_HANGUP_RECT;
    MMI_IMAGE_ID_T image_id = NULL;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_title = {0};
    MMI_STRING_T state_string = {0};
    MMI_STRING_T name_string = {0};
    ZDT_LOG("[%s] type:%d", __FUNCTION__, type);
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = SONG_FONT_26;
    text_style.font_color = MMI_WHITE_COLOR;
    switch(type)
    {
        case MMI_VIDEO_CALL_STATE_DIALING:
            {

                MMIRES_GetText(TXT_VIDEO_CALL, win_id, &state_string);
                GUIRES_DisplayImg(PNULL, &out_hangup_rect, PNULL, win_id, IMAGE_CALL_HANGUP, &lcd_dev_info);
                GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                    &state_rect,
                    &state_rect,
                    &state_string,
                    &text_style,
                    GUISTR_STATE_ALIGN,
                    GUISTR_TEXT_DIR_AUTO
                    );
				g_is_activeVideo = TRUE;
                draw_head_image_and_name(win_id, text_style,lcd_dev_info,type);
            }
            break;

        case MMI_VIDEO_CALL_STATE_INCOMING:
            {
                WATCHCOM_DisplayBackground(win_id);

                MMIRES_GetText(TXT_VIDEO_CALL_INCOM, win_id, &state_string);
                GUIRES_DisplayImg(PNULL, &hangup1_rect, PNULL, win_id, IMAGE_CALL_HANGUP, &lcd_dev_info);
                GUIRES_DisplayImg(PNULL, &answer1_rect, PNULL, win_id, IMAGE_CALL_ACCEPT, &lcd_dev_info);

                GUISTR_DrawTextToLCDInRect(
                    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                    &state_rect,
                    &state_rect,
                    &state_string,
                    &text_style,
                    GUISTR_STATE_ALIGN,
                    GUISTR_TEXT_DIR_AUTO
                    );
				g_is_activeVideo = TRUE;
                draw_head_image_and_name(win_id, text_style,lcd_dev_info,type);
            }
            break;
        case MMI_VIDEO_CALL_STATE_ACTIVE:
            {    
                MMIRES_GetText(TXT_PLS_CALL_WAIT, win_id, &state_string);
                GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                    &state_rect,
                    &state_rect,
                    &state_string,
                    &text_style,
                    GUISTR_STATE_ALIGN,
                    GUISTR_TEXT_DIR_AUTO
                    );
				g_is_activeVideo = TRUE;
                GUIRES_DisplayImg(PNULL, &hangup_in_vodeo_rect, PNULL, win_id, IMAGE_CALL_HANGUP, &lcd_dev_info);

 //               GUIRES_DisplayImg(PNULL, &del_rect, PNULL, win_id, IMG_CC_MINUS_ICON, &lcd_dev_info);
 //               GUIRES_DisplayImg(PNULL, &add_rect, PNULL, win_id, IMG_CC_VOLPLUS_ICON, &lcd_dev_info);
            }
            break;
        case MMI_VIDEO_CALL_STATE_WAITING:
            break;
        default:
            break;

    }
    
}

LOCAL MMI_RESULT_E HandleVideoMainWin(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            
        }
        break;
    case MSG_FULL_PAINT:
        {

        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
        {

        }
        break;
#endif
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}


WINDOW_TABLE(MMI_VIDEO_CALL_WIN_TAB) = 
{
    WIN_ID(WATCH_VIDEO_CALL_WIN_ID),
    WIN_FUNC((uint32)HandleVideoMainWin),
    //CREATE_BUTTON_CTRL(IMG_REJECT_ICON,  VIDEO_CALL_HANGUP_BTN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

LOCAL void MMIVIDEO_OpenVideoWin(void)
{
    if(MMK_IsOpenWin(WATCH_VIDEO_CALL_WIN_ID))
    {
        return;
    }
    MMK_CreateWin((uint32*)MMI_VIDEO_CALL_WIN_TAB, PNULL);
}

#if 0// not use
LOCAL void DrawCtrl_In_Video_Call(BOOLEAN show)
{
    GUI_RECT_T layerRect= {0};
    GUI_RECT_T win_rect = MMITHEME_GetFullScreenRect();
    Create_Video_Call_Ui_Layer(WATCH_VIDEO_CALL_INCOMING_WIN_ID);
    GUI_FillRect(&s_video_call_ui_layer_handle,win_rect, MMI_BLACK_COLOR);
    Video_Call_Display_video_type(WATCH_VIDEO_CALL_INCOMING_WIN_ID, s_video_call_ui_layer_handle,MMI_VIDEO_CALL_STATE_ACTIVE);
    layerRect = UILAYER_GetLayerRect(&s_video_call_ui_layer_handle);
    avc_osd_blend(AVC_OSD_LAYER_ID_1,show,UILAYER_GetLayerBufferPtr(&s_video_call_ui_layer_handle),layerRect.left,layerRect.top,layerRect.right,layerRect.bottom);
    MMITHEME_UpdateRect();
}
#endif

//视频通话状态回调
LOCAL void Video_Call_Status_Callback(avc_msg_t *p_msg)
{
    ZDT_LOG("[%s]recv msg. code=%d", __FUNCTION__, p_msg->code);
    switch (p_msg->code) {
        case AVC_MSG_JOIN_CHANNEL_SUCCESS: /* local本端成功加入channel消息 */
            //MMIVIDEO_OpenVideoWin();
            //DrawCtrl_In_Video_Call(FALSE);
            break;
        case AVC_MSG_USER_JOINED: /* remote远端成功加入channel消息 */
            if(MMK_IsOpenWin(WATCH_VIDEO_CALL_INCOMING_WIN_ID))
            {
                //MMK_SendMsg(WATCH_VIDEO_CALL_INCOMING_WIN_ID,MSG_FULL_PAINT,PNULL);
            }   
            break;
        case AVC_MSG_USER_OFFLINE: /* remote远端已下线 */
            if(MMK_IsOpenWin(WATCH_VIDEO_CALL_INCOMING_WIN_ID))
            {
               //MMK_SendMsg(WATCH_VIDEO_CALL_INCOMING_WIN_ID,MSG_CLOSE_WINDOW,PNULL);
               //MMK_CloseWin(WATCH_VIDEO_CALL_INCOMING_WIN_ID);
               //Video_Call_Show_Toast(TXT_VIDEO_CALL_REMOTE_HANGUP);
               SCI_TraceLow("Video_Call_Status_Callback, wuxx, AVC_MSG_USER_OFFLINE");

               Video_Call_Remote_Hangup();
            }
            else if(MMK_IsOpenWin(WATCH_VIDEO_CALL_OUT_WIN_ID))
            {
                MMK_SendMsg(WATCH_VIDEO_CALL_OUT_WIN_ID,MSG_CLOSE_WINDOW,PNULL);
            }
            g_is_inVideo = FALSE;
			g_is_activeVideo = FALSE; 
            break;
        case AVC_MSG_CONNECTION_LOST: /* local本端因网络原因掉线 */
            break;
        case AVC_MSG_INVALID_APP_ID: /* appId不合法 */
            break;
        case AVC_MSG_INVALID_CHANNEL_NAME: /* channel name不合法 */
            break;
        case AVC_MSG_INVALID_TOKEN: /* token不合法 */
            break;
        case AVC_MSG_TOKEN_EXPIRED: /* token已过期 */
            break;
        case AVC_MSG_FIRST_VIDEO_FRAME_RECV: /* 加入channel后接收到remote远端第一帧视频数据 */
            {
                GUI_RECT_T hangup_in_vodeo_rect = VIDEO_CALL_IN_VIDEO_HANGUP_RECT;
                //DrawCtrl_In_Video_Call(TRUE);
                g_is_inVideo = TRUE;
                Video_Call_Hangup_Show(TRUE);
                ////视频接通,30秒自动挂断定时器停止///
                Stop_Video_Call_Waiting_Timer();
                ///////////////////////////////////////////////////
            }
            break;
        default:
            ZDT_LOG("[%s] no msg handler, code %d", __FUNCTION__, p_msg->code);
            break;
    }
}

////////////////////////////////////// dialing //////////////////////////////////////////////////
// 设备call app
LOCAL MMI_RESULT_E HandleVideoDialingWin(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T win_rect = MMITHEME_GetFullScreenRect();

    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            Start_Video_Call_Waiting_Timer(win_id,VIDEO_CALL_OUT_TIME_OUT);
        }
        break;
    case MSG_FULL_PAINT:
        {
            MMK_GetWinRect(win_id, &win_rect);
            GUI_FillRect(&lcd_dev_info, win_rect, MMI_BLACK_COLOR);
            Video_Call_Display_video_type(win_id, lcd_dev_info,MMI_VIDEO_CALL_STATE_DIALING);
        }
        break;
    case MSG_TIMER:
        {

        }
        break;
    case MSG_KEYDOWN_RED:
        break;

    case MSG_KEYUP_RED:
            if(FALSE == Video_Call_Reject_Or_Hangup(win_id))
            {
                MMK_CloseWin(win_id);
            }
        break;
        
    case MSG_TP_PRESS_MOVE:
    case MSG_TP_PRESS_DOWN:
        {
        }
        break;
    case MSG_TP_PRESS_UP:
        {
            
        }
        break;
    case MSG_CLOSE_WINDOW:
        {
			//解决内存丢失问题
            VIDEO_USER_INFO *video_user_info = (VIDEO_USER_INFO *) MMK_GetWinUserData(win_id);
            if(video_user_info != NULL)
            {
                SCI_FREE(video_user_info);
                MMK_SetWinUserData(win_id, NULL);
            }
            Stop_Video_Call_Waiting_Timer();
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

WINDOW_TABLE(MMI_VIDEO_CALL_OUT_WIN_TAB) = 
{
    WIN_ID(WATCH_VIDEO_CALL_OUT_WIN_ID),
    WIN_FUNC((uint32)HandleVideoDialingWin),
    WIN_HIDE_STATUS,
    END_WIN
};

LOCAL void Video_Call_Out(VIDEO_USER_INFO call_info)
{
    MMI_WIN_ID_T win_id = WATCH_VIDEO_CALL_OUT_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    VIDEO_USER_INFO *user_info = PNULL;

    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }
    win_handle = MMK_CreateWin((uint32*)MMI_VIDEO_CALL_OUT_WIN_TAB, PNULL);
    user_info = SCI_ALLOC_APP(sizeof(VIDEO_USER_INFO));
    SCI_MEMSET(user_info,0,sizeof(VIDEO_USER_INFO));
    SCI_MEMCPY(user_info, &call_info, sizeof(VIDEO_USER_INFO));
    MMK_SetWinUserData(win_handle, user_info);
}

LOCAL void Video_Call_Out_Close()
{
    MMI_WIN_ID_T win_id = WATCH_VIDEO_CALL_OUT_WIN_ID;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }
}

PUBLIC BOOLEAN  Video_Call_Device_Idle_Check(void)
{
    // if((MMICC_IsExistIncommingCall() || MMICC_IsExistActiveCall()) || MMIZDT_IsClassModeWinOpen() == TRUE || VideoChat_IsInCall())
    // {
    //     return TRUE;
    // }
    // return FALSE;
    if( MMICC_IsExistIncommingCall() || MMICC_IsExistActiveCall() 
        || MMIAPICC_IsInState(CC_MT_CONNECTING_STATE) || MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE)
        || MMIAPICC_IsInState(CC_MO_CONNECTING_STATE) || (MMIZDT_IsClassModeWinOpen() == TRUE) 
        #ifdef ZTE_WATCH
        || Low_Battery_Model_Check()
        #endif
        || VideoChat_IsInCall())
    {
        return TRUE;
    }
    return FALSE;
}

////////////////////////////////////// contact list //////////////////////////////////////////////////

LOCAL THEMELIST_ITEM_STYLE_T new_style = {0}; //为啥是 static 才有效

LOCAL void Display_Contact_List(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, VIDEO_USER_INFO *user_info)
{
    BOOLEAN ret = FALSE;
    MMI_STRING_T    cur_str_t   = {0};
    uint16 tempStr[101] ={0};
    uint16 i = 0, j = 0;
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    uint16 position = 0;


    GUILIST_ITEM_T               item_t    =  {0};
    GUILIST_ITEM_DATA_T          item_data = {0};
    wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};
    //add by James li begin
    uint16 total_item_num = 0;
    MMI_STRING_T    empty_str = {0};
    GUILIST_EMPTY_INFO_T empty_info = {0};
    //add by James li end
    uint8 contact_num = 0;
    uint16 phoneNumber[30] ={0};
    //item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON;
    

    //#if 1//def NEW_LEFT_RIGHT_STYLE   
    const uint16 h_gap = 5;
    const uint16 w_gap = 5;
    const uint16 img_height = 60;
    const uint16 img_width = 60;
    const uint16 height = img_height + 2*h_gap;
    GUI_RECT_T l_rect0 = { w_gap, h_gap,  img_width + w_gap,  img_height+h_gap};//bg
    GUI_RECT_T l_rect1 = { img_width+w_gap+8,  h_gap,  MMI_MAINSCREEN_WIDTH-2*h_gap , img_height+h_gap};//text
    GUI_RECT_T r_rect2 = { MMI_MAINSCREEN_WIDTH - img_width- 2*w_gap, h_gap,   MMI_MAINSCREEN_WIDTH - w_gap,  img_height+h_gap};//bg
    ////GUI_RECT_T r_rect1 = { MMI_MAINSCREEN_WIDTH - img_width- 2*w_gap+2,  h_gap,  MMI_MAINSCREEN_WIDTH - 2*w_gap , img_height+h_gap};//text
    
    
    
    //uint32 i = GUILIST_GetTotalItemNum(ctrl_id);
    THEMELIST_ITEM_STYLE_T* pItemStyle = THEMELIST_GetStyle(GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON);
    SCI_MEMCPY(&new_style, pItemStyle, sizeof(*pItemStyle));
   
    new_style.height = new_style.height_focus = img_height + h_gap*2;
   
    //
    new_style.content[0].rect = new_style.content[0].rect_focus = l_rect0;
    new_style.content[1].rect = new_style.content[1].rect_focus = l_rect1;
    new_style.content[1].font= new_style.content[1].font_focus = SONG_FONT_26;
    new_style.content[2].rect = new_style.content[2].rect_focus = r_rect2;
     
    item_t.item_data_ptr = &item_data; 
    for(i=0;i<MAX_VIDEO_USERS_COUNT;i++)
    {
        if(user_info[i].phone_user_id[0] != 0)
       {
           contact_num++;
       }
    }
    GUILIST_RemoveAllItems(ctrl_id);

    for(i = 0 ;i < contact_num ; i++)
    {
        if(user_info[i].video_status == 1)
        {
            SCI_MEMSET(tempStr,0,(101)*sizeof(uint16));
            SCI_STRCPY(tempStr,user_info[i].family_relative);
            image_id = WATCHCOM_GetAvaterImageId(tempStr);
            item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;        
            item_data.item_content[0].item_data.image_id = image_id;
        
            item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;       
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = user_info[i].family_relative;
            item_data.item_content[1].item_data.text_buffer.wstr_len = (uint16) MMIAPICOM_Wstrlen (user_info[i].family_relative);

            item_data.item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;        
            item_data.item_content[2].item_data.image_id = IMAGE_VIDEO_CALL_ICON;

            ret = GUILIST_AppendItem (ctrl_id, &item_t);
            if(ret)
            {
                CTRLLIST_SetItemUserData(ctrl_id, position, &i); 
                //#if 1//def NEW_LEFT_RIGHT_STYLE 
                GUILIST_SetItemStyleEx(ctrl_id, position, &new_style );
                //#endif
                position++;   
            }  
        }
        
    }

    if (contact_num == 0)
    {
        WATCHCOM_DisplayTips(win_id, TXT_VIDEO_CALL_CONTACT_EMPTY);
    }
}

LOCAL MMI_RESULT_E  HandleVideoContactListWin(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = VIDEO_CALL_CONTACT_LIST_CTRL_ID;
    BOOLEAN ret = FALSE;
    uint32 pos_user_data = 0; // position user data
    GUILIST_INIT_DATA_T list_init = {0}; 
    VIDEO_USER_INFO *user_info = NULL; 
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            GUI_RECT_T rect = {VIDEO_CALL_CONTACT_LIST_X,VIDEO_CALL_CONTACT_LIST_Y,(MMI_MAINSCREEN_WIDTH -1),(MMI_MAINSCREEN_HEIGHT -1)};
            list_init.both_rect.v_rect.left = 0;
            list_init.both_rect.v_rect.right = (MMI_MAINSCREEN_WIDTH -1);
            list_init.both_rect.v_rect.top = 0;
            list_init.both_rect.v_rect.bottom = (MMI_MAINSCREEN_HEIGHT-1);//240;//180;

            list_init.both_rect.h_rect.left = 0;
            list_init.both_rect.h_rect.right = (MMI_MAINSCREEN_WIDTH -1);//240;//180;
            list_init.both_rect.h_rect.top = 0;
            list_init.both_rect.h_rect.bottom = (MMI_MAINSCREEN_HEIGHT -1);
            
            list_init.type = GUILIST_TEXTLIST_E;
                        
            GUILIST_CreateListBox(win_id, 0, ctrl_id, &list_init);
            GUILIST_SetRect(ctrl_id,&rect);
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE );//不需要分割线
            ret = GUILIST_SetMaxItem(ctrl_id, YX_DB_WHITE_MAX_SUM, FALSE);
            MMK_SetAtvCtrl(win_id,ctrl_id);
        }
        break;
        
        case MSG_FULL_PAINT:
            {
                user_info = (VIDEO_USER_INFO *) MMK_GetWinUserData(win_id);
                if(user_info != NULL)
                {
                    WATCHCOM_DisplayTitle(win_id, TXT_VIDEO_CALL_CONTACT_TITLE);
                    Display_Contact_List(win_id, ctrl_id, user_info);
                }
                else
                {
                    GUI_RECT_T rect = {0,0,0,0};
                    GUILIST_SetRect(ctrl_id,&rect);
                    WATCHCOM_DisplayBackground(win_id);
                    WATCHCOM_DisplayTips(win_id, TXT_VIDEO_CALL_CONTACT_EMPTY);
                }
            }
            break;
            
        case MSG_GET_FOCUS:
        //    MMIZDT_Video_Call_Show(win_id);
            break;
            
        case MSG_LOSE_FOCUS:
            break;
            
     case MSG_KEYUP_OK:
     case MSG_KEYUP_GREEN:
         {
            uint16 num_index = GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_GetItemData(ctrl_id, num_index, &pos_user_data);// user_data stand position
         }
         break;
    case MSG_KEYDOWN_RED:
        break;

    case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
        break;
        
    case MSG_KEYDOWN_WEB:
            //MMIZDT_PB_Call_Start(4+(m_yx_whitelist_curpage-1)*4);
        break;
        
    case MSG_NOTIFY_LIST_ITEM_CONTENT_CLICK:
    {
        uint16 index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id, index, &pos_user_data);// user_data stand position
        user_info = (VIDEO_USER_INFO *) MMK_GetWinUserData(win_id);
        //ZDT_LOG("[%s] MSG_NOTIFY_LIST_ITEM_CONTENT_CLICK 11", __FUNCTION__);
        if(user_info != NULL)
        {
        //    ZDT_LOG("[%s] MSG_NOTIFY_LIST_ITEM_CONTENT_CLICK", __FUNCTION__);
            VIDEO_USER_INFO user = user_info[index];
            Video_Call_Out(user);
            YX_Net_Send_StartVideoCall(user);          
        }
        break;
    }
        
    case MSG_KEYDOWN_DOWN:
            //m_yx_whitelist_curindex = 0;
            //MMIZDT_PB_TP_Move(win_id,1);
        break;
        
    case MSG_KEYDOWN_UP:
            //m_yx_whitelist_curindex = 0;
            //MMIZDT_PB_TP_Move(win_id,0);
        break;

    case MSG_KEYDOWN_LEFT:

        break;

    case MSG_KEYDOWN_RIGHT:

        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    
    case MSG_CLOSE_WINDOW:
        {
            user_info = (VIDEO_USER_INFO *) MMK_GetWinUserData(win_id);
            if(user_info != NULL)
            {
                SCI_FREE(user_info);
                MMK_SetWinUserData(win_id, NULL);
            } 
        }
        break;      

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}


WINDOW_TABLE(MMI_VIDEO_CALL_CONTACT_LIST_WIN_TAB) = 
{
    WIN_ID(WATCH_VIDEO_CALL_CONTACT_LIST_WIN_ID),
    WIN_FUNC((uint32)HandleVideoContactListWin),
    WIN_HIDE_STATUS,
    END_WIN
};

LOCAL void MMIVideo_Call_ContactListWin(VIDEO_USER_INFO *user_info)
{
    MMI_HANDLE_T win_handle = 0;
    MMI_WIN_ID_T win_id = WATCH_VIDEO_CALL_CONTACT_LIST_WIN_ID;
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }
    win_handle = MMK_CreateWin((uint32*)MMI_VIDEO_CALL_CONTACT_LIST_WIN_TAB, NULL);
    MMK_SetWinUserData(win_handle, user_info);
    MMK_SetWinRect(win_handle, &rect);
}

LOCAL void MMIVideo_Call_ContactListWin_Close()
{
    MMI_WIN_ID_T win_id = WATCH_VIDEO_CALL_CONTACT_LIST_WIN_ID;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }
}

/************************************/
//视频通话联系人列表是实时去服务器获取
/*************************************/
extern void YX_API_GetVideoUsersFromServer();
LOCAL MMI_RESULT_E HandleRequestContactWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            YX_API_GetVideoUsersFromServer();
        }
        break;
        case MSG_CLOSE_WINDOW:
        {
           // MMIVideo_Call_ContactListWin();
            MMIVideo_Call_ContactListWin(NULL);
        }
        break;
        default:
        {
            result = MMI_RESULT_FALSE;
        }
        break;
    }
    return (result);
}

PUBLIC void Video_Call_RequestContactSuccess(VIDEO_USER_INFO *video_call_user)
{
    VIDEO_USER_INFO *user_info = PNULL;
    if(MMK_IsOpenWin(WATCH_VIDEO_CALL_WAITING_WIN_ID))
    {
       MMK_CloseWin(WATCH_VIDEO_CALL_WAITING_WIN_ID);
    }
    //alloc space
    user_info = SCI_ALLOC_APP(sizeof(VIDEO_USER_INFO)*MAX_VIDEO_USERS_COUNT);
    SCI_MEMSET(user_info,0,sizeof(VIDEO_USER_INFO)*MAX_VIDEO_USERS_COUNT);
    SCI_MEMCPY(user_info, video_call_user, sizeof(VIDEO_USER_INFO)*MAX_VIDEO_USERS_COUNT);
    MMIVideo_Call_ContactListWin(user_info);
}

PUBLIC void MMIVideo_Call_MainWin()
{
    //MMIVideo_Call_ContactListWin();
    MMI_WIN_ID_T wait_win_id = WATCH_VIDEO_CALL_WAITING_WIN_ID;
    WATCH_SOFTKEY_TEXT_ID_T   softket_test = {TXT_NULL,TXT_NULL,TXT_NULL};
    WatchCOM_WaitingTimeOutWin_FullScreen_Enter(wait_win_id,PNULL,image_watch_waitingwin_fullscreen_pic,softket_test,HandleRequestContactWinMsg, 5000);

}

/**************************************/
//拒接或者挂断视频通话
/**************************************/
LOCAL BOOLEAN Video_Call_Reject_Or_Hangup(MMI_WIN_ID_T win_id)
{
    BOOLEAN res = FALSE;
    VIDEO_CALL_INFO *video_call_info = NULL;
    video_call_info = (VIDEO_CALL_INFO *) MMK_GetWinUserData(win_id);
    if(video_call_info != NULL && (video_call_info->call_type == VIDEO_CALL_INCOMING || video_call_info->call_type == VIDEO_CALL_OUT))
    {
        YX_Net_Send_UPWATCHHANGUP(video_call_info->video_id);
        MMIAPISET_StopRing(MMISET_RING_TYPE_CALL);
        MMK_CloseWin(win_id);
        res = TRUE;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else// wuxx add //信号差或者在屏蔽房无信号时挂断按键也要挂断 // VIDEO_INCOMING_BUT_NO_SIGNAL process wuxx
    {
        //来电却突然无信号 无法接通的按挂断键的处理
        if(MMK_IsOpenWin(WATCH_VIDEO_CALL_INCOMING_WIN_ID))
        {
            #if 1 // WUXX_20231111
            Video_Call_Close(WATCH_VIDEO_CALL_INCOMING_WIN_ID);
            MMK_CloseWin(WATCH_VIDEO_CALL_INCOMING_WIN_ID);
            #else
            video_call_stop();
            SCI_Sleep(1000);//延迟一下，要不可能窗口关闭了,视频界面没刷新
            MMK_CloseWin(win_id);
            #endif
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return res;
}

/***********************************************************/
//视频来电 或者视频中挂断或接听
/***********************************************************/
LOCAL void Video_Call_Accept_And_Hangup(GUI_POINT_T tp_up, MMI_WIN_ID_T win_id)
{
    GUI_RECT_T accept_rect = VIDEO_CALL_INCOMING_ACCRPT_RECT;
    GUI_RECT_T hangup_rect = VIDEO_CALL_INCOMING_HANGUP_RECT;
    GUI_RECT_T hangup_in_video_rect = VIDEO_CALL_IN_VIDEO_HANGUP_RECT;
    GUI_RECT_T out_call_hangup_rect = VIDEO_CALL_OUT_HANGUP_RECT;// 手表呼出APP不接听,手表挂断
    
    VIDEO_CALL_INFO *video_call_info = NULL;
    if (GUI_PointIsInRect(tp_up, accept_rect)) //接听
    {
        ZDT_LOG("[%s] accept video call", __FUNCTION__);
        video_call_info = (VIDEO_CALL_INFO *) MMK_GetWinUserData(win_id);
        if(video_call_info != NULL && !g_is_incomeing && video_call_info->call_type == VIDEO_CALL_INCOMING)
        {
            MMIAPISET_StopRing(MMISET_RING_TYPE_CALL);// MMIAPISET_StopRing须放在video_call_start 前面,不然可能导致来视频无声
            ////视频接通,30秒自动挂断定时器停止///
            Stop_Video_Call_Waiting_Timer();
            ///////////////////////////////////////////////////

             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
             // wuxx add .信号差的时候,按接听键给出提示--暂时无法接通,改为信号差或者连接中断更好
             //来电却突然无信号 无法接通的按接听键的提示和处理// VIDEO_INCOMING_BUT_NO_SIGNAL process wuxx
            if (MMIZDT_Net_Rssi_IsLow())
            {
                //MMIPUB_OpenAlertWaitingWin(TXT_VIDEO_CALL_CONNECT_TIMEOUT);// 弹出2 秒消失框
                Video_Call_Show_Toast(TXT_VIDEO_CALL_CONNECT_TIMEOUT); // wuxx add 
                #if 1 // WUXX_20231111
                Video_Call_Close(WATCH_VIDEO_CALL_INCOMING_WIN_ID);
                MMK_CloseWin(win_id);
                #else
                video_call_stop();
                ////SCI_Sleep(1000);//延迟一下，要不可能窗口关闭了,视频界面没刷新
                MMK_CloseWin(win_id);
                #endif
            
                return;
            }
             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            g_is_incomeing = TRUE;
            video_call_start(video_call_info->appId, video_call_info->channel_name, Video_Call_Status_Callback);
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else// wuxx add // VIDEO_INCOMING_BUT_NO_SIGNAL process wuxx ,此条件暂未进入,但也添加.避免NULL 等异常情况
        {
            // video_call_info == NULL 等异常情况--按接听键--给出提示
            if(MMK_IsOpenWin(WATCH_VIDEO_CALL_INCOMING_WIN_ID))
            {
                   //注释掉这三行,因为在接通以后点击挂断按键右边区域也会挂掉--此区域是未接通时的挂断按钮区域
                    ////Video_Call_Show_Toast(TXT_VIDEO_CALL_CONNECT_TIMEOUT); // wuxx add // 20231226 del

                
                #if 1 // WUXX_20231111
                    ////Video_Call_Close(WATCH_VIDEO_CALL_INCOMING_WIN_ID); // 20231226 del
                    ////MMK_CloseWin(win_id); // 20231226 del
                #else
                video_call_stop();
                ////SCI_Sleep(1000);//延迟一下，要不可能窗口关闭了,视频界面没刷新
                MMK_CloseWin(win_id);
                #endif
            }

        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                
    }
    else if(GUI_PointIsInRect(tp_up, hangup_rect)) //挂断
    {
        ZDT_LOG("[%s] hangup video call", __FUNCTION__);
        if(!g_is_incomeing)
        {
            Video_Call_Reject_Or_Hangup(win_id);//来电拒接
        }
    }
    else if(GUI_PointIsInRect(tp_up, hangup_in_video_rect) && g_is_video_call_show_hangup) //视频通话中挂断
    {
        if(g_is_incomeing && g_is_inVideo)//视频通话中挂断
        {
            // DrawCtrl_In_Video_Call(TRUE);
            video_call_stop();
            ////SCI_Sleep(1000);//延迟一下，要不可能窗口关闭了,视频界面没刷新
            MMK_CloseWin(win_id);
        }
        else
        {
            video_call_info = (VIDEO_CALL_INFO *) MMK_GetWinUserData(win_id);
            if(video_call_info != NULL && video_call_info->call_type == VIDEO_CALL_OUT)
            {
                Video_Call_Reject_Or_Hangup(win_id);//去电回拨拒接
            }
        }         
    }
    else if(GUI_PointIsInRect(tp_up, out_call_hangup_rect)&&(!g_is_inVideo)) //呼出,但APP不接听时, 手表挂断
    {
        ZDT_LOG("[%s] hangup video call 02", __FUNCTION__);
        SCI_TraceLow("Video_Call_Accept_And_Hangup,wuxx, out_call_hangup_rect 01, g_is_incomeing=%d", g_is_incomeing);
        if(g_is_incomeing)// or out call
        {
           SCI_TraceLow("Video_Call_Accept_And_Hangup,wuxx, out_call_hangup_rect 02, g_is_incomeing=%d", g_is_incomeing);
            Video_Call_Reject_Or_Hangup(win_id);//来电拒接
        }
    }

}

LOCAL void Video_Call_Close(MMI_WIN_ID_T win_id)
{
    VIDEO_CALL_INFO *video_call_info = NULL;
    g_is_incomeing = FALSE;
    video_call_stop();
    video_call_info = (VIDEO_CALL_INFO *) MMK_GetWinUserData(win_id);
    if(video_call_info != NULL)
    {
        SCI_FREE(video_call_info);
        MMK_SetWinUserData(win_id, NULL);
        //video_call_info = NULL;
    }
    Stop_Video_Call_Waiting_Timer();
    //ReleaseVideo_Call_Ui_Layer();
    video_call_backlight(FALSE);
    ////SCI_Sleep(1000);
    MMK_UpdateScreen();
    g_is_inVideo = FALSE;
	g_is_activeVideo = FALSE; 
}

//////////////////////////////////// incomming ///////////////////////////////////////
LOCAL MMI_RESULT_E HandleVideoIncommingWin(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T win_rect = MMITHEME_GetFullScreenRect();
    VIDEO_CALL_INFO *video_call_info = NULL;
    video_call_info = (VIDEO_CALL_INFO *) MMK_GetWinUserData(win_id);//wuxx add 20231111

    MMI_CheckAllocatedMemInfo();
    ZDT_LOG("[%s] msg_id:%d", __FUNCTION__, msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            video_call_backlight(TRUE);
            video_call_info = (VIDEO_CALL_INFO *) MMK_GetWinUserData(win_id);
            if(video_call_info != NULL && video_call_info->call_type == VIDEO_CALL_OUT)
            {
                Start_Video_Call_Waiting_Timer(win_id,VIDEO_CALL_TIME_OUT);
            }
            MMK_WinGrabFocus(MMK_ConvertIdToHandle(win_id)); 
        }
        break;
    case MSG_FULL_PAINT:
        MMK_GetWinRect(win_id, &win_rect);
        GUI_FillRect(&lcd_dev_info, win_rect, MMI_BLACK_COLOR);
        {
            video_call_info = (VIDEO_CALL_INFO *) MMK_GetWinUserData(win_id);

            //还未接通时, APP 远端挂断, 不在视频中时的远端挂断标志
            g_video_call_remote_hangup_but_not_invideo = FALSE; // 标志使用完要置FALSE 
            
            if(video_call_info != NULL)
            {
                if(video_call_info->call_type == VIDEO_CALL_INCOMING)
                {
                    Video_Call_Display_video_type(win_id, lcd_dev_info,MMI_VIDEO_CALL_STATE_INCOMING);
                    MMIAPISET_PlayCallRingByVol(MMISET_VOL_NINE, R_CALL_1, AUD_PLAY_FOREVER, MMISET_RING_TYPE_CALL, PNULL);// wuxx add, APP发找手表,正在响铃时,再来视频通话,无铃声问题
                }
                else
                {
                    ZDT_LOG("[%s] out video call", __FUNCTION__);
                    Video_Call_Display_video_type(win_id, lcd_dev_info,MMI_VIDEO_CALL_STATE_ACTIVE);
                    g_is_incomeing = TRUE;
                    video_call_start(video_call_info->appId, video_call_info->channel_name, Video_Call_Status_Callback);
                }                
            }         
        }
        
        break;
    case MSG_TIMER:
        {
            if (*(uint8*)param == video_call_waiting_timer_id)
            {
                MMK_CloseWin(win_id);
            }
            //ZDT_LOG("[%s] time id:%d", __FUNCTION__,video_call_waiting_timer_id);
        }
        break;
    case MSG_TP_PRESS_MOVE:
    case MSG_TP_PRESS_DOWN:
        break;
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T tp_up = {0};
            tp_up.x = MMK_GET_TP_X(param);
            tp_up.y = MMK_GET_TP_Y(param);
            video_call_info = (VIDEO_CALL_INFO *) MMK_GetWinUserData(win_id);
            if(video_call_info != NULL)
            {
                if(video_call_info->call_type == VIDEO_CALL_OUT)
                {
                    GUI_RECT_T hangup_in_video_rect = VIDEO_CALL_IN_VIDEO_HANGUP_RECT;
                    if(GUI_PointIsInRect(tp_up, hangup_in_video_rect)) //解决第一次开机呼出后不能退出的问题
                    {
                        Video_Call_Reject_Or_Hangup(win_id);
                        break;
                    }
                }
            }         
            Video_Call_Accept_And_Hangup(tp_up, win_id);
            Video_Call_Hangup_Show(TRUE);
        }
        break;
    case MSG_CLOSE_WINDOW:
        {
#if defined(VIDEO_CALL_AGORA_IN_CAMERA_SCREEN_INCOMMING_ERR) // wuxx add 20231111
            External_CloseDCApplet();// 防止在相机界面来视频接听挂断死机,须先退出CAMER CLOSE 
#endif
            g_is_inVideo = FALSE;
			g_is_activeVideo = FALSE; 
            Video_Call_Close(win_id);           
            //还未接通时, APP 远端挂断, 不在视频中时的远端挂断标志
            g_video_call_remote_hangup_but_not_invideo = FALSE; // 标志使用完要置FALSE 
        }
        break;
    case MSG_KEYDOWN_RED:
        break;

    case MSG_KEYUP_RED:
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //RED 键可挂断视频,来电铃声要停止,要发挂断的指令给APP // wuxx add 20231111
        YX_Net_Send_UPWATCHHANGUP(video_call_info->video_id);
        //if(video_call_info != NULL && (video_call_info->call_type == VIDEO_CALL_INCOMING || video_call_info->call_type == VIDEO_CALL_OUT))
        if(video_call_info != NULL && (video_call_info->call_type == VIDEO_CALL_INCOMING))
        {
            MMIAPISET_StopRing(MMISET_RING_TYPE_CALL);
        }
        Video_Call_Close(win_id);
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//if(g_is_incomeing && g_is_inVideo)//视频通话中挂断
        {
            MMK_CloseWin(win_id);
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

WINDOW_TABLE(MMI_VIDEO_CALL_INCOMMING_WIN_TAB) = 
{
    WIN_ID(WATCH_VIDEO_CALL_INCOMING_WIN_ID),
    WIN_FUNC((uint32)HandleVideoIncommingWin),
    WIN_STYLE(WS_DISABLE_FLING_CLOSE_WIN),
//    CREATE_BUTTON_CTRL(IMAGE_CALL_ACCEPT,  VIDEO_CALL_ACCEPT_BTN_ID),
//    CREATE_BUTTON_CTRL(IMAGE_CALL_HANGUP,  VIDEO_CALL_HANGUP_BTN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

//////////////////////////// video create //////////////////////////
LOCAL void Video_Call_IncomingWin(VIDEO_CALL_INFO video_call_info)
{
    MMI_WIN_ID_T win_id = WATCH_VIDEO_CALL_INCOMING_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    VIDEO_CALL_INFO *call_info = NULL;

    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }
    g_is_incomeing = FALSE;
    g_is_inVideo = FALSE;
    call_info = SCI_ALLOC_APP(sizeof(VIDEO_CALL_INFO));
    SCI_MEMSET(call_info,0,sizeof(VIDEO_CALL_INFO));
    SCI_MEMCPY(call_info, &video_call_info, sizeof(VIDEO_CALL_INFO));
    win_handle = MMK_CreateWin((uint32*)MMI_VIDEO_CALL_INCOMMING_WIN_TAB, NULL);
    MMK_SetWinUserData(win_handle, call_info);
}

PUBLIC BOOLEAN MMIVIDEO_IsOpenVideoWin(void)
{
    return MMK_IsOpenWin(WATCH_VIDEO_CALL_INCOMING_WIN_ID);
}

PUBLIC void MMIVIDEO_Incoming(char *appid, char *channel_name)
{
   // Video_Call_IncomingWin(NULL);
}

PUBLIC BOOLEAN VideoChat_IsInCall()
{
    return MMIVIDEO_IsOpenVideoWin();
}

PUBLIC uint8 Video_Call_Incoming(VIDEO_CALL_INFO video_call_info)
{
#if defined(VIDEO_CALL_AGORA_IN_CAMERA_SCREEN_INCOMMING_ERR)
    //WATCHCOM_CloseAudioOrVieo 统一处理
    External_CloseDCApplet();// 防止在相机界面来视频接听挂断死机,须先退出CAMER CLOSE // 保留
#endif

#if 1 // WUXX_20231111 来去电都执行一次,清空一些资源及变量
    Video_Call_Close(WATCH_VIDEO_CALL_INCOMING_WIN_ID);
#endif


    if(!VideoChat_IsInCall())
    {
	    WATCHCOM_CloseAudioOrVieo();
        if(video_call_info.call_type == VIDEO_CALL_OUT)//设备拨打app
        {
            Video_Call_Out_Close();
        }
        else
        {
            uint8 volume = MMIAPISET_GetCallRingVolume();
            MMIAPISET_PlayCallRingByVol(volume, R_CALL_1, AUD_PLAY_FOREVER, MMISET_RING_TYPE_CALL, PNULL);
        }
        Video_Call_IncomingWin(video_call_info);
    }
    ZDT_LOG("[%s] appid:%s uid:%s channel_name:%s video_id:%s", __FUNCTION__, video_call_info.appId, video_call_info.mUseIdStr,video_call_info.channel_name, video_call_info.video_id);
}

PUBLIC void Video_Call_Remote_Hangup()
{
    MMI_WIN_ID_T win_id = WATCH_VIDEO_CALL_INCOMING_WIN_ID;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (g_is_inVideo)
    {
        // RTC_MESSAGE_ROOM_EVENT_REMOTE_LEAVING  MSG 中发 YX_Net_Send_UPVCALLTIME , 避免重复 20231208
        ////////YX_Net_Send_UPVCALLTIME(g_video_call_info_tmp.video_id, g_baidu_video_call_timestamp_total);// APP 挂断,终端也发通话时长// wuxx del it 20231208

        //还未接通时, APP 远端挂断, 不在视频中时的远端挂断标志
        g_video_call_remote_hangup_but_not_invideo = FALSE;
    }
    else
    {
        //还未接通时, APP 远端挂断, 不在视频中时的远端挂断标志
        g_video_call_remote_hangup_but_not_invideo = TRUE;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(MMK_IsOpenWin(WATCH_VIDEO_CALL_INCOMING_WIN_ID))//wuxx add, APP两次挂断,手表死机修正//安卓APP与手表视频后挂断,在IOS APP上也有弹出呼叫界面,IOS上点击挂断，终端弹出--对方已挂断--后死机. 修正
    {
        Video_Call_Show_Toast(TXT_VIDEO_CALL_REMOTE_HANGUP);
    }

    if (MMK_IsOpenWin(win_id))
    {
        SCI_Sleep(1000);//延迟一下，要不可能窗口关闭了,视频界面没刷新
        MMK_CloseWin(win_id);
    }
    MMIAPISET_StopRing(MMISET_RING_TYPE_CALL);
}

PUBLIC uint8 Video_Call_Incoming_Test()
{
    VIDEO_CALL_INFO video_call_info = {0};
    strcpy(&video_call_info.appId, "f692ba8aa8b941afa9421d10a5f7b929");
    strcpy(&video_call_info.channel_name,"866973060000099");
    video_call_info.family_relative[0] = 0x7238;
    video_call_info.family_relative[1] = 0x7238;
    //MMIVideo_Call_ContactListWin_Close();
    video_call_info.call_type = VIDEO_CALL_INCOMING;
    Video_Call_Out_Close();
    Video_Call_IncomingWin(video_call_info);
    ZDT_LOG("[%s] appid:%s uid:%s channel_name:%s video_id:%s", __FUNCTION__, video_call_info.appId, video_call_info.mUseIdStr,video_call_info.channel_name, video_call_info.video_id);
}

