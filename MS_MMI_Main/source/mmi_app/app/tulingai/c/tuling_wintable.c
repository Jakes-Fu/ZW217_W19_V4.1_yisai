/****************************************************************************
** File Name:      mmiphsapp_wintab.c                                           *
** Author:          jianshengqi                                                       *
** Date:           03/22/2006                                              *
** Copyright:      2006 TLT, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the PHS                   *
/****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2006       Jianshq         Create
** 
****************************************************************************/

#define _MMI_TULING_WINTAB_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "std_header.h"
#include "window_parse.h"
#include "mmk_timer.h"
#include "mmk_app.h"
#include "mmicc_export.h"
#include "guitext.h"
#include "guilcd.h"
#include "guilistbox.h"
#include "guiedit.h"
#include "guilabel.h"
#include "guilistbox.h"
#include "mmi_default.h"
#include "mmi_common.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "mmi_appmsg.h"
#include "mmipub.h"
#include "mmi_common.h"

//#include "zdt_app.h"
#include "mmiidle_export.h"
#include "mmi_position.h"
#include "ldo_drvapi.h"
#include "mmisrvrecord_export.h"
#include "mmisrvfmm_export.h"

#include "mmifmm_pic.h"

#include "tuling_ai_chat.h"
#include "tuling_id.h"
#include "tulingai_image.h"

#include "zdt_app.h"





LOCAL MMI_RESULT_E  Handle_AIChat_WinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            );
PUBLIC void AppendAIListContent(uint8* content, BOOLEAN isLeft);

PUBLIC BOOLEAN AI_IsInAiChatWin();
LOCAL void AI_ShowBottom(MMI_WIN_ID_T win_id);




#define AI_VOICE_SEND_DIR "D:\\AIVoiceSend"
#define AI_VOICE_RCV_DIR "D:\\AIVoiceRcv"

#define AI_LIST_MAX_SIZE   60


#define  AI_TEST_MENU_LEN  20
#define  AI_TEST_MENU_NUM  5
const uint8 ai_test_menu_text[AI_TEST_MENU_NUM][AI_TEST_MENU_LEN]= 
{
    "Trace Del",
    "Trace Save",
    "Log Enable",
    "Log Disable",
    "SCI_ASSERT"

};



//ai chat
LOCAL RECORD_SRV_HANDLE ai_chat_record_handle = PNULL;
static uint8 ai_chat_record_anim_timer_id = 0;
LOCAL uint32 ai_record_update_anim_index =0;

static uint8 max_record_time_timer_id = 0;

#define AI_BOTTOM_HEIGHT 64 //52   // 58
#define AI_LIST_BOTTON_POSITON  (MMI_MAINSCREEN_HEIGHT-AI_BOTTOM_HEIGHT)//160



wchar ai_chat_dir_wstr[] = { 'A','i','R', 'e', 'c', 'o', 'r', 'd', 'i', 'n', 'g', 's', 0};

wchar ai_chat_received_recording_dir_wstr[] = { 'D', ':', '\\', 'A', 'i', 'R', 'e', 'c', 'o', 'r', 'd', 'i', 'n', 'g', 's', '\\', 'A', 'i', 'R', 'e', 'c', 'e', 'i', 'v', 'e', 'd', ' ', 'a', 'i','r', 'e', 'c', 'o', 'r', 'd', 'i', 'n', 'g', 's',0};

#define MMI_AICHAT_DIR           (ai_chat_dir_wstr)
#define MMI_AICHAT_DIR_LEN        13


typedef struct
{
    wchar               file_name[MMIFMM_PATHNAME_LEN +1];
    uint16              file_len;
    uint32              win_id;
    uint32              s_suffix_type;
    BOOLEAN             is_need_display;
    BOOLEAN             user_filename_valid;
    BOOLEAN             is_protect_checked;
    MMIFILE_DEVICE_E    s_cur_dev;
    MMIFMM_PATH_INFO_T  patch_info;
    MMIFILE_DEVICE_E    s_device_table[MMI_DEVICE_NUM];
    MMIFMM_SOFTKEY_TEXT_T sk_text_id;
    MMIFMM_OPENWIN_CALLBACK_T callback;
} MMIFMM_OPEN_FILE_PATH_INFO_T;


typedef struct
{
    MMIFMM_DATA_INFO_T * s_fmm_list_data_ptr;
    MMIFMM_PATH_INFO_T * s_fmm_current_path_ptr;
    MMIFMM_OPEN_FILE_PATH_INFO_T * s_file_path_info_ptr;
} MMIFMM_MAIN_WIN_DATA_T;

typedef struct
{
    MMIFMM_MAIN_WIN_DATA_T  *main_d_ptr;
    MMIFMM_PATH_INFO_T      *s_fmm_path_ptr;
    MMIFMM_PICTURE_INDEX_DATA_T    *s_pic_index_data_ptr;
    wchar                   *s_list_file_name_ptr;
    wchar                   s_list_file_name_len;
    MMI_CTRL_ID_T           list_ctrl_id; //list ctrl id
    MMI_CTRL_ID_T           tab_ctrl_id; //tab ctrl id
    MMI_CTRL_ID_T           com_ctrl_id;  //softkey ctrl_id
    MMIFMM_FILE_TYPE_E      cur_file_type; //current file type
} MMIFMM_VIEW_WIN_DATA_T;

typedef enum
{
    MMIFMM_APPEND_LIST_ITEM_TYPE,          //append
    MMIFMM_REPLACE_LIST_ITEM_TYPE,         //replace
    MMIFMM_NEED_LIST_ITEM_DATA_TYPE,       //need data
    MMIFMM_NEED_LIST_ITEM_CONTENT_TYPE    //need content
} MMIFMM_LIST_ITEM_TYPE_E;


WINDOW_TABLE( MMIAI_CHAT_WIN_TAB ) = 
{
    WIN_FUNC((uint32) Handle_AIChat_WinMsg),    
    WIN_ID( MMIAI_CHAT_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};



static AICHAT_RECORD_STATUS_T cur_ai_chat_disp_status = CHAT_RECORD_DISP_IDLE;


LOCAL void AiChat_BackLight(BOOLEAN is_alway_on)
{
      if(is_alway_on)
      {
		MMIDEFAULT_SetAlwaysHalfOnBackLight(FALSE);
		MMIDEFAULT_AllowTurnOffBackLight(FALSE);
		MMIDEFAULT_TurnOnBackLight();
      }
      else
      {
		MMIDEFAULT_AllowTurnOffBackLight(TRUE);
		MMIDEFAULT_SetAlwaysHalfOnBackLight(FALSE);
      }
}                                           


LOCAL uint16 FmmInitDevicePath (
    MMIFILE_DEVICE_E      device,     // [IN]
    wchar               *path_ptr   //[IN]/[OUT]
)
{
    uint16 i  = 0;

    if (device < MMI_DEVICE_NUM && (PNULL != path_ptr))
    {
        MMIAPICOM_Wstrncpy (path_ptr, MMIAPIFMM_GetDevicePath (device), MMIAPIFMM_GetDevicePathLen (device));
        i++;
        path_ptr[i] = MMIFILE_COLON;
        i++;
        path_ptr[i] = 0;
    }

    return (i);
}


PUBLIC BOOLEAN MMIFMM_OpenAiRecordWin (MMIFMM_OPENWIN_INFO_T * openwin_info)
{
    BOOLEAN result = FALSE;
    if (PNULL == openwin_info)
    {
        return result;
    }
    else
    {
        MMIFILE_DEVICE_E fs_dev = MMI_DEVICE_NUM;

        if (MMIAPIENG_GetIQModeStatus())
        {
            MMIPUB_OpenAlertWarningWin (TXT_IQ_DATA_PROCESSING);
            return FALSE;
        }

        if (MMIAPIUDISK_UdiskIsRun())
        {
            //u disk runing
            MMIPUB_OpenAlertWarningWin (TXT_COMMON_UDISK_USING);
            return FALSE;
        }

        fs_dev = MMIAPIFMM_GetFirstValidDevice();
        if (MMI_DEVICE_NUM == fs_dev)
        {
            //no valid device, get first exist devcie
            fs_dev = MMIAPIFMM_GetFirstExistDevice();
        }

        if (MMI_DEVICE_NUM > fs_dev)
        {
            MMIFMM_MAIN_WIN_DATA_T *main_win_p = PNULL;
            MMIAPIFMM_InitFolders();
         
            main_win_p = SCI_ALLOC_APPZ (sizeof (MMIFMM_MAIN_WIN_DATA_T));
            if (PNULL == main_win_p)
            {
                return FALSE;
            }
            main_win_p->s_fmm_current_path_ptr = SCI_ALLOC_APPZ (sizeof (MMIFMM_PATH_INFO_T));

            if (PNULL == main_win_p->s_fmm_current_path_ptr)
            {
                SCI_FREE(main_win_p);
                main_win_p = PNULL;
                return FALSE;
            }

            main_win_p->s_fmm_list_data_ptr = SCI_ALLOC_APPZ (sizeof (MMIFMM_DATA_INFO_T));

            if (PNULL == main_win_p->s_fmm_list_data_ptr)
            {
                SCI_FREE (main_win_p->s_fmm_current_path_ptr);
                SCI_FREE(main_win_p);
                main_win_p->s_fmm_current_path_ptr = PNULL;
                main_win_p = PNULL;
                return FALSE;
            }

            main_win_p->s_file_path_info_ptr = (MMIFMM_OPEN_FILE_PATH_INFO_T *) SCI_ALLOC_APPZ (sizeof (MMIFMM_OPEN_FILE_PATH_INFO_T));

            if (PNULL == main_win_p->s_file_path_info_ptr)
            {
                SCI_FREE(main_win_p->s_fmm_list_data_ptr);
                SCI_FREE (main_win_p->s_fmm_current_path_ptr);
                SCI_FREE(main_win_p);
                main_win_p->s_fmm_list_data_ptr = PNULL;
                main_win_p->s_fmm_current_path_ptr = PNULL;
                main_win_p = PNULL;
                return FALSE;
            }

            main_win_p->s_file_path_info_ptr->is_protect_checked = openwin_info->privacy_protect;
            main_win_p->s_file_path_info_ptr->s_suffix_type = openwin_info->find_suffix_type;

            main_win_p->s_file_path_info_ptr->sk_text_id = openwin_info->sk_text_id;
            if (PNULL != openwin_info->path_ptr && 0 != openwin_info->path_len
                    && (MMIAPIFMM_IsFolderExist (openwin_info->path_ptr, openwin_info->path_len)
                        || MMIAPIFMM_IsFileExist (openwin_info->path_ptr, openwin_info->path_len)
                       )
               )
            {
                MMIAPICOM_Wstrncpy (main_win_p->s_file_path_info_ptr->patch_info.pathname, openwin_info->path_ptr, openwin_info->path_len);
                main_win_p->s_file_path_info_ptr->patch_info.pathname_len = openwin_info->path_len;

                MMIAPICOM_Wstrncpy (main_win_p->s_fmm_current_path_ptr->pathname, openwin_info->path_ptr, openwin_info->path_len);
                main_win_p->s_fmm_current_path_ptr->pathname_len = openwin_info->path_len;

                if (PNULL != openwin_info->filename && 0 != openwin_info->file_len)
                {
                    MMIAPICOM_Wstrncpy (main_win_p->s_file_path_info_ptr->file_name, openwin_info->filename, openwin_info->file_len);
                    main_win_p->s_file_path_info_ptr->file_len = openwin_info->file_len;
                }

                if (openwin_info->file_len > 0)
                {
                    main_win_p->s_file_path_info_ptr->is_need_display = openwin_info->is_need_display;
                    main_win_p->s_file_path_info_ptr->user_filename_valid = TRUE;
                }
                else
                {
                    main_win_p->s_file_path_info_ptr->is_need_display = FALSE;
                    main_win_p->s_file_path_info_ptr->user_filename_valid = FALSE;
                }

                main_win_p->s_file_path_info_ptr->patch_info.path_depth = MMIFMM_GetPathDepth (main_win_p->s_file_path_info_ptr->patch_info.pathname, main_win_p->s_file_path_info_ptr->patch_info.pathname_len);

                if (MMIAPIFMM_GetDeviceStatus (openwin_info->path_ptr, MMIFILE_DEVICE_LEN))
                {
                    //current path device exist
                    fs_dev = MMIAPIFMM_GetDeviceTypeByPath (openwin_info->path_ptr, MMIFILE_DEVICE_LEN);
                }
                else
                {
                    FmmInitDevicePath (fs_dev, main_win_p->s_fmm_current_path_ptr->pathname);
                }

                main_win_p->s_file_path_info_ptr->s_cur_dev = fs_dev;
            }
            else
            {
                main_win_p->s_file_path_info_ptr->patch_info.path_depth = 1;
                main_win_p->s_file_path_info_ptr->s_cur_dev = fs_dev;
                FmmInitDevicePath (fs_dev, main_win_p->s_fmm_current_path_ptr->pathname);
            }

            main_win_p->s_fmm_current_path_ptr->pathname_len = (uint16) MMIAPICOM_Wstrlen (main_win_p->s_fmm_current_path_ptr->pathname);
            main_win_p->s_fmm_current_path_ptr->path_depth = MMIFMM_GetPathDepth (main_win_p->s_fmm_current_path_ptr->pathname, main_win_p->s_fmm_current_path_ptr->pathname_len);
            MMK_CreateWin ( (uint32 *) MMIAI_CHAT_WIN_TAB, (ADD_DATA) main_win_p);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin (TXT_PLEASE_INSERT_SD);
        }

        result = TRUE;
    }

    return result;
}




//not use anymore
BOOLEAN MMIAI_OpenChatWin(void)
{
    if(FALSE == MMK_IsOpenWin(MMIAI_CHAT_WIN_ID))
    {
        MMK_CreateWin((uint32*)MMIAI_CHAT_WIN_TAB,PNULL);
    }
    return TRUE;
}

BOOLEAN MMIAI_CloseChatWin(void)
{
    if(MMK_IsOpenWin(MMIAI_CHAT_WIN_ID))
    {
        MMK_CloseWin(MMIAI_CHAT_WIN_ID);
    }
    return TRUE;
}

PUBLIC void MMIAPIMENU_EnterAiChat(void)
{

    MMIAI_OpenChatWin();

    /*MMIFILE_DEVICE_E    file_dev = MMI_DEVICE_UDISK;
    MMIFMM_OPENWIN_INFO_T openwin_info = {0};
    wchar*              full_path = PNULL;
    uint16              full_path_len = MMIRECORD_SRV_MAX_FILE_NAME_LEN;
    const wchar*        file_dev_ptr = PNULL;
    uint16              file_dev_len = 0;
    uint32              file_suffix =  MMIFMM_FILE_TYPE(MMIFMM_MOVIE_ALL)| MMIFMM_MEDIA_TYPE(MMIFMM_MOVIE_MP4) |MMIFMM_MEDIA_TYPE(MMIFMM_MUSIC_MP3) | MMIFMM_MEDIA_TYPE(MMIFMM_MUSIC_AMR);
    //uint32              file_suffix =  MMIFMM_MEDIA_TYPE(MMIFMM_MUSIC_MP3) | MMIFMM_MEDIA_TYPE(MMIFMM_MUSIC_AMR);
    full_path = SCI_ALLOC_APP((MMIRECORD_SRV_MAX_FILE_NAME_LEN )*sizeof(wchar));
    if (PNULL == full_path)
    {
        return;
    }
    SCI_MEMSET(full_path, 0, ((MMIRECORD_SRV_MAX_FILE_NAME_LEN )* sizeof(wchar)));

    //check directory
    file_dev_ptr = MMIAPIFMM_GetDevicePath((file_dev));
    file_dev_len = MMIAPIFMM_GetDevicePathLen((file_dev));
    //MMIMULTIM_DIR_RECORDING
    //MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_RECORDINGS, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_RECORDINGS),
    //PNULL, 0, full_path, &full_path_len);
    MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_AI_RECORDINGS, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_AI_RECORDINGS),
    PNULL, 0, full_path, &full_path_len);
    
    openwin_info.path_ptr = full_path;
    openwin_info.path_len = full_path_len;
    openwin_info.filename = PNULL;
    openwin_info.file_len = 0;
    openwin_info.is_need_display = FALSE;
    openwin_info.privacy_protect = FALSE;
    openwin_info.find_suffix_type = file_suffix;
    openwin_info.sk_text_id.leftsoft_id = IMAGE_NULL;
    openwin_info.sk_text_id.middlesoft_id = IMAGE_NULL;
    openwin_info.sk_text_id.rightsoft_id = IMAGE_NULL;

    //MMK_CreateWin((uint32 *)MMISET_PEDOMETER_WIN_TAB,PNULL);
    
    MMIFMM_OpenAiRecordWin(&openwin_info);
    SCI_FREE(full_path);*/
}

/*****************************************************************************/
//  Description : GetCurrentDev
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_DEVICE_E GetCurrentDev(void)
{
    MMIFILE_DEVICE_E file_dev = MMI_DEVICE_UDISK;
	wchar *file_path_ptr = MMIRECORDSRV_GetRecordFilePath(ai_chat_record_handle);

    file_dev = MMI_DEVICE_UDISK;
    return file_dev;
}


LOCAL void AiRecordNotifyCallback(RECORD_SRV_HANDLE record_srv_handle, 
						  MMIRECORD_SRV_SUPPORT_TYPE_E support_type, 
						  MMIRECORD_SRV_RESULT_E result)
{

	if (record_srv_handle == ai_chat_record_handle
		&& MMIRECORD_SRV_SUPPORT_RECORD == support_type)
	{			
		
		if(!MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED ,GetCurrentDev(), MMIRECORD_SRV_MIN_NEED_SPACE, PNULL))
		{
			result = MMIRECORD_SRV_RESULT_NO_SPACE;
		}
		//HandleRecordEndMsg(result);//最好post msg到相应的窗口处理		
	}
}


/*LOCAL MMIRECORD_SRV_RESULT_E StartAiChatRecord(void)
{    
    MMI_STRING_T  call_name_str = {0};

    MMIRECORD_SRV_RECORD_PARAM_T record_param = {0};
    MMIFILE_DEVICE_E record_dev = MMIAPIRECORD_GetRecordStorage();

    if (PNULL != ai_chat_record_handle)
    {
        return MMIRECORD_SRV_RESULT_ERROR;
    }
    ai_chat_record_handle = MMIRECORDSRV_RequestRecordHandle(AiRecordNotifyCallback);
    if (PNULL == ai_chat_record_handle)
    {
        return MMIRECORD_SRV_RESULT_ERROR;
    }

    call_name_str.wstr_ptr = L"AIRecord";
    call_name_str.wstr_len = 9;

    record_param.fmt = MMISRVAUD_RECORD_FMT_AMR;
    record_param.prefix_ptr = PNULL;
    record_param.record_dev_ptr = &record_dev;
    record_param.record_file_id_ptr = PNULL;
    record_param.record_name_ptr = &call_name_str;
    //record_param.source = MMISRVAUD_RECORD_SOURCE_CALL;
    record_param.source = MMISRVAUD_RECORD_SOURCE_NORMAL;
    record_param.record_param_ptr = PNULL;
    record_param.volume = MMIAPISET_GetMultimVolume();
    record_param.is_create_file_name = TRUE;
    return MMIRECORDSRV_StartRecord(ai_chat_record_handle, record_param);        

    return MMIRECORD_SRV_RESULT_ERROR;

    
}*/



/*****************************************************************************/
//  Description : 停止录音（通话录音）
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMI_StopAiChatRecord(void)
{
	if (PNULL != ai_chat_record_handle)
	{
		MMIRECORDSRV_StopRecord(ai_chat_record_handle);
		MMIRECORDSRV_FreeRecordHandle(ai_chat_record_handle);
		ai_chat_record_handle = PNULL;
	}

}

LOCAL GUI_LCD_DEV_INFO anim_layer = {0};

LOCAL void CreateAnimLayer( 
                           MMI_WIN_ID_T      win_id
                           )
{
    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T    create_info = {0};  
        UILAYER_APPEND_BLT_T append_layer = {0};
        
        //s_layer_test[0]
        create_info.lcd_id = MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = 0;//94;
        create_info.offset_y = 240-52;//130;
        create_info.width = 52;
        create_info.height = 52;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;   
        UILAYER_CreateLayer(&create_info, &anim_layer);           
        //UILAYER_SetLayerAlpha(&anim_layer, 100);
//        UILAYER_SetLayerColorKey(&s_layer_test[2], TRUE, UILAYER_TRANSPARENT_COLOR);
        append_layer.lcd_dev_info = anim_layer;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);

    }
}




LOCAL void ClearAiChatRecordAnim(MMI_WIN_ID_T win_id)
{
    AI_ShowBottom(win_id);

    #if 0
    MMI_IMAGE_ID_T cur_img_id = 0;
    const GUI_LCD_DEV_INFO *lcd_info = MMITHEME_GetDefaultLcdDev();
    GUI_POINT_T      point = {0,240-52};//{94,130};
    GUI_BG_DISPLAY_T bg_display = {0};
    GUI_BG_T bg_ptr = {0};
    GUI_RECT_T      bg_rect;

    bg_rect.left = 0;//94;
    bg_rect.top= 240-52;//130;
    bg_rect.right= 52;//146;
    bg_rect.bottom = 240;//182;        
    bg_ptr.bg_type = GUI_BG_COLOR;
    bg_ptr.color = MMI_BLACK_COLOR;
    bg_display.win_handle = MMK_ConvertIdToHandle(win_id);
    bg_display.display_rect = bg_rect;
    bg_display.rect = bg_rect;               
    GUI_DisplayBg(&bg_ptr,&bg_display,MMITHEME_GetDefaultLcdDev());
    #endif
    #if 0//def UI_MULTILAYER_SUPPORT //yangyu delete test
    UILAYER_Clear(&anim_layer);
    #endif
}

LOCAL MMI_IMAGE_ID_T GetAiRecordAnimalFrame(uint32 cur_frame_index)
{
    MMI_IMAGE_ID_T  anim_frame[AICHAT_RECORD_ANIMAL_TOTAL_FRAME_NUM] = {IMAGE_AI_RECORD_ANIM_01, IMAGE_AI_RECORD_ANIM_02, 
                                                                        IMAGE_AI_RECORD_ANIM_03, IMAGE_AI_RECORD_ANIM_04};
    if(cur_frame_index < AICHAT_RECORD_ANIMAL_TOTAL_FRAME_NUM)
    {
        {
            return anim_frame[cur_frame_index];

        }
    }
    return 0;
}

#if 0 //old ui
LOCAL void DisplayAiChatRecordAnim(MMI_WIN_ID_T win_id)
{
    MMI_IMAGE_ID_T cur_img_id = 0;
    const GUI_LCD_DEV_INFO *lcd_info = MMITHEME_GetDefaultLcdDev();
    GUI_POINT_T      point = {0,240-52};//{94,130};
    GUI_BG_DISPLAY_T bg_display = {0};
    GUI_BG_T bg_ptr = {0};
    GUI_RECT_T      bg_rect;
    GUI_RECT_T      bg_rect2 = {240-52-1, 240-52-1, 240-1,240-1} ;

    bg_rect.left = 0;//94;
    bg_rect.top= 240-52;//130;
    bg_rect.right= 52;//146;
    bg_rect.bottom = 240;//182;    
    bg_ptr.bg_type = GUI_BG_COLOR;
    bg_ptr.color = MMI_BLACK_COLOR;
    bg_display.win_handle = MMK_ConvertIdToHandle(win_id);
    bg_display.display_rect = bg_rect;
    bg_display.rect = bg_rect;               
    GUI_DisplayBg(&bg_ptr,&bg_display,MMITHEME_GetDefaultLcdDev());
    bg_display.display_rect = bg_rect2;
    bg_display.rect = bg_rect2;               
    GUI_DisplayBg(&bg_ptr,&bg_display,MMITHEME_GetDefaultLcdDev());
    cur_img_id = GetAiRecordAnimalFrame(ai_record_update_anim_index);

    if(0 != cur_img_id)
    {
    #if 0//def UI_MULTILAYER_SUPPORT //yangyu delete test
        GUI_LCD_DEV_INFO main_dev_info = {0, 0};
        GUI_RECT_T rect0 = {0};
        //UILAYER_APPEND_BLT_T append_layer = {0};
        uint16 lcd_width = 0;
        uint16 lcd_height = 0;

        //append_layer.lcd_dev_info = anim_layer;
        //append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        //UILAYER_AppendBltLayer(&append_layer);
        //rect0.left = 0;
        //rect0.top = 0;
        //rect0.right = 52;
        //rect0.bottom = 52;
        rect0 = UILAYER_GetLayerRect(&anim_layer);
        
        //LCD_FillRect(&anim_layer, UILAYER_GetLayerRect(&anim_layer), MMI_RED_COLOR);
        GUIRES_DisplayImg(PNULL,
            &rect0,
            PNULL,
            win_id,
            cur_img_id,
            &anim_layer);

    #else 
            GUIRES_DisplayImg(&point,
                PNULL,
                PNULL,
                win_id,
                cur_img_id,
                lcd_info);

                GUIRES_DisplayImg(PNULL,
                &bg_rect2,
                PNULL,
                win_id,
                cur_img_id,
                lcd_info);
    #endif    
    }
}

#endif


void MMIAI_Chat_ShowData(uint8 status, char * err_str)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle[10] = {0x5FAE, 0x804A, 0}; //微聊 5FAE 804A 
    uint16 idle_wstr[20] = {0x8BF7, 0x6309, 0x952E, 0x5F00, 0x59CB, 0x8BB2, 0x8BDD, 0x002E, 0x002E, 0x002E, 0}; //请按键开始讲话...8BF7 6309 952E 5F00 59CB 8BB2 8BDD 002E 002E 002E 
    uint16 recording_wstr[20] = {0x6B63, 0x5728, 0x5F55, 0x97F3, 0x002E, 0x002E, 0x002E, 0}; //正在录音...6B63 5728 5F55 97F3 002E 002E 002E 
    uint16 recordok_wstr[10] = {0x5F55, 0x97F3, 0x6210, 0x529F, 0x0021, 0}; //录音成功!5F55 97F3 6210 529F  0021 
    uint16 recordfail_wstr[10] = {0x5F55, 0x97F3, 0x5931, 0x8D25, 0x0021, 0}; //录音失败!5F55 97F3 5931 8D25  0021 
    uint16 sending_wstr[20] = {0x6B63, 0x5728, 0x53D1, 0x9001, 0x8BED, 0x97F3, 0x002E, 0x002E, 0x002E, 0x002E, 0}; //正在发送语音....6B63 5728 53D1 9001 8BED 97F3 002E 002E 002E 002E 
    uint16 sendok_wstr[10] = {0x53D1, 0x9001, 0x6210, 0x529F, 0x0021, 0}; //发送成功!53D1 9001 6210 529F 0021 
    uint16 sendfail_wstr[10] = {0x53D1, 0x9001, 0x5931, 0x8D25, 0x0021, 0}; //发送失败!53D1 9001 5931 8D25 0021 
    GUI_FONT_T f_big =WATCH_DEFAULT_BIG_FONT;
    GUI_FONT_T f_mid =MMI_DEFAULT_NORMAL_FONT;
    
    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = MMI_MAINSCREEN_WIDTH-1;
    rect.bottom = MMI_MAINSCREEN_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    cur_rect = rect;
    cur_rect.top    = 2; 
    cur_rect.bottom = cur_rect.top + 24;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    switch(status)
    {
        case CHAT_RECORD_DISP_IDLE:
                cur_str_t.wstr_len = MMIAPICOM_Wstrlen(idle_wstr);
                cur_str_t.wstr_ptr = idle_wstr;
            break;
            
        case CHAT_RECORD_DISP_RECORDING:
                cur_str_t.wstr_len = MMIAPICOM_Wstrlen(recording_wstr);
                cur_str_t.wstr_ptr = recording_wstr;
            break;
            
        case CHAT_RECORD_DISP_RECORD_OK:
                cur_str_t.wstr_len = MMIAPICOM_Wstrlen(recordok_wstr);
                cur_str_t.wstr_ptr = recordok_wstr;
            break;
            
        case CHAT_RECORD_DISP_RECORD_FAIL:
                cur_str_t.wstr_len = MMIAPICOM_Wstrlen(recordfail_wstr);
                cur_str_t.wstr_ptr = recordfail_wstr;
            break;
            
        case CHAT_RECORD_DISP_SENDING:
                cur_str_t.wstr_len = MMIAPICOM_Wstrlen(sending_wstr);
                cur_str_t.wstr_ptr = sending_wstr;
            break;
            
        case CHAT_RECORD_DISP_SEND_OK:
                cur_str_t.wstr_len = MMIAPICOM_Wstrlen(sendok_wstr);
                cur_str_t.wstr_ptr = sendok_wstr;
            break;
            
        case CHAT_RECORD_DISP_SEND_FAIL:
                cur_str_t.wstr_len = MMIAPICOM_Wstrlen(sendok_wstr);
                cur_str_t.wstr_ptr = sendok_wstr;
            break;
            
        default:
                cur_str_t.wstr_len = MMIAPICOM_Wstrlen(idle_wstr);
                cur_str_t.wstr_ptr = idle_wstr;
            break;
    }
    
    cur_rect.top    = cur_rect.bottom+30; 
    cur_rect.bottom = cur_rect.top + 24;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    if(err_str != PNULL)
    {
        SCI_MEMSET(disp_str,0,sizeof(disp_str));
        SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
        cur_rect.top    = cur_rect.bottom+10; 
        cur_rect.bottom = cur_rect.top + 24;
        cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "%s",err_str);
        cur_str_t.wstr_ptr = disp_wstr;
        MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&cur_rect,       
            (const GUI_RECT_T      *)&cur_rect,     
            (const MMI_STRING_T    *)&cur_str_t,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
    }
    return;
}



BOOLEAN MMIAI_UpdateChatWin(uint8 status,char * err_str)
{
    cur_ai_chat_disp_status = status;
    if(MMK_IsOpenWin(MMIAI_CHAT_WIN_ID))
    {
        MMIAI_Chat_ShowData(status,err_str);
    }
    return TRUE;
}
/*
BOOLEAN MMIAI_UpdateChatWinErr(AIVocRecordErrStatus err_code)
{
    if(MMK_IsOpenWin(MMIAI_CHAT_WIN_ID))
    {
        char tmp_buf[100] = {0};
        sprintf(tmp_buf,"Err:%d",err_code);
        MMIAI_Chat_ShowData(cur_ai_chat_disp_status,tmp_buf);
    }
    return TRUE;
}
*/
BOOLEAN MMIAI_UpdateChatStr(char * err_str)
{
    if(MMK_IsOpenWin(MMIAI_CHAT_WIN_ID))
    {
        MMIAI_Chat_ShowData(cur_ai_chat_disp_status,err_str);
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : add data to record list item
//  Global resource dependence :
//  Author: dong.chunguang
//  Note:
/*****************************************************************************/
LOCAL void AppendTwoLineAnimTextAndTextAiChatListItem (
    MMI_WIN_ID_T            win_id,
    MMI_CTRL_ID_T           ctrl_id,
    MMI_TEXT_ID_T           left_softkey_id,
    MMIFMM_FILE_TYPE_E      file_type,
    const wchar*            filename,
    uint16                  filename_len,
    uint32                  total_time,
    BOOLEAN                 path_is_valid,
    MMIFMM_PATH_INFO_T      *current_path_ptr,
    uint16                  item_index,
    uint16                  item_content_index,
    MMIFMM_LIST_ITEM_TYPE_E type,
    BOOLEAN                 is_two_line
)
{
     
    GUILIST_ITEM_T       		item_t    =  {0};
    GUILIST_ITEM_DATA_T  		item_data = {0};/*lint !e64*/
    uint16          			list_name_len  = 0;
    uint16          			full_name_len  = 0;
    wchar 						temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    wchar  						list_file_name [MMIFMM_FULL_FILENAME_LEN+1] = {0};

    MMIFMM_VIEW_WIN_DATA_T   	*view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);
    MMIFMM_DETAIL_DATA_T    	detail_data = {0};
    uint16                    	name_len  = 0;
    uint16                   	suffix_len = 0;
    uint16                     	path_len = 0;
    int32                       i = 0;
    uint8           			total_time_str[GUILIST_STRING_MAX_NUM]  = {0};
    MMI_STRING_T    			total_str_t = {0};
	wchar 						time_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
	wchar 						record_time_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
	//uint16 						time_len = 0;

    if (PNULL == view_win_d)
    {
        SCI_TRACE_LOW ("AppendTwoLineAnimTextAndTextAiChatListItem PNULL == view_win_d!");
        return;
    }

    /*Quick Action*/
    if (MMIFMM_FILE_TYPE_FOLDER != file_type)
    {
        if(is_two_line)
        {        
            item_t.item_style = GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT; //GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT;
        }
        else
        {        
            item_t.item_style = GUIITEM_STYLE_ONE_LINE_TWO_ICON_TWO_TEXT; //GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT;
        }            
 //       item_t.left_item_style = GUIITEM_STYLE_2TEXT_AND_TEXT_WITH_LEFT_FOCUS_MS;
    }

    //split file name
    //if (MMIFMM_FILE_TYPE_MUSIC == file_type ||MMIFMM_FILE_TYPE_MOVIE == file_type)
    if (MMIFMM_FILE_TYPE_MUSIC == file_type )
    {
        MMIAPIFMM_SplitFullPathExt(filename,
            filename_len,
            PNULL,&path_len,
            PNULL,&name_len,
            PNULL,&suffix_len);
    }

    for (i = path_len - 1; MMIFILE_SLASH != filename[i] && i > 0; i--)
    {
        NULL;/*lint !e522 */
    }
    //End

#if 0
    if (MMIFMM_FILE_TYPE_MUSIC == file_type)
    {
         item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_2TEXT_MS;
    }

    item_t.first_letter = GetFirstLatterByName (filename);
#endif
    if (type == MMIFMM_APPEND_LIST_ITEM_TYPE)
    {
        GUILIST_AppendItem (ctrl_id, &item_t);
    }
    else if (type == MMIFMM_REPLACE_LIST_ITEM_TYPE)
    {
        GUILIST_ReplaceItem (ctrl_id, &item_t, item_index);
    }
    else
    {
        SCI_MEMSET (view_win_d->s_list_file_name_ptr, 0, ( (MMIFMM_FULL_FILENAME_LEN + 1) * sizeof (wchar)));
#if 0 
        /* full path */
        if (path_is_valid)
        {
            uint16  index = MMIFMM_GetFinallySlashIndex (filename, (filename_len - 1));
            list_name_len = (uint16) (filename_len - 1 - index);
            MMIAPICOM_Wstrncpy (list_file_name,  &filename[index + 1], list_name_len);

            full_name_len = filename_len;
            MMIAPICOM_Wstrncpy (view_win_d->s_list_file_name_ptr, filename, full_name_len);
        }
        else
        {
            list_name_len = filename_len;
            MMIAPICOM_Wstrncpy (list_file_name, filename, list_name_len);
            full_name_len = (uint16) MMIFMM_CombineToFileName (current_path_ptr->pathname, current_path_ptr->pathname_len,
                            filename, filename_len,
                            view_win_d->s_list_file_name_ptr, (uint16) MMIFMM_FULL_FILENAME_LEN);
        }

       
		//name
        if (MMIFMM_FILE_TYPE_MUSIC == file_type ||MMIFMM_FILE_TYPE_MOVIE == file_type)
        {
             item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
             item_data.item_content[1].item_data.text_buffer.wstr_len = path_len - i;
             item_data.item_content[1].item_data.text_buffer.wstr_ptr = &filename[i];
        }
#endif    

        item_data.item_content[0].item_data_type    = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = IMAGE_AI_VOICE_LEFT_BG;//IMAGE_MSN_VOICE_BG;     
		
        //set recording start time
        //if (MMIFMM_FILE_TYPE_MUSIC == file_type ||MMIFMM_FILE_TYPE_MOVIE == file_type)
        if (MMIFMM_FILE_TYPE_MUSIC == file_type )
        {
            MMI_TM_T  tm = {0};
            tm = MMIAPICOM_Second2Tm (total_time);
            SCI_MEMSET (total_time_str, 0, sizeof (total_time_str));
            SCI_MEMSET (record_time_wstr, 0, sizeof (record_time_wstr));
            SCI_MEMSET (temp_wstr, 0, sizeof (temp_wstr));
            MMIAPISET_FormatTimeStrByTime (tm.tm_hour, tm.tm_min, total_time_str, GUILIST_STRING_MAX_NUM);
            MMIAPICOM_StrToWstr (total_time_str, temp_wstr);
            MMIAPICOM_Wstrncpy(record_time_wstr,temp_wstr,5);
            item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[2].item_data.text_buffer.wstr_ptr = record_time_wstr;
            item_data.item_content[2].item_data.text_buffer.wstr_len = (uint16) MMIAPICOM_Wstrlen (record_time_wstr);
        }

        if (MMIFMM_FILE_TYPE_FOLDER != file_type)
        {
            MMISRVAUD_CONTENT_INFO_T  mp3_file_info = {0};
            SCI_MEMSET (&detail_data , 0, sizeof (MMIFMM_DETAIL_DATA_T));
            detail_data.time = view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[item_index]->time;
            detail_data.file_size = view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[item_index]->file_size;
            //detail_data.filename_len = (uint16) MMIFMM_CombineFullFileName (view_win_d->main_d_ptr->s_fmm_list_data_ptr, PNULL, item_index, detail_data.filename, MMIFMM_FILENAME_LEN);
            MMIAPICOM_Wstrncpy (detail_data.filename,
                    view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[item_index]->filename_ptr,
                    view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[item_index]->filename_len
                   );
            detail_data.filename_len = view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[item_index]->filename_len;
            detail_data.type = view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[item_index]->type;

            MMISRVAUD_GetFileContentInfo (
                &detail_data.filename,
                MMIAPICOM_Wstrlen (&detail_data.filename),
                &mp3_file_info
                );
            SCI_MEMSET (temp_wstr, 0, sizeof (temp_wstr));
            SCI_MEMSET (time_wstr, 0, sizeof (time_wstr));
            sprintf ( (char*) temp_wstr, "%d\"", mp3_file_info.total_time);
            MMIAPICOM_StrToWstr (temp_wstr, time_wstr);

            item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[3].item_data.text_buffer.wstr_ptr = time_wstr;
            item_data.item_content[3].item_data.text_buffer.wstr_len = (uint16) MMIAPICOM_Wstrlen (time_wstr);
        }


        
        if (type == MMIFMM_NEED_LIST_ITEM_DATA_TYPE)
        {
            GUILIST_SetItemData (ctrl_id, &item_data, item_index);
        }
        else if (type == MMIFMM_NEED_LIST_ITEM_CONTENT_TYPE)
        {
            GUILIST_SetItemContent (ctrl_id, &item_data.item_content[item_content_index], item_index, item_content_index);
        }
    }
}

PUBLIC BOOLEAN AI_IsInAiChatWin()
{
    MMI_WIN_ID_T win_id = MMK_GetFocusWinId();
    if(win_id == MMIAI_CHAT_WIN_ID)
    {
        return TRUE;
    }
    return FALSE;
}

PUBLIC BOOLEAN AI_IsChatWinOpen()
{
    
    if(MMK_IsOpenWin(MMIAI_CHAT_WIN_ID) )
    {
        return TRUE;
    }
    return FALSE;
}

//yangyu add new style
#define NEW_LEFT_RIGHT_STYLE
LOCAL THEMELIST_ITEM_STYLE_T left_style = {0};
LOCAL THEMELIST_ITEM_STYLE_T right_style = {0};

//yangyu end

PUBLIC void AppendAIListContent(uint8* content, BOOLEAN isLeft)
{
    BOOLEAN ret = FALSE;

     
    GUILIST_ITEM_T       		item_t    =  {0};
    GUILIST_ITEM_DATA_T  		item_data = {0};
    wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};
    uint16 total_item_num = 0;

    #ifdef NEW_LEFT_RIGHT_STYLE   
    const uint16 h_gap = 2;
    const uint16 w_gap = 2;
    const uint16 img_height = 40; // xiongkai 50;
    const uint16 img_width = 145; // xiongkai  170;//150;
    const uint16 height = img_height + 2*h_gap;
    GUI_RECT_T l_rect0 = { w_gap, h_gap,  img_width + w_gap,  img_height+h_gap};//bg
    GUI_RECT_T l_rect1 = { w_gap+34+2,  h_gap,  img_width-2 , img_height+h_gap};//text
    GUI_RECT_T r_rect0 = { MMI_MAINSCREEN_WIDTH - img_width- 2*w_gap, h_gap,   MMI_MAINSCREEN_WIDTH - w_gap,  img_height+h_gap};//bg
    GUI_RECT_T r_rect1 = { MMI_MAINSCREEN_WIDTH - img_width- 2*w_gap+4,  h_gap,  MMI_MAINSCREEN_WIDTH - 2*w_gap-2-34 , img_height+h_gap};//text
    
    
    //uint32 i = GUILIST_GetTotalItemNum(ctrl_id);
    THEMELIST_ITEM_STYLE_T* pItemStyle = THEMELIST_GetStyle(GUIITEM_STYLE_ONE_LINE_BGICON_TEXT_MS);
    SCI_MEMCPY(&left_style, pItemStyle, sizeof(*pItemStyle));
    SCI_MEMCPY(&right_style, pItemStyle, sizeof(*pItemStyle));
    
    left_style.height = left_style.height_focus = img_height + h_gap*2;
    right_style.height = right_style.height_focus = img_height + h_gap*2;
    
    left_style.content[0].rect = left_style.content[0].rect_focus = l_rect0;
    left_style.content[1].rect = left_style.content[1].rect_focus = l_rect1;
    
    right_style.content[0].rect = right_style.content[0].rect_focus = r_rect0;
    right_style.content[1].rect = right_style.content[1].rect_focus = r_rect1;

    
  
    #endif

    #if 1   //xiongkai add 
    //is left  :  左侧对话框      需求  左侧靠右对齐显示，右侧靠左显示。
        left_style.content[1].state = GUIITEM_CONTENT_STATE_TEXT_R_ALIGN;
        right_style.content[1].state = GUIITEM_CONTENT_STATE_NONE;	
    #endif

    if(!MMK_IsOpenWin(MMIAI_CHAT_WIN_ID))
    {
        return;
    }
    
    
    item_t.item_data_ptr = &item_data; 
    item_t.item_style =  GUIITEM_STYLE_ONE_LINE_BGICON_TEXT_MS;//GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT;//GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_MASK_MS;//GUIITEM_STYLE_ONE_LINE_ICON_TEXT_MS;//GUIITEM_STYLE_ONE_LINE_ICON_TEXT;//GUIITEM_STYLE_ONE_LINE_TEXT; 
    #ifdef NEW_LEFT_RIGHT_STYLE 
    item_t.item_style =  GUIITEM_STYLE_CUSTOMIZE;
    #endif

    
    if(isLeft)   //is left  : 	左侧对话框     需求  左侧靠右对齐显示，右侧靠左显示。
    {
   //     item_t.left_item_style = GUIITEM_STYLE_ONE_LINE_TEXT_WITH_RIGHT_FOCUS_MS; //GUIITEM_STYLE_ONE_LINE_TEXT_WITH_LEFT_FOCUS_MS;	
        item_data.item_content[1].is_custom_font_color = TRUE;
        item_data.item_content[1].custom_font_color = MMI_RED_COLOR;		
        item_data.item_content[1].custom_font_color_focus = MMI_RED_COLOR;				
    }
    else
    {
  //      item_t.right_item_style =GUIITEM_STYLE_ONE_LINE_TEXT_WITH_LEFT_FOCUS_MS; // GUIITEM_STYLE_ONE_LINE_TEXT_WITH_RIGHT_FOCUS_MS;
    }
     //item_t.left_item_style = GUIITEM_STYLE_2TEXT_AND_TEXT_WITH_LEFT_FOCUS_MS;
    
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;        
    item_data.item_content[0].item_data.image_id = isLeft ? IMAGE_AI_VOICE_LEFT_BG : IMAGE_AI_VOICE_RIGHT_BG;
    

    
    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = buff;
    //MMIAPICOM_StrToWstr(content , item_data.item_content[0].item_data.text_buffer.wstr_ptr);// luan ma,Garbled
    //GUI_GBToWstr(item_data.item_content[0].item_data.text_buffer.wstr_ptr, (const uint8*)content, SCI_STRLEN(content));//luan ma,has chinese
    //GUI_GB2UCS(item_data.item_content[0].item_data.text_buffer.wstr_ptr, (const uint8*)content, SCI_STRLEN(content));//luan ma,has chinese
    //GUI_GBKToWstr(item_data.item_content[0].item_data.text_buffer.wstr_ptr, (const uint8*)content, SCI_STRLEN(content));//luan ma,has chinese
    GUI_UTF8ToWstr(item_data.item_content[1].item_data.text_buffer.wstr_ptr, GUILIST_STRING_MAX_NUM + 1, content, SCI_STRLEN(content));

    item_data.item_content[1].item_data.text_buffer.wstr_len = (uint16) MMIAPICOM_Wstrlen (item_data.item_content[1].item_data.text_buffer.wstr_ptr);
    ret = GUILIST_AppendItem (MMIAI_CHAT_LIST_CTRL_ID, &item_t);

    if(ret)
    {
        total_item_num = CTRLLIST_GetTotalItemNum(MMIAI_CHAT_LIST_CTRL_ID);
        if(total_item_num >= 1)
        {
            #ifdef NEW_LEFT_RIGHT_STYLE 
            GUILIST_SetItemStyleEx(MMIAI_CHAT_LIST_CTRL_ID, total_item_num -1, isLeft ? &left_style : &right_style);
            #endif
            CTRLLIST_SetSelectedItem(MMIAI_CHAT_LIST_CTRL_ID, total_item_num-1, TRUE);
            GUILIST_SetCurItemIndex(MMIAI_CHAT_LIST_CTRL_ID, total_item_num-1); 
        }
        
        #ifndef WIN32
        MMK_UpdateScreen();
        #endif
       
    }
    
}


/*****************************************************************************/
//  Description : 添加列表recrodlist的item
//  Global resource dependence :
//  Author: dong.chunguang
//  Note:
/*****************************************************************************/
LOCAL void AiChatAppendRecordListItem (//当前索引
    MMI_WIN_ID_T win_id ,
    MMI_CTRL_ID_T        ctrl_id,
    MMI_TEXT_ID_T        left_softkey_id,
    MMIFMM_DATA_INFO_T   *list_data_ptr,
    MMIFMM_PATH_INFO_T   *current_path_ptr
)
{
    uint16 i = 0;
    uint32 max_num = 0;
	//添加PICTHUMBNAIL_SUPPORT控制
    BOOLEAN is_two_line = FALSE;
#ifdef  VIDEOTHUMBNAIL_SUPPORT     
    BOOLEAN                  is_videotwo_line = FALSE;
#endif
	MMI_HANDLE_T list_handle = MMK_ConvertIdToHandle (ctrl_id);


    max_num = MIN (MMIFMM_FILE_NUM, list_data_ptr->folder_num + list_data_ptr->file_num);

    for (i = 0; i < max_num; i++)
    {
        AppendTwoLineAnimTextAndTextAiChatListItem (win_id, list_handle,
                                              left_softkey_id,
                                              list_data_ptr->data[i]->type,
                                              list_data_ptr->data[i]->filename_ptr,
                                              list_data_ptr->data[i]->filename_len,
                                              list_data_ptr->data[i]->time,
                                              list_data_ptr->path_is_valid,
                                              current_path_ptr,
                                              i,
                                              0,
                                              MMIFMM_APPEND_LIST_ITEM_TYPE,
                                              list_data_ptr->data[i]->pad
                                             );
    }
}


/*****************************************************************************/
//  Description : handle record list append data
//  Global resource dependence :
//  Author: dong.chunguang
//  Note:
/*****************************************************************************/
LOCAL void AiChatHandleRecordListAppendData (
    MMI_WIN_ID_T         win_id,
    DPARAM param,
    MMI_HANDLE_T ctrl_handle,
    MMIFMM_LIST_ITEM_TYPE_E type
)
{
    GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = PNULL;
    GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = PNULL;
    uint16 index = 0;
    MMIFMM_VIEW_WIN_DATA_T *view_win_d = PNULL;
    MMI_HANDLE_T list_handle = MMK_ConvertIdToHandle (ctrl_handle);
	//添加PICTHUMBNAIL_SUPPORT控制
    BOOLEAN is_two_line      = FALSE;
#ifdef  VIDEOTHUMBNAIL_SUPPORT 
    BOOLEAN  is_videotwo_line = FALSE;
#endif

    view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);

    if (PNULL != view_win_d)
    {

        if (MMIFMM_NEED_LIST_ITEM_DATA_TYPE == type)
        {
            need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*) param;

            //SCI_ASSERT( PNULL != need_item_data_ptr );
            if (PNULL == need_item_data_ptr)
            {
                //SCI_TRACE_LOW:"MMIFMM:[FmmHandleListAppendData]PNULL == need_item_data_ptr"
                return;
            }

            index = need_item_data_ptr->item_index;
        }
        else if (MMIFMM_NEED_LIST_ITEM_CONTENT_TYPE == type)
        {
            need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*) param;

            //SCI_ASSERT( PNULL != need_item_content_ptr );
            if (PNULL == need_item_content_ptr)
            {
                return;
            }

            index = need_item_content_ptr->item_index;
            //s_is_picfolder = MMIFMM_IsDefaultPicFolder(view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname,view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname_len);
        }
        else
        {
            return;
        }

        if (index < view_win_d->main_d_ptr->s_fmm_list_data_ptr->file_num + view_win_d->main_d_ptr->s_fmm_list_data_ptr->folder_num)
        {
            AppendTwoLineAnimTextAndTextAiChatListItem (win_id,
                                                  list_handle,
                                                  view_win_d->main_d_ptr->s_file_path_info_ptr->sk_text_id.leftsoft_id,
                                                  view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->type,
                                                  view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_ptr,
                                                  view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len,
                                                  view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->time,
                                                  view_win_d->main_d_ptr->s_fmm_list_data_ptr->path_is_valid,
                                                  view_win_d->main_d_ptr->s_fmm_current_path_ptr,
                                                  index,
                                                  0,
                                                  type,
                                                  view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->pad);
        }
        else
        {
        }
        //GUILIST_SetSelectedItem(list_handle,view_win_d->main_d_ptr->s_fmm_list_data_ptr->file_num-1, TRUE);
        //GUILIST_SetCurItemIndex(list_handle,view_win_d->main_d_ptr->s_fmm_list_data_ptr->file_num-1);
    }
}

LOCAL void AiUpdateRecordListData (
    MMI_WIN_ID_T           win_id,
    MMI_CTRL_ID_T          ctrl_id,
    MMIFMM_READ_TYPE_E     type,
    MMI_TEXT_ID_T          sfk_left_id,
    BOOLEAN                is_empty
)
{
    uint16      max_num = 0;

    MMIFMM_SORT_TYPE_E  sort = MMIFMM_SORT_TYPE_TIME;
    GUILIST_EMPTY_INFO_T empty_info = {0};
    MMIFMM_VIEW_WIN_DATA_T *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);
    //SCI_TRACE_LOW:"MMIFMM: UpdateListData, win_id =%d, ctrl_id = %d, type = %d."
    
    if (PNULL == view_win_d)
    {
        return;
    }
	/*
    if (view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth <= 1)
    {
        GUIFORM_SetChildDisplay (MMIFMM_UDISK_TITLE_BUTTON_FORM_CTRL_ID, MMIFMM_UDISK_TITLE_BUTTON_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
    }
    else
    {
        GUIFORM_SetChildDisplay (MMIFMM_UDISK_TITLE_BUTTON_FORM_CTRL_ID, MMIFMM_UDISK_TITLE_BUTTON_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
    }
	*/
    max_num = GUILIST_GetTotalItemNum (ctrl_id);
    if (max_num > 0)
    {
        GUILIST_SetTextListMarkable (ctrl_id, FALSE);
        GUILIST_RemoveAllItems (ctrl_id);
    }

    //保存排序类型
    //sort = MMIFMM_GeFileSortType();
    if (is_empty)
    {
    	char 				str_welcome[256] = {0};
        FmmUpdateValidPath (win_id, FALSE);

        ///MMIFMM_FreeFileInfoBuffer(FALSE);
        MMIFMM_FreeFileInfo (view_win_d->main_d_ptr->s_fmm_list_data_ptr, FALSE);
        SCI_MEMSET (view_win_d->main_d_ptr->s_fmm_list_data_ptr , 0, sizeof (MMIFMM_DATA_INFO_T));

        MMIAPICOM_Wstrncpy (view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname, view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname, view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len);
        view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname_len = view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len;
        view_win_d->main_d_ptr->s_fmm_list_data_ptr->path_depth = view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth;

        view_win_d->main_d_ptr->s_fmm_list_data_ptr->sort = sort;

        // 读当前路径数据
        max_num = MMISET_GetCurrentPathFile (view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname, view_win_d->main_d_ptr->s_fmm_list_data_ptr);
		MMIAPICOM_WstrToStr((wchar*)view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname, (char*)str_welcome);
		SCI_TRACE_LOW("dcg get input text %s",str_welcome);
    }
    else
    {
        max_num = (uint16) (view_win_d->main_d_ptr->s_fmm_list_data_ptr->file_num + view_win_d->main_d_ptr->s_fmm_list_data_ptr->folder_num);
    }

    GUILIST_SetMaxItem (ctrl_id, max_num, TRUE);
    // 增加到listbox 中
    GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
    AiChatAppendRecordListItem(win_id, ctrl_id,  sfk_left_id, view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr);
    GUILIST_SetSelectedItem(ctrl_id,max_num-1, TRUE);
    GUILIST_SetCurItemIndex(ctrl_id,max_num-1);
    //FmmAppendListItem (win_id, ctrl_id,  sfk_left_id, view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr);
    if (max_num > 0)
    {
    	GUI_BG_T    common_bg = {0};

        common_bg.bg_type = GUI_BG_IMG;
        common_bg.img_id  = IMAGE_THEME_BG;
	GUIWIN_SetSoftkeyCommonBg(win_id, &common_bg);
	    
    }
    else
    {
        //empty_info.text_id = qtn_sett_ilist_none_bc;
        //GUILIST_SetEmptyInfo(ctrl_id, &empty_info);
    }
}


LOCAL MMI_RESULT_E AiChatListViewWinOpenCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_MAIN_WIN_DATA_T *main_win_d = (MMIFMM_MAIN_WIN_DATA_T*) MMK_GetWinAddDataPtr (win_id);
    MMIFMM_VIEW_WIN_DATA_T *view_win_d = PNULL;

    if (PNULL != main_win_d)
    {
        view_win_d = (MMIFMM_VIEW_WIN_DATA_T *) SCI_ALLOC_APPZ (sizeof (MMIFMM_VIEW_WIN_DATA_T));
        view_win_d->main_d_ptr = main_win_d;

        view_win_d->s_fmm_path_ptr = SCI_ALLOC_APPZ (sizeof (MMIFMM_PATH_INFO_T) * (MMI_DEVICE_NUM - 1));

        if (PNULL == view_win_d->s_fmm_path_ptr)
        {
            SCI_FREE (view_win_d);
            SCI_TRACE_LOW ("AiChatListViewWinOpenCtrlFunc HandleUdiskChildWindow PNULL == view_win_d->s_fmm_path_ptr");
            return MMI_RESULT_FALSE;
        }

        view_win_d->s_list_file_name_ptr = SCI_ALLOC_APPZ (sizeof (wchar) * (MMIFMM_FILENAME_LEN + 1));
        view_win_d->list_ctrl_id = MMIAI_CHAT_LIST_CTRL_ID;
        view_win_d->com_ctrl_id = 0;
        view_win_d->tab_ctrl_id = 0;
    } 

    MMK_SetWinUserData (win_id, (void *) view_win_d);

    return MMI_RESULT_TRUE;
}

LOCAL void StartRecordAnimTimer()
{
    if(ai_chat_record_anim_timer_id != 0)
    {
        MMK_StopTimer(ai_chat_record_anim_timer_id);
    }
    ai_chat_record_anim_timer_id =  MMK_CreateWinTimer(MMIAI_CHAT_WIN_ID, AICHAT_RECORD_ANIM_DELAY,TRUE);

}

LOCAL void StopRecordAnimTimer()
{
    if(ai_chat_record_anim_timer_id != 0)
    {
        MMK_StopTimer(ai_chat_record_anim_timer_id);
    }
    ai_chat_record_anim_timer_id = 0;

}

LOCAL void StartRecordMaxTimeTimer()
{
    if(max_record_time_timer_id != 0)
    {
        MMK_StopTimer(max_record_time_timer_id);
    }
    max_record_time_timer_id =  MMK_CreateWinTimer(MMIAI_CHAT_WIN_ID, AI_CHAT_MAX_RECORD_TIME_MS, FALSE);

}

LOCAL void StopRecordMaxTimeTimer()
{
    if(max_record_time_timer_id != 0)
    {
        MMK_StopTimer(max_record_time_timer_id);
    }
    max_record_time_timer_id = 0;

}

LOCAL void AI_ShowBottom(MMI_WIN_ID_T        win_id)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   record_text_rect       = AI_CHAT_RECORD_TEXT_RECT;
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    //uint16 tittle[10] = {0x957F, 0x6309, 0x5E95 ,0x90E8, 0x8BB2, 0x8BDD, 0}; //长按此处讲话957F 6309 6B64 5904 8BB2 8BDD 
    uint16 tittle[5] = {0x6309,0x4F4F,0x8BB2,0x8BDD, 0}; ///按住讲话    
    GUI_FONT_T f_big = SONG_FONT_26;//SONG_FONT_22;//SONG_FONT_20;//SONG_FONT_34;
    GUI_POINT_T      point = {0};
    GUI_RECT_T bottom_rect = AI_CHAT_BOTTOM_RECT;
    
    LCD_FillRect(&lcd_dev_info, bottom_rect, MMI_BLACK_COLOR);

    point.x = AI_CHAT_RECORD_IMG_X;
    point.y = AI_CHAT_RECORD_IMG_Y;

    GUIRES_DisplayImg(&point,PNULL,PNULL,win_id,IMAGE_AI_NO_RECORD_BG, &lcd_dev_info);

    
    text_style.align = ALIGN_LVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&record_text_rect,       
        (const GUI_RECT_T      *)&record_text_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    return;
}

LOCAL void DisplayAiChatRecordAnim(MMI_WIN_ID_T win_id)
{
    MMI_IMAGE_ID_T cur_img_id = 0;
    GUI_POINT_T      point = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   record_text_rect = AI_CHAT_RECORD_TEXT_RECT;
    GUI_RECT_T   rect        = AI_CHAT_BOTTOM_RECT; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle[10] = {0x6b63, 0x5728, 0x5f55, 0x97f3,0}; //正在录音
    GUI_FONT_T f_big = SONG_FONT_26;//SONG_FONT_22;//SONG_FONT_20;//SONG_FONT_34;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    point.x = AI_CHAT_RECORD_IMG_X;		
    point.y = AI_CHAT_RECORD_IMG_Y;

    GUIRES_DisplayImg(&point,PNULL,PNULL,win_id,IMAGE_AI_RECORDING_BG,&lcd_dev_info);
    
    text_style.align = ALIGN_LVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&record_text_rect,       
        (const GUI_RECT_T      *)&record_text_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );	
}


#ifdef WIN32

LOCAL void AddAiTestItems()
{
    uint8 utf8_str1[4] = { 0 };
    uint8 utf8_str2[4+3] = { 0 };
    uint8 utf8_str3[4+3+3] = { 0 };
    uint8 utf8_str4[4+3+3+3] = { 0 };
    uint8 utf8_str5[4+3+3+3+3] = { 0 };
    uint8 utf8_str6[4+3+3+3+3+3] = { 0 };
    uint8 utf8_str7[4+3+3+3+3+3+3] = { 0 };
    uint8 utf8_str8[4+3+3+3+3+3+3+3] = { 0 };
    uint8 utf8_str9[4+3+3+3+3+3+3+3+3] = { 0 };
    uint8 utf8_str10[4+3+3+3+3+3+3+3+3+3] = { 0 };
    uint8 utf8_str11[4+3+3+3+3+3+3+3+3+3+3] = { 0 };
    uint8 utf8_str12[4+3+3+3+3+3+3+3+3+3+3+3] = { 0 };
    uint8 utf8_str13[4+3+3+3+3+3+3+3+3+3+3+3+3] = { 0 };
    uint8 utf8_str14[4+3+3+3+3+3+3+3+3+3+3+3+3+3] = { 0 };
    uint8 utf8_str15[4+3+3+3+3+3+3+3+3+3+3+3+3+3+3] = { 0 };
    uint8 utf8_str16[4+3+3+3+3+3+3+3+3+3+3+3+3+3+3+3] = { 0 };
    uint8 utf8_str17[4+3+3+3+3+3+3+3+3+3+3+3+3+3+3+3+3] = { 0 };
    uint8 utf8_str18[4+3+3+3+3+3+3+3+3+3+3+3+3+3+3+3+3+3] = { 0 };
    uint8 utf8_str19[4+3+3+3+3+3+3+3+3+3+3+3+3+3+3+3+3+3+3] = { 0 };
    uint8 utf8_str20[4+3+3+3+3+3+3+3+3+3+3+3+3+3+3+3+3+3+3+3] = { 0 };
    uint8 *p = NULL;
    int i = 0;
    //int j = 0;
    int k = 0;
    k++; p = utf8_str1; for(i = 0; i < 3*k; i=i+3 ){ p[i+0] = 0xe5; p[i+1] = 0x93; p[i+2] = 0x88; }
    k++; p = utf8_str2; for(i = 0; i < 3*k; i=i+3 ){ p[i+0] = 0xe5; p[i+1] = 0x93; p[i+2] = 0x88; }
    k++; p = utf8_str3; for(i = 0; i < 3*k; i=i+3 ){ p[i+0] = 0xe5; p[i+1] = 0x93; p[i+2] = 0x88; }
    k++; p = utf8_str4; for(i = 0; i < 3*k; i=i+3 ){ p[i+0] = 0xe5; p[i+1] = 0x93; p[i+2] = 0x88; }
    k++; p = utf8_str5; for(i = 0; i < 3*k; i=i+3 ){ p[i+0] = 0xe5; p[i+1] = 0x93; p[i+2] = 0x88; }
    k++; p = utf8_str6; for(i = 0; i < 3*k; i=i+3 ){ p[i+0] = 0xe5; p[i+1] = 0x93; p[i+2] = 0x88; }
    k++; p = utf8_str7; for(i = 0; i < 3*k; i=i+3 ){ p[i+0] = 0xe5; p[i+1] = 0x93; p[i+2] = 0x88; }
    k++; p = utf8_str8; for(i = 0; i < 3*k; i=i+3 ){ p[i+0] = 0xe5; p[i+1] = 0x93; p[i+2] = 0x88; }
    k++; p = utf8_str9; for(i = 0; i < 3*k; i=i+3 ){ p[i+0] = 0xe5; p[i+1] = 0x93; p[i+2] = 0x88; }
    k++; p = utf8_str10; for(i = 0; i < 3*k; i=i+3 ){ p[i+0] = 0xe5; p[i+1] = 0x93; p[i+2] = 0x88; }
    k++; p = utf8_str11; for(i = 0; i < 3*k; i=i+3 ){ p[i+0] = 0xe5; p[i+1] = 0x93; p[i+2] = 0x88; }
    k++; p = utf8_str12; for(i = 0; i < 3*k; i=i+3 ){ p[i+0] = 0xe5; p[i+1] = 0x93; p[i+2] = 0x88; }
    k++; p = utf8_str13; for(i = 0; i < 3*k; i=i+3 ){ p[i+0] = 0xe5; p[i+1] = 0x93; p[i+2] = 0x88; }
    k++; p = utf8_str14; for(i = 0; i < 3*k; i=i+3 ){ p[i+0] = 0xe5; p[i+1] = 0x93; p[i+2] = 0x88; }
    k++; p = utf8_str15; for(i = 0; i < 3*k; i=i+3 ){ p[i+0] = 0xe5; p[i+1] = 0x93; p[i+2] = 0x88; }
    k++; p = utf8_str16; for(i = 0; i < 3*k; i=i+3 ){ p[i+0] = 0xe5; p[i+1] = 0x93; p[i+2] = 0x88; }
    k++; p = utf8_str17; for(i = 0; i < 3*k; i=i+3 ){ p[i+0] = 0xe5; p[i+1] = 0x93; p[i+2] = 0x88; }
    k++; p = utf8_str18; for(i = 0; i < 3*k; i=i+3 ){ p[i+0] = 0xe5; p[i+1] = 0x93; p[i+2] = 0x88; }
    k++; p = utf8_str19; for(i = 0; i < 3*k; i=i+3 ){ p[i+0] = 0xe5; p[i+1] = 0x93; p[i+2] = 0x88; }
    k++; p = utf8_str20; for(i = 0; i < 3*k; i=i+3 ){ p[i+0] = 0xe5; p[i+1] = 0x93; p[i+2] = 0x88; }
  
   
    
    #if 1
    AppendAIListContent( utf8_str1, TRUE);
    AppendAIListContent( utf8_str2, TRUE);
    AppendAIListContent( utf8_str3, TRUE);
    AppendAIListContent( utf8_str4, TRUE);
    AppendAIListContent( utf8_str5, TRUE);
    AppendAIListContent( utf8_str6, TRUE);
    AppendAIListContent( utf8_str7, TRUE);
    AppendAIListContent( utf8_str8, TRUE);
    AppendAIListContent( utf8_str9, TRUE);
    AppendAIListContent( utf8_str10, TRUE);
    AppendAIListContent( utf8_str11, TRUE);
    AppendAIListContent( utf8_str12, TRUE);
    AppendAIListContent( utf8_str13, TRUE);
    AppendAIListContent( utf8_str14, TRUE);
    AppendAIListContent( utf8_str15, TRUE);
    AppendAIListContent( utf8_str16, TRUE);
    AppendAIListContent( utf8_str17, TRUE);
    AppendAIListContent( utf8_str18, TRUE);
    AppendAIListContent( utf8_str19, TRUE);

    AppendAIListContent( utf8_str1, FALSE);
    AppendAIListContent( utf8_str2, FALSE);
    AppendAIListContent( utf8_str3, FALSE);
    AppendAIListContent( utf8_str4, FALSE);
    AppendAIListContent( utf8_str5, FALSE);
    AppendAIListContent( utf8_str6, FALSE);
    AppendAIListContent( utf8_str7, FALSE);
    AppendAIListContent( utf8_str8, FALSE);
    AppendAIListContent( utf8_str9, FALSE);
    AppendAIListContent( utf8_str10, FALSE);
    AppendAIListContent( utf8_str11, FALSE);
    AppendAIListContent( utf8_str12, FALSE);
    AppendAIListContent( utf8_str13, FALSE);
    AppendAIListContent( utf8_str14, FALSE);
    AppendAIListContent( utf8_str15, FALSE);
    AppendAIListContent( utf8_str16, FALSE);
    AppendAIListContent( utf8_str17, FALSE);
    AppendAIListContent( utf8_str18, FALSE);
    AppendAIListContent( utf8_str19, FALSE);
    #endif
    AppendAIListContent( utf8_str10, FALSE);
    AppendAIListContent( utf8_str14, FALSE);
    

}
#endif




LOCAL MMI_RESULT_E  Handle_AIChat_WinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    /*MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    uint8 * p_index = PNULL;
    AI_APP_T * pMe = &g_ai_app;

    p_index = (uint8 *) MMK_GetWinAddDataPtr(win_id);*/

    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    GUI_RECT_T      bg_rect = MMITHEME_GetFullScreenRectEx(win_id);
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_BG_T bg_ptr = {0};
    GUI_BG_DISPLAY_T bg_display = {0};
    const GUI_LCD_DEV_INFO *lcd_info = MMITHEME_GetDefaultLcdDev();
    GUI_POINT_T      point = {0};
    MMIRECORD_SRV_RESULT_E record_result = MMIRECORD_SRV_RESULT_SUCCESS;
    MMI_CTRL_ID_T           ctrl_id = MMIAI_CHAT_LIST_CTRL_ID;
    MMIFMM_VIEW_WIN_DATA_T* view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);
    GUILIST_INIT_DATA_T list_init = {0};    
    uint16                      cur_selection   =   0;
    BOOLEAN ret = FALSE;
    
    /*uint8 * p_index = PNULL;
    p_index = (uint8 *) MMK_GetWinAddDataPtr(win_id);*/

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //AiChat_BackLight(TRUE);//yangyu delete
        if (MMI_RESULT_FALSE == AiChatListViewWinOpenCtrlFunc (win_id))
        {
            MMK_CloseWin (win_id);
            break;
        }
        MMISRVAUD_ReqVirtualHandle("FIX CALL SELECT RING", MMISRVAUD_PRI_NORMAL);

        view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);     
        
        list_init.both_rect.v_rect.left = 0;
        list_init.both_rect.v_rect.right = MMI_MAINSCREEN_WIDTH;
        list_init.both_rect.v_rect.top = 0;
        list_init.both_rect.v_rect.bottom = MMI_MAINSCREEN_HEIGHT-AI_CHAT_RECORD_IMG_MARGIN_BOTTOM;//185;//240;//180;

        list_init.both_rect.h_rect.left = 0;
        list_init.both_rect.h_rect.right = MMI_MAINSCREEN_HEIGHT-AI_CHAT_RECORD_IMG_MARGIN_BOTTOM;//240;//180;
        list_init.both_rect.h_rect.top = 0;
        list_init.both_rect.h_rect.bottom = MMI_MAINSCREEN_WIDTH;
        
        list_init.type = GUILIST_TEXTLIST_E;
                    
        GUILIST_CreateListBox(win_id, 0, ctrl_id, &list_init);
        GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE );//不需要分割线
        GUILIST_SetListState( ctrl_id, GUILIST_STATE_TEXTSCROLL_ENABLE | GUILIST_STATE_AUTO_SCROLL, TRUE );//长文本滚动
        MMK_SetAtvCtrl(win_id,ctrl_id);

#if defined(ZDT_MMI_USE_SMALL_FONT)
        CTRLLIST_SetTextFont(ctrl_id, SONG_FONT_20, MMI_CYAN_COLOR);
#else
        CTRLLIST_SetTextFont(ctrl_id, SONG_FONT_28, MMI_CYAN_COLOR);
#endif
        ret = GUILIST_SetMaxItem(ctrl_id, AI_LIST_MAX_SIZE, FALSE);
        //CTRLLIST_SetAutoSwitchItem(ctrl_id , TRUE);
        //AiUpdateRecordListData (win_id, ctrl_id, MMIFMM_READ_TYPE_FILE_DIR, TXT_NULL, TRUE);
       
        MMIAI_Net_Init(); //init net sim 
        MMIAI_Net_Open(); //send msg ,to active pdp
        //AI_Net_Init(); // init url ip task, not here ,need after pdp active

        
        break;
        
    case MSG_FULL_PAINT:
        AI_ShowBottom(win_id);//new ui

        #ifdef WIN32
            AddAiTestItems();
        #endif
        break;
        
    
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        //AiChatHandleRecordListAppendData (win_id, param, ctrl_id, 2);
        break;
        
    case MSG_CTL_PENOK:
    case MSG_CTL_MIDSK:
        /*MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        MMIAPIFMM_PlayMusicFile(MMISET_RING_TYPE_OTHER, FALSE,
                    view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[cur_selection]->filename_ptr, view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[cur_selection]->filename_len, 
                    1, 0, MMIAPIENVSET_GetCurModeRingVol(MMISET_RING_TYPE_CALL), NULL);*/
        break;

    case MSG_KEYREPEAT_WEB:
    case MSG_KEYREPEAT_OK:  
        /*
        record_result = StartAiChatRecord();
        if(record_result ==MMIRECORD_SRV_RESULT_SUCCESS )
        {
            ai_record_update_anim_index = 0;
            ai_chat_record_anim_timer_id =  MMK_CreateWinTimer(win_id, AICHAT_RECORD_ANIM_DELAY,TRUE);
            DisplayAiChatRecordAnim(win_id);
            return record_result;   
        }*/        
        break;

    case MSG_KEYPRESSUP_WEB:
    case MSG_KEYPRESSUP_OK:
        /*
        MMI_StopAiChatRecord();    
        MMK_StopTimer(ai_chat_record_anim_timer_id);
        ClearAiChatRecordAnim(win_id);*/
        break;
    case MSG_TP_PRESS_DOWN:
    {
        GUI_POINT_T   point = {0};
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
        if(point.y > AI_LIST_BOTTON_POSITON &&  point.y < MMI_MAINSCREEN_HEIGHT )
        {
            /*StartRecordAnimTimer();
            MMI_StopAiChatRecord();
            record_result = StartAiChatRecord();
            if(record_result ==MMIRECORD_SRV_RESULT_SUCCESS )
            {
               
               
                DisplayAiChatRecordAnim(win_id);
                return record_result;   
            }*/
            /*
            StartRecordAnimTimer();
            DisplayAiChatRecordAnim(win_id);
            StartRecordMaxTimeTimer();
            AI_API_Record_Start();*/
            
        }
        ClearAiChatRecordAnim(win_id);
    }

        break;
   
    case MSG_TP_PRESS_LONG:    
    {
        
        GUI_POINT_T   point = {0};
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
        if(ZDT_SIM_Exsit() == FALSE)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,STR_SIM_NOT_SIM_EXT01,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            break;
        }
        if (MMIAPICONNECTION_isGPRSSwitchedOff())
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_YX_WCHAT_NEED_NET,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            break;
        }
#ifdef ZDT_PLAT_YX_SUPPORT
        /*if(yx_DB_Set_Rec.net_open == 0)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_YX_WCHAT_NEED_OPEN,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            break;
        }*/
#endif
        if(point.y > AI_LIST_BOTTON_POSITON &&  point.y < MMI_MAINSCREEN_HEIGHT )
        {
            /*record_result = StartAiChatRecord();
            if(record_result ==MMIRECORD_SRV_RESULT_SUCCESS )
            {
                ai_record_update_anim_index = 0;
                ai_chat_record_anim_timer_id =  MMK_CreateWinTimer(win_id, AICHAT_RECORD_ANIM_DELAY,TRUE);
                DisplayAiChatRecordAnim(win_id);
                return record_result;   
            }   */
            //CTRLLIST_StopTextAnim(ctrl_id);//stop auto switch marquee text
            StartRecordAnimTimer();
            DisplayAiChatRecordAnim(win_id);
            StartRecordMaxTimeTimer();
            #ifndef WIN32
            AI_API_Record_Start();
            #endif
        }
        
    }
    break;

    case MSG_TP_PRESS_UP:
    {
        
        GUI_POINT_T   point = {0};
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
  //      if(point.y > AI_LIST_BOTTON_POSITON &&  point.y < MMI_MAINSCREEN_HEIGHT )
  //      {
            /*MMI_StopAiChatRecord();
            
            StopRecordAnimTimer();
            ClearAiChatRecordAnim(win_id);
            AiUpdateRecordListData (win_id, ctrl_id, MMIFMM_READ_TYPE_FILE_DIR, TXT_NULL, TRUE);
            MMK_UpdateScreen();*/
            StopRecordAnimTimer();
            ClearAiChatRecordAnim(win_id);
            MMK_UpdateScreen();
            StopRecordMaxTimeTimer();
            #ifndef WIN32
            AI_API_Record_Stop();
            #endif
            //AI_TCPSendAIRecordFile(); // NOT really stop
     //   }   

    }        
    break;

	case MSG_TIMER:
		if (*(uint8*)param == ai_chat_record_anim_timer_id)
		{
		    if(ai_record_update_anim_index < AICHAT_RECORD_ANIMAL_TOTAL_FRAME_NUM)
		    {
		        ai_record_update_anim_index++;
		    }
            else
            {
                ai_record_update_anim_index = 0;
            }
		    DisplayAiChatRecordAnim(win_id);
		}
        else if (*(uint8*)param == max_record_time_timer_id)
        {
            StopRecordAnimTimer();
            ClearAiChatRecordAnim(win_id);
            //StopRecordMaxTimeTimer(); 
            AI_API_Record_Stop();
            //AI_TCPSendAIRecordFile();
        }
		else
		{
		    recode = MMI_RESULT_FALSE;
		}
		break;

	case  MSG_LOSE_FOCUS:
			 StopRecordAnimTimer();
			//             StopRecordAnimTimer();			//zhou   BUG 设备机器人发送中接收微聊/视频/电话/查找/充电等提醒时，等待几秒后机器人发送图标会上浮，界面重叠
            ClearAiChatRecordAnim(win_id);
            //MMK_UpdateScreen();
            StopRecordMaxTimeTimer();
            #ifndef WIN32
            AI_API_Record_Stop();
            #endif
            //AI_TCPSe
		break;
    case MSG_KEYDOWN_RED:
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_KEYUP_RED:
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        MMISRVAUD_FreeVirtualHandle("FIX CALL SELECT RING"); 
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        AiChat_BackLight(FALSE);
        StopRecordAnimTimer();
        AI_Tcp_ForceCloseAll();//close tcp socket and thread
        AI_API_Rcv_Mp3_PlayStop();//stop play, clear buf list
        /*if(p_index != PNULL)
        {
            SCI_FREE(p_index);
        }*/
        MMIAI_Net_Close();
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}



