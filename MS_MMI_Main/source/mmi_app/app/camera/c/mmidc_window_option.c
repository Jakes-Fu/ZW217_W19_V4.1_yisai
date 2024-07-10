/*****************************************************************************
** File Name:      mmidc_option_wintab.c                                     *
** Author:                                                                   *
** Date:           2008-5	                                                 *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe dc preview window table     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2006        ryan.xu        Create										 *
*****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "mmi_app_camera_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#ifdef  CAMERA_SUPPORT
#include "mmidisplay_data.h"
#include "window_parse.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiidle_subwintab.h"
#include "mmiphone_export.h"
#include "mmi_default.h"
#include "mmk_timer.h"
#include "mmi_appmsg.h"
#include "guicommon.h"
#include "guilcd.h"
#include "guiblock.h"
#include "mmidc_camera_id.h"
#include "mmipub.h"
#include "mmidc_gui.h"
#include "mmidc_flow.h"
#include "mmidc_camera_text.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "guilistbox.h"
#include "mmidc_display.h"
#include "mmidc_save.h"
#include "mmidc_main.h"
#include "mmidc_export.h"
#include "mmidc_option.h"
#include "mmidc_setting.h"
//#include "mmifmm_text.h"
#include "mmiudisk_export.h"
#include "guisetlist.h"
#include "cafcontrol.h"
#include "mmipic_edit_export.h"
#include "mmi_text.h"
#include "mmisd_export.h"
#include "mmi_menutable.h"

#ifdef MMIDC_F_U_CAMERA
#include "mmidc_imageprocess.h"
#endif

#ifdef MMIDC_F_WORK_MODE
#include "mmidc_workmode.h"
#endif

#ifdef MMIDC_ADD_SPECIAL_EFFECT
#include "guires.h"
#endif
/**--------------------------------------------------------------------------*
 ** 						MACRO DEFINITION								 *
 **--------------------------------------------------------------------------*/
#ifndef TIP_NULL
#define TIP_NULL 0
#endif

#define OPTION_ITEM_MAX 10

typedef struct
{
   int8 max_item_num;
   int8 cur_item;
   GUIITEM_STYLE_E item_style;
   uint32 user_data[OPTION_ITEM_MAX];
   MMI_TEXT_ID_T text_array[OPTION_ITEM_MAX];
   MMI_TEXT_ID_T text1_array[OPTION_ITEM_MAX];   
   MMI_IMAGE_ID_T image_array[OPTION_ITEM_MAX];
   BOOLEAN is_gray[OPTION_ITEM_MAX];
}MMIDC_OPTION_ITEM_T;


const GUIMENU_ITEM_T 	mmidc_photo_review_opt_menu[] =
{
#ifdef MMIDC_DESKTOP_PDA
    {ID_CAMERA_OPT_SEND, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_SEND, 0, 0, 0, MENU_NULL},
#endif
    {ID_CAMERA_OPT_DELETE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_DELETE, 0, 0, 0, MENU_NULL},
#ifndef FLASH_SUPPORT
    {ID_CAMERA_OPT_WALLPAPER, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_SETAS_WALLPAPER , 0, 0, 0, MENU_NULL},
#endif
#ifdef PIC_VIEWER_SUPPORT
    {ID_CAMERA_OPT_PHOTO_DIRECTORY, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_ENTERTAIMENT_PIC, 0, 0, 0, MENU_NULL},
#else
    {ID_CAMERA_OPT_PHOTO_DIRECTORY, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_DC_PIC, 0, 0, 0, MENU_NULL},
#endif
 #ifdef PIC_EDITOR_SUPPORT
    {ID_CAMERA_OPT_PIC_EDIT, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PICVIEW_EDIT, 0, 0, 0, MENU_NULL}
 #endif
}; 

#if defined(MMS_SUPPORT) || defined(BLUETOOTH_SUPPORT) || defined(SNS_SUPPORT)
const GUIMENU_ITEM_T	mmidc_send_opt[] =
{
#ifdef MMS_SUPPORT
    {ID_CAMERA_SEND_BY_MMS,		TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MMS,	0, 0, 0, MENU_NULL},
#endif
#if defined(BLUETOOTH_SUPPORT)       
    {ID_CAMERA_SEND_BY_BT,		TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_BLUETOOTH,	0, 0, 0, MENU_NULL},
#endif
#ifdef SNS_SUPPORT
#ifdef WRE_WEIBO_SUPPORT
    {ID_CAMERA_SHARE_TO_SINA,		TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_SHARE_TO_SINA,	0, 0, 0, MENU_NULL},
#endif
#ifdef WRE_FACEBOOK_SUPPORT
    {ID_CAMERA_SHARE_TO_FACEBOOK,		TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_SHARE_TO_FACEBOOK,	0, 0, 0, MENU_NULL},
#endif
#ifdef WRE_TWITTER_SUPPORT
    {ID_CAMERA_SHARE_TO_TIWTTER,		TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_SHARE_TO_TWITTER,	0, 0, 0, MENU_NULL}
#endif
#endif
};
#endif
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
_ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T mmidc_menu_win_table[] = 
{
    {NULL},
#include "mmidc_menu_wintable.def"
};

/*****************************************************************************/
// 	Description : Register DC menu group
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_RegMenuWin(void)
{
    MMI_RegMenuGroup(MMI_MODULE_CAMERA, mmidc_menu_win_table);
}

/*---------------------------------------------------------------------------*/
/*                          EXTERN FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

extern PUBLIC MMI_RESULT_E MMIDC_HandleDeletePubWinBG(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E	msg_id, DPARAM param);

/*---------------------------------------------------------------------------*/
/*                          LOCAL  FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : option win when in start status
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDcStorageOptWinMsg(
                                                     MMI_WIN_ID_T      win_id, 
                                                     MMI_MESSAGE_ID_E  msg_id, 
                                                     DPARAM            param
                                                     );
                                                     
#ifdef MMIDC_MINI_RESOURCE_SIZE
/*****************************************************************************/
//  Description : effect option win
//  Global resource dependence : none
//  Author: jinju.ma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEffectOptWinMsg(
                                                     MMI_WIN_ID_T      win_id, 
                                                     MMI_MESSAGE_ID_E  msg_id, 
                                                     DPARAM            param
                                                     ); 
#endif   
#ifdef MMIDC_ADD_SPECIAL_EFFECT
/*****************************************************************************/
// 	Description : handle contrast adjust win
//	Global resource dependence : none
//  Author: jinju.ma
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIDC_OpenContrastAdjustWinMsg(
												  MMI_WIN_ID_T      win_id, 
												  MMI_MESSAGE_ID_E  msg_id, 
												  DPARAM            param
												  );
/*****************************************************************************/
// 	Description : handle brightness adjust win
//	Global resource dependence : none
//  Author: jinju.ma
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIDC_OpenBrightnessAdjustWinMsg(
													MMI_WIN_ID_T      win_id, 
													MMI_MESSAGE_ID_E  msg_id, 
													DPARAM            param
													);
/*****************************************************************************/
// 	Description : get contrast/brightness child win ctrl rect
//	Global resource dependence : none
//  Author: jinju.ma
//	Note:
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T MMIDC_GetChildwinCtrlRect(
												MMI_WIN_ID_T      win_id,
												GUI_RECT_T  win_rect,	
												uint16      bottom,										   
												GUI_RECT_T  *rbtn_rect_ptr,
												GUI_RECT_T  *lbtn_rect_ptr			
												);
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//Description :  adjust higher
//Global resource dependence : none
//Author: jinju.ma
//Note:
/*****************************************************************************/
LOCAL void AdjustHighCallBack();
/*****************************************************************************/
//Description :  adjust lower
//Global resource dependence : none
//Author: jinju.ma
//Note:
/*****************************************************************************/
LOCAL void AdjustLowCallBack();
#endif
/*****************************************************************************/
// 	Description : open photo white balance option
//	Global resource dependence : none
//  Author: jinju.ma
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWhiteBalanceOptWinMsg(
											   MMI_WIN_ID_T      win_id, 
											   MMI_MESSAGE_ID_E  msg_id, 
											   DPARAM            param
											   );
#endif                                                                                                   
/*****************************************************************************/
// 	Description : handle photo review option window msg
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePhotoReviewOptionWinMsg(MMI_WIN_ID_T win_id, 
                                                            MMI_MESSAGE_ID_E msg_id, 
                                                            DPARAM param
                                                            );

/*****************************************************************************/
// 	Description : handle dc setting windows messages
//	Global resource dependence : 
//  Author: robert.wang
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDCSettingWinMsg(
                                          MMI_WIN_ID_T      win_id, 	
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          );


/*****************************************************************************/
//Description : get setting items
//Global resource dependence : 
//Author: robert.wang
//Note:   
/*****************************************************************************/
LOCAL uint16 GetSettingItems(MMIDC_OPTION_ITEM_T *option_item_ptr //out
                                          );

/*****************************************************************************/
//Description : get dc setting value items
//Global resource dependence : 
//Author: robert.wang
//Note:   
/*****************************************************************************/
LOCAL uint16 GetDCSettingValueItems(
                            uint32 setting_num, //in
                            MMI_TEXT_ID_T* win_text_id_ptr,//out
                            MMIDC_OPTION_ITEM_T *option_item_ptr //out
                                          );

/*****************************************************************************/
//Description : get dv setting value items
//Global resource dependence : 
//Author: robert.wang
//Note:   
/*****************************************************************************/
LOCAL uint16 GetDVSettingValueItems(
                            uint32 setting_num, //in
                            MMI_TEXT_ID_T* win_text_id_ptr,//out
                            MMIDC_OPTION_ITEM_T *option_item_ptr //out
                                          );

/**--------------------------------------------------------------------------*
 ** 						STATIC DEFINITION								 *
 **--------------------------------------------------------------------------*/

WINDOW_TABLE( MMIDC_STORAGE_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleDcStorageOptWinMsg),
    WIN_ID(MMIDC_STORAGE_OPT_WIN_ID),
    WIN_TITLE(TXT_PRIOR_STORE_MEDIUM),
    /* CREATE_POPMENU_CTRL(MENU_DC_STORAGE_OPTION,MMIDC_STORAGE_OPT_MENU_CTRL_ID),*/
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIDC_STORAGE_OPT_LISTBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#ifdef MMIDC_MINI_RESOURCE_SIZE
WINDOW_TABLE( MMIDC_EFFECT_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleEffectOptWinMsg),
    WIN_ID(MMIDC_EFFECT_OPT_WIN_ID),
    WIN_TITLE(TXT_DC_EFFECT),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIDC_EFFECT_OPT_LISTBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif
#ifdef MMIDC_ADD_SPECIAL_EFFECT
WINDOW_TABLE( MMIDC_CONTRAST_ADJUST_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)MMIDC_OpenContrastAdjustWinMsg),
		WIN_ID(MMIDC_CONTRAST_ADJUST_WIN_ID),
		WIN_TITLE(TXT_COMM_CONTRAST),
		CREATE_BUTTON_CTRL(IMAGE_COMMON_COUNT_RBTN_UN, MMIDC_CONTRAST_UP_BUTTON_CTRL_ID),
		CREATE_BUTTON_CTRL(IMAGE_COMMON_COUNT_LBTN_UN, MMIDC_CONTRAST_DOWN_BUTTON_CTRL_ID),
		WIN_STYLE(WS_TOOL_WIN),
		WIN_MOVE_STYLE(MOVE_NOT_FULL_SCREEN_WINDOW),
		END_WIN
};

WINDOW_TABLE( MMIDC_BRIGHTNESS_ADJUST_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)MMIDC_OpenBrightnessAdjustWinMsg),
		WIN_ID(MMIDC_BRIGHTNESS_ADJUST_WIN_ID),
		WIN_TITLE(TXT_COMMON_COMMON_BRIGHTNESS),
		CREATE_BUTTON_CTRL(IMAGE_COMMON_COUNT_RBTN_UN, MMIDC_BRIGHTNESS_UP_BUTTON_CTRL_ID),
		CREATE_BUTTON_CTRL(IMAGE_COMMON_COUNT_LBTN_UN, MMIDC_BRIGHTNESS_DOWN_BUTTON_CTRL_ID),
		WIN_STYLE(WS_TOOL_WIN),
		WIN_MOVE_STYLE(MOVE_NOT_FULL_SCREEN_WINDOW),
		END_WIN
};
WINDOW_TABLE( MMIDC_WHITE_BALANCE_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleWhiteBalanceOptWinMsg),
		WIN_ID(MMIDC_WHITE_BALANCE_OPT_WIN_ID),
		WIN_TITLE(TXT_DC_WHITE_BALANCE),
		CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIDC_WHITE_BALANCE_OPT_LISTBOX_CTRL_ID),
		WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
		END_WIN
};
#endif
WINDOW_TABLE(MMIDC_PHOTO_REVIEW_OPTION_WIN_TAB) = 
{
    //CLEAR_LCD,
        //WIN_PRIO(WIN_ONE_LEVEL),
        WIN_FUNC((uint32)HandlePhotoReviewOptionWinMsg),    
        WIN_ID(MMIDC_PHOTO_REVIEW_OPTION_WIN_ID),
        WIN_TITLE(STXT_OPTION),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_MENU_CTRL(MMIDC_PHOTO_REVIEW_OPT_MENU, MMIDC_PHOTO_REVIEW_OPTION_MENU_CTRL_ID),
        END_WIN
};

WINDOW_TABLE(MMIDC_SETTING_WIN_TAB) = 
{
    WIN_ID(MMIDC_SETTING_WIN_ID),
    WIN_FUNC((uint32) HandleDCSettingWinMsg),  
    WIN_TITLE(STXT_MAIN_SETTING),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#else
	WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIDC_SETTING_LIST_CTRL_ID),
    END_WIN
}; 

LOCAL uint32 s_current_setting_item = 0;
/*****************************************************************************/
// 	Description : handle photo review option window msg
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePhotoReviewOptionWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E            result      = MMI_RESULT_TRUE;
    MMI_MENU_ID_T	        menu_id     = 0;
    MMI_MENU_GROUP_ID_T     group_id    = 0;
    

#ifdef MMIDC_DESKTOP_PDA
    BOOLEAN isIstyle = MMITHEME_IsIstyle();
#endif    
    //SCI_TRACE_LOW:"[MMIDC]: HandlePhotoReviewOptionWinMsg(), msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_WINDOW_OPTION_290_112_2_18_2_9_58_772,(uint8*)"d", msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMIDC_DESKTOP_PDA
        if(!isIstyle)
        {
            GUIMENU_SetItemVisible(MMIDC_PHOTO_REVIEW_OPTION_MENU_CTRL_ID,MMIDC_PHOTO_REVIEW_OPT_MENU,ID_CAMERA_OPT_SEND,FALSE);
        }
#endif
        MMK_SetAtvCtrl(MMIDC_PHOTO_REVIEW_OPTION_WIN_ID, MMIDC_PHOTO_REVIEW_OPTION_MENU_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(MMIDC_PHOTO_REVIEW_OPTION_MENU_CTRL_ID, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));
        switch ( menu_id )
        {
        case ID_CAMERA_OPT_DELETE:
            MMIPUB_OpenQueryWinByTextIdEx(SPRD_CAMERA_APPLET_ID,\
                                          TXT_DELETE,\
                                          IMAGE_PUBWIN_QUERY,\
                                          PNULL,\
                                          MMIDC_HandleDeletePubWinBG,\
                                          PNULL \
                                          );
            break;
            
        case ID_CAMERA_OPT_SEND:
            MMIDC_OpenSendPhotoWin();
            
            break;
            
        case ID_CAMERA_OPT_WALLPAPER:
            MMIDC_SetCurrentPhotoAsWallpaper();
            
            break;         
        case ID_CAMERA_OPT_PHOTO_DIRECTORY:
            MMIDC_OpenPictureWin();
            MMK_CloseWin(win_id);
            break;
            
#ifdef PIC_EDITOR_SUPPORT
        case ID_CAMERA_OPT_PIC_EDIT:
            {
                wchar   *full_path_ptr = PNULL;
                uint16  full_path_len = 0;
                PHOTO_STOREG_INFO_T *photo_info_ptr = PNULL;
                
                photo_info_ptr = MMIDC_GetCurrentPhotoInfo();
                if (PNULL != photo_info_ptr)
                {
                    full_path_ptr = photo_info_ptr->file_info.file_name;
                    full_path_len = photo_info_ptr->file_info.file_name_len;
                }
                
                MMIAPIPIC_OpenPicEditorMainWin(full_path_ptr, full_path_len);
            }
            break;
#endif            
        default:
            break;
        }
        break;
        
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        case MSG_PROMPTWIN_OK:
            if(MMIDC_GetCameraMode() == CAMERA_MODE_DC)
            {
                MMIDC_DeletePhoto();
                MMIPUB_CloseQuerytWinEx(SPRD_CAMERA_APPLET_ID, PNULL);
                MMK_CloseWin(win_id);
            }
            break;
        case MSG_PROMPTWIN_CANCEL: 
            MMIPUB_CloseQuerytWinEx(SPRD_CAMERA_APPLET_ID, PNULL);
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return (result);
}

/*****************************************************************************/
// 	Description : open photo review option window
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenPhotoReviewOption()
{
    MMIDC_CreateWinByApplet((uint32*)MMIDC_PHOTO_REVIEW_OPTION_WIN_TAB, (ADD_DATA)PNULL);
}


/*****************************************************************************/
//  Description : dc storage option for setting storage device
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
/*
LOCAL MMI_RESULT_E HandleDcStorageOptWinMsg(
                                                     MMI_WIN_ID_T      win_id, 
                                                     MMI_MESSAGE_ID_E  msg_id, 
                                                     DPARAM            param
                                                     )
{
    MMI_RESULT_E            recode      = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T     group_id    = 0;
    MMI_MENU_ID_T           menu_id     = 0;
    MMI_CTRL_ID_T           ctrl_id     = MMIDC_STORAGE_OPT_LISTBOX_CTRL_ID;
     
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        {
            DC_DEV_E   file_dev = MMIDC_GetPhotoStorageDevice();
            BOOLEAN have_sd = TRUE;
            BOOLEAN have_udisk = TRUE;
            BOOLEAN udisk = FALSE;
            BOOLEAN sd = FALSE;
            GUIMENU_POP_SEL_INFO_T  pop_item_info = {0};
            
            MMIAPIUDISK_GetAllDeviceStatus(&udisk, &sd);
            //SCI_TRACE_LOW:"[MMIDC] HandleDcStorageOptWinMsg udisk=%d,sd=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_WINDOW_OPTION_420_112_2_18_2_9_58_773,(uint8*)"dd",udisk,sd);
            if(udisk && sd)
            {
                   if(MMIAPISD_IsSDPlugIn())
                   {
                        MMIAPICOM_EnableGrayed(win_id,MENU_DC_STORAGE_OPTION, ID_CAMERA_STORAGE_CARD, !have_sd);
                    }
                    MMIAPICOM_EnableGrayed(win_id,MENU_DC_STORAGE_OPTION, ID_CAMERA_UDISK, !have_udisk);
                    
                    pop_item_info.is_static = TRUE;
                    pop_item_info.ctrl_id = MMIDC_STORAGE_OPT_MENU_CTRL_ID;
                    pop_item_info.group_id = MENU_DC_STORAGE_OPTION;
                    if(DC_DEV_SD == file_dev)
                    {
                        pop_item_info.menu_id = ID_CAMERA_STORAGE_CARD;
                    }
                    else if(DC_DEV_UDISK == file_dev)
                    {
                        pop_item_info.menu_id = ID_CAMERA_UDISK;
                    }

                    GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);
            }
        }        
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        switch(menu_id)
        {          
        case ID_CAMERA_UDISK:                                
            MMIDC_SetPhotoStorageDevice(DC_DEV_UDISK);
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            MMK_CloseWin(win_id);
            break;
            
        case ID_CAMERA_STORAGE_CARD:
            MMIDC_SetPhotoStorageDevice(DC_DEV_SD);
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            MMK_CloseWin(win_id);
            break;            
        default:
            break;
        }
        break;
        
        case MSG_CTL_CANCEL:
        case MSG_LOSE_FOCUS:
            MMK_CloseWin(win_id);
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }    
    return recode;
}
*/

/*****************************************************************************/
//  Description : Get Storage Device List
//  Global resource dependence : none
//  Author: robert.wang
//  Note:  
/*****************************************************************************/
PUBLIC int8 GetStorageDeviceList(MMIFILE_DEVICE_E *dev_list, //out
                                MMI_TEXT_ID_T *text_id_list //out
                               )
{
    CAMERA_MODE_E  mode = 0;
    MMIFILE_DEVICE_E sd_type = 0;
    int8 count_dev_num = 0;

    //MMIFILE_DEVICE_E file_dev = 0;
    MMI_TEXT_ID_T text_id = TXT_NULL;

    mode = MMIDC_GetCameraMode();
    
    //find existed device
	for(sd_type = MMI_DEVICE_UDISK; sd_type < MMI_DEVICE_NUM; sd_type++)
	{
        if (PNULL != text_id_list)
        {
            text_id_list[sd_type] = TXT_NULL;
        }
        
        //if DV not support UDISK
#if defined (MMI_UDISK_MEM_ENABLE) && !defined (MMI_DV_UDISK_ENABLE)
        if ((CAMERA_MODE_DV == mode)
            && (MMI_DEVICE_UDISK == sd_type)
            )
        {
            continue;
        }
#endif
#ifndef CMCC_UI_STYLE
        if (MMIAPIFMM_GetDeviceTypeStatus(sd_type))
#endif
        {
            if (PNULL != dev_list)
            {
                //file_dev = MMIFILE_GetFileDeviceType(sd_type);
                dev_list[count_dev_num] = sd_type;
            }
            
            if (PNULL != text_id_list)
            {
                text_id = MMIAPIFMM_GetDeviceName(sd_type);
                text_id_list[count_dev_num] = text_id;
            }
            
            count_dev_num++;
        }       
    }

    //SCI_TRACE_LOW:"[MMIDC] GetStorageDeviceList count_dev_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_WINDOW_OPTION_535_112_2_18_2_9_59_774,(uint8*)"d", count_dev_num);
    return count_dev_num;
}
/*****************************************************************************/
//  Description : dc storage option for setting storage device
//  Global resource dependence : none
//  Author: robert.wang
//  Date: 09-9-22
//  Note:  this function is for DC and DV 
//        目前固定使用默认路径，不允许修改
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDcStorageOptWinMsg (
                                            MMI_WIN_ID_T        win_id,
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM          param
                                            )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMIDC_STORAGE_OPT_LISTBOX_CTRL_ID;
    
    MMI_TEXT_ID_T       str_id[MMI_DEVICE_NUM] = {0};
    MMIFILE_DEVICE_E    dev_list[MMI_DEVICE_NUM] = {0};
    int16               i = 0;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    MMIFILE_DEVICE_E   file_dev =  0; 
    uint16  max_items = 0;
    uint16   cur_item = 0;
    CAMERA_MODE_E   mode = CAMERA_MODE_DC;
    MMIFILE_DEVICE_E    sys_dev = 0;


    mode = MMIDC_GetCameraMode();
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id,ctrl_id);

        if(CAMERA_MODE_DV == mode)
        {
             file_dev = MMIDC_GetPrefVideoStorageDevice();
        }else{
            file_dev = MMIDC_GetPrefPhotoStorageDevice();
        }

        max_items = GetStorageDeviceList(dev_list, str_id);
        //SCI_TRACE_LOW:"[MMIDC] HandleDcStorageOptWinMsg max_items=%d,file_dev=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_WINDOW_OPTION_588_112_2_18_2_9_59_775,(uint8*)"dd",max_items,file_dev);
        if (0 == max_items)
        {
            MMIDC_ErrorTip(TXT_NO_SD_CARD_ALERT);
            MMK_CloseWin(win_id);
            break;
        }       
      
        GUILIST_SetMaxItem(ctrl_id,max_items, FALSE );

        //get list index
        for (i = 0; i < max_items; i++)
        {
            if (file_dev == dev_list[i])
            {
                cur_item = i;
                break;
            }
        }
        
        //if default device is not in device list,
        //then change storage device
        if (i == max_items)
        {
            sys_dev = dev_list[0];

            if(CAMERA_MODE_DV == mode)
            {
                MMIDC_SetPrefVideoStorageDevice(sys_dev);
            }else{
                MMIDC_SetPrefPhotoStorageDevice(sys_dev);
            }
        }

        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
        item_t.item_data_ptr = &item_data;
        
        item_data.softkey_id[0] = TXT_COMMON_OK;
        item_data.softkey_id[1] = COMMON_TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;

        for(i = 0; i < max_items; i++)
        {
            item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            
            item_data.item_content[0].item_data.text_id = str_id[i];
            
            if(!GUILIST_InsertItem(ctrl_id, &item_t, i))
            {               
                break;
            }
        }
        
        GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, cur_item);
        
        break;
    case MSG_FULL_PAINT:
        break;
        
    case MSG_GET_FOCUS:
        
        break;
        
    case MSG_LOSE_FOCUS:
        
        break;
        
    case MSG_TIMER:
        
        break;
        case MSG_CTL_MIDSK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        max_items = GetStorageDeviceList(dev_list, str_id);   
        //SCI_TRACE_LOW:"[MMIDC] HandleDcStorageOptWinMsg OK max_items=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_WINDOW_OPTION_672_112_2_18_2_9_59_776,(uint8*)"d", max_items);
        
        if (0 < max_items)
        {
            cur_item = GUILIST_GetCurItemIndex(ctrl_id);
            sys_dev = dev_list[cur_item];    
            
            //SCI_TRACE_LOW:"[MMIDC] HandleDcStorageOptWinMsg OK sys_dev=%d,mode=%d,cur_item=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_WINDOW_OPTION_680_112_2_18_2_9_59_777,(uint8*)"ddd",sys_dev, mode, cur_item);

            if(CAMERA_MODE_DV == mode)
            {
                MMIDC_SetPrefVideoStorageDevice(sys_dev);
            }else{
                MMIDC_SetPrefPhotoStorageDevice(sys_dev);
            }

            MMIDC_OpenAlertWin(MMIPUB_SOFTKEY_ONE,TXT_COMPLETE,IMAGE_PUBWIN_SUCCESS,PNULL);
        }
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        
        MMK_CloseWin(win_id);
        
        break;
        
    case MSG_APP_LEFT:
        
        break;
        
    case MSG_APP_RIGHT:
        
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_CLOSE_WINDOW:
        
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#ifdef MMIDC_MINI_RESOURCE_SIZE
/*****************************************************************************/
//  Description : effect option win
//  Global resource dependence : none
//  Author: jinju.ma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEffectOptWinMsg(
                                                     MMI_WIN_ID_T      win_id, 
                                                     MMI_MESSAGE_ID_E  msg_id, 
                                                     DPARAM            param
                                                     )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIDC_EFFECT_OPT_LISTBOX_CTRL_ID;

    uint16   cur_selection = 0;
	CAMERA_MODE_E   mode = CAMERA_MODE_DC;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:

		GUILIST_SetMaxItem( ctrl_id, 8, FALSE );
		MMIAPISET_AppendListItemByTextId(TXT_COMMON_COMMON_NORMAL, TXT_COMMON_OK, ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		MMIAPISET_AppendListItemByTextId(TXT_DC_MODE_BANDW, TXT_COMMON_OK ,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		MMIAPISET_AppendListItemByTextId(TXT_DC_MODE_BLUE, TXT_COMMON_OK ,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		MMIAPISET_AppendListItemByTextId(TXT_DC_MODE_GREEN, TXT_COMMON_OK ,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		MMIAPISET_AppendListItemByTextId(TXT_DC_MODE_YELLOW, TXT_COMMON_OK ,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		MMIAPISET_AppendListItemByTextId(TXT_DC_MODE_RED, TXT_COMMON_OK ,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		MMIAPISET_AppendListItemByTextId(TXT_DC_MODE_CANVAS, TXT_COMMON_OK ,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		MMIAPISET_AppendListItemByTextId(TXT_DV_MODE_NEGATIVE, TXT_COMMON_OK ,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
				
		mode = MMIDC_GetCameraMode();
		if(CAMERA_MODE_DC == mode)
		{
		cur_selection = MMIDC_GetPhotoEffect();          
		}
		else
		{
			cur_selection = MMIDC_GetVideoEffect();
		} 
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
    case MSG_FULL_PAINT:
        break;
        
    case MSG_GET_FOCUS:
        
        break;
        
    case MSG_LOSE_FOCUS:
        
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_CTL_PENOK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
		cur_selection = GUILIST_GetCurItemIndex( ctrl_id );		
		mode = MMIDC_GetCameraMode();
		if(CAMERA_MODE_DC == mode)
		{
			MMIDC_SetPhotoEffect((DC_EFFECT_E)cur_selection);
		}
		else
		{
			MMIDC_SetVideoEffect((DC_EFFECT_E)cur_selection);
		}
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        
        MMK_CloseWin(win_id);
        
        break;
        
    case MSG_CLOSE_WINDOW:
        
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}  
#endif
#ifdef MMIDC_ADD_SPECIAL_EFFECT
/*****************************************************************************/
// 	Description : handle contrast adjust win
//	Global resource dependence : none
//  Author: jinju.ma
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIDC_OpenContrastAdjustWinMsg(
                                                     MMI_WIN_ID_T      win_id, 
                                                     MMI_MESSAGE_ID_E  msg_id, 
                                                     DPARAM            param
                                                     )
{
	MMI_RESULT_E    result = MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO  lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	MMI_CTRL_ID_T   ctrl_id = MMIDC_CONTRAST_SET_SETLIST_CTRL_ID;

	GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect();
	GUI_RECT_T      win_rect = {0};	
	GUI_BOTH_RECT_T setlist_rect = {0};
	GUI_RECT_T title_rect = {0};
	GUI_RECT_T rbtn_rect = {0};
	GUI_RECT_T lbtn_rect = {0};
	
	uint16 text_height = 0;
	uint16   cur_selection = 0;
	
	CONTRAST_E i = 0;
	GUI_BG_T    bg = {0};
	CAMERA_MODE_E  mode = 0;
	MMI_STRING_T text_str = {0};
	GUI_FONT_ALL_T  font_info = {0};
	GUISETLIST_ITEM_INFO_T  item_info = {0};
	wchar str_data[] = {L'0',L'1',L'2', L'3',L'4',L'5',L'6',0};
    
	win_rect.left = (uint16)full_rect.left + MMIDC_CHILDWIN_HOR_SPACE - 1;
	win_rect.right = (uint16)full_rect.right - MMIDC_CHILDWIN_HOR_SPACE + 1;        
	win_rect.bottom = (uint16)full_rect.bottom - MMIDC_CHILDWIN_VER_SPACE + 1;
	win_rect.top = (uint16)win_rect.bottom - (3 * MMIDC_CHILDWIN_VER_SPACE) / 2 + 1;

	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:		
		MMK_SetWinRect(win_id, &win_rect);

		//set win title
		MMI_GetLabelTextByLang(TXT_COMM_CONTRAST, &text_str);           		
		text_height = MMIDC_GetStringHeight(&text_str);

		title_rect.left = win_rect.left;
		title_rect.right = win_rect.right;
		title_rect.top = win_rect.top + 1 + 1;
		title_rect.bottom = title_rect.top + text_height + MMIDC_CHILDWIN_ADJUST_SPACE;
		GUIWIN_SetTitleRect(win_id, title_rect);
		GUIWIN_SetTitleTextAlign(win_id, ALIGN_HVMIDDLE);
		GUIWIN_SetTitleFontColor(win_id, MMIDC_CHILDWIN_CTRL_BACKGROUND);
 		GUIWIN_SetTitleBackground(win_id, IMAGE_NULL, MMIDC_CHILDWIN_BACKGROUND);
		
		//create setlist ctrl
		bg.bg_type = GUI_BG_COLOR;
		bg.color = MMIDC_CHILDWIN_CTRL_BACKGROUND;
		setlist_rect = MMIDC_GetChildwinCtrlRect(win_id,win_rect,title_rect.bottom,&rbtn_rect,&lbtn_rect);
	 	GUISETLIST_CreateCtrlDyn(win_id, ctrl_id, &setlist_rect);
		GUISETLIST_SetTitleTextId(ctrl_id,TXT_COMM_CONTRAST);
        GUISETLIST_SetBackGround(ctrl_id, &bg);

		//add setlist ctrl content
		font_info.font = MMIDC_TEXT_FONT;
		font_info.color = MMIDC_CHILDWIN_BACKGROUND;
		GUISETLIST_SetFont(ctrl_id, &font_info);
		GUISETLIST_SetCtrlState(ctrl_id, GUISETLIST_STATE_ISCYC, FALSE);
		for(i = 0; i < CONTRAST_MAX; i++)
		{ 
			item_info.item_state = GUISETLIST_ITEM_STATE_TEXT;
			item_info.str_info.wstr_len = 1;
			item_info.str_info.wstr_ptr = &str_data[i];
			GUISETLIST_AddItem(ctrl_id, &item_info);
		}

		mode = MMIDC_GetCameraMode();
		if ( CAMERA_MODE_DC == mode)
		{
			cur_selection = MMIDC_GetContrast();
		}
		else
		{
			cur_selection = MMIDC_GetVideoContrast();
		}
		GUISETLIST_SetCurIndex(ctrl_id, cur_selection);

		//draw button ctrl
		GUIBUTTON_SetRect(MMIDC_CONTRAST_UP_BUTTON_CTRL_ID, &rbtn_rect);
		GUIBUTTON_SetRect(MMIDC_CONTRAST_DOWN_BUTTON_CTRL_ID, &lbtn_rect);
#ifdef TOUCH_PANEL_SUPPORT		
		GUIBUTTON_SetCallBackFunc(MMIDC_CONTRAST_UP_BUTTON_CTRL_ID, AdjustHighCallBack);
		GUIBUTTON_SetCallBackFunc(MMIDC_CONTRAST_DOWN_BUTTON_CTRL_ID, AdjustLowCallBack);
#endif		
		MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_FULL_PAINT:
	 	LCD_FillRect(&lcd_dev_info, win_rect, MMIDC_CHILDWIN_BACKGROUND);
        break;

    case MSG_GET_FOCUS:	
        cur_selection = GUISETLIST_GetCurIndex( ctrl_id );		
		mode = MMIDC_GetCameraMode();
		if ( CAMERA_MODE_DC == mode)
		{
			MMIDC_SetContrast(cur_selection);
		}
		else
		{
			MMIDC_SetVideoContrast(cur_selection);
		}

        break;
        
    case MSG_LOSE_FOCUS:
        
        break;

    case MSG_APP_WEB:
        
		break;
	
	case MSG_APP_UP:
    case MSG_APP_DOWN:
		break;

    case MSG_APP_LEFT:
	case MSG_APP_RIGHT:
	case MSG_NOTIFY_SETLIST_SWITCH:
		cur_selection = GUISETLIST_GetCurIndex( ctrl_id );
		mode = MMIDC_GetCameraMode();
		if ( CAMERA_MODE_DC == mode)
		{
			MMIDC_SetContrast(cur_selection);
		}
		else
		{
			MMIDC_SetVideoContrast(cur_selection);
		}

		break;

    case MSG_APP_OK:
	case MSG_APP_CANCEL:
	 	LCD_FillRect(&lcd_dev_info, win_rect, MMIDC_GetTransparentColor());    		
		MMK_CloseWin(win_id);
		MMIDC_ShowAllOSD();
        
        break;

    case MSG_CLOSE_WINDOW:
        
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
	return (result);
}

/*****************************************************************************/
// 	Description : handle brightness adjust win
//	Global resource dependence : none
//  Author: jinju.ma
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIDC_OpenBrightnessAdjustWinMsg(
                                                     MMI_WIN_ID_T      win_id, 
                                                     MMI_MESSAGE_ID_E  msg_id, 
                                                     DPARAM            param
                                                     )
{
	MMI_RESULT_E    result = MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO  lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	MMI_CTRL_ID_T   ctrl_id = MMIDC_BRIGHTNESS_SET_SETLIST_CTRL_ID;

	GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect();
	GUI_RECT_T      win_rect = {0};	
	GUI_BOTH_RECT_T setlist_rect = {0};
	GUI_RECT_T title_rect = {0};
	GUI_RECT_T rbtn_rect = {0};
	GUI_RECT_T lbtn_rect = {0};
	
	uint16 text_height = 0;
	uint16   cur_selection = 0;
	
	BRIGHTNESS_E i = 0;
	GUI_BG_T    bg = {0};
	CAMERA_MODE_E  mode = 0;
	MMI_STRING_T text_str = {0};
	GUI_FONT_ALL_T  font_info = {0};
	GUISETLIST_ITEM_INFO_T  item_info = {0};
	wchar str_data[] = {L'0',L'1',L'2', L'3',L'4',L'5',L'6',0};
    
	win_rect.left = (uint16)full_rect.left + MMIDC_CHILDWIN_HOR_SPACE - 1;
	win_rect.right = (uint16)full_rect.right - MMIDC_CHILDWIN_HOR_SPACE + 1;        
	win_rect.bottom = (uint16)full_rect.bottom - MMIDC_CHILDWIN_VER_SPACE + 1;
	win_rect.top = (uint16)win_rect.bottom - (3 * MMIDC_CHILDWIN_VER_SPACE) / 2 + 1;

	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:		
		MMK_SetWinRect(win_id, &win_rect);

		//set win title
		MMI_GetLabelTextByLang(TXT_COMMON_COMMON_BRIGHTNESS, &text_str);           		
		text_height = MMIDC_GetStringHeight(&text_str);

		title_rect.left = win_rect.left;
		title_rect.right = win_rect.right;
		title_rect.top = win_rect.top + 1 + 1;
		title_rect.bottom = title_rect.top + text_height + MMIDC_CHILDWIN_ADJUST_SPACE;
		GUIWIN_SetTitleRect(win_id, title_rect);
		GUIWIN_SetTitleTextAlign(win_id, ALIGN_HVMIDDLE);
		GUIWIN_SetTitleFontColor(win_id, MMIDC_CHILDWIN_CTRL_BACKGROUND);
 		GUIWIN_SetTitleBackground(win_id, IMAGE_NULL, MMIDC_CHILDWIN_BACKGROUND);
		
		//create setlist ctrl
		bg.bg_type = GUI_BG_COLOR;
		bg.color = MMIDC_CHILDWIN_CTRL_BACKGROUND;
		setlist_rect = MMIDC_GetChildwinCtrlRect(win_id,win_rect,title_rect.bottom,&rbtn_rect,&lbtn_rect);
	 	GUISETLIST_CreateCtrlDyn(win_id, ctrl_id, &setlist_rect);
		GUISETLIST_SetTitleTextId(ctrl_id,TXT_COMMON_COMMON_BRIGHTNESS);
        GUISETLIST_SetBackGround(ctrl_id, &bg);

		//add setlist ctrl content
		font_info.font = MMIDC_TEXT_FONT;
		font_info.color = MMIDC_CHILDWIN_BACKGROUND;
		GUISETLIST_SetFont(ctrl_id, &font_info);
		GUISETLIST_SetCtrlState(ctrl_id, GUISETLIST_STATE_ISCYC, FALSE);
		for(i = 0; i < BRIGHTNESS_LEVEL_MAX; i++)
		{ 
			item_info.item_state = GUISETLIST_ITEM_STATE_TEXT;
			item_info.str_info.wstr_len = 1;
			item_info.str_info.wstr_ptr = &str_data[i];
			GUISETLIST_AddItem(ctrl_id, &item_info);
		}

		mode = MMIDC_GetCameraMode();
		if ( CAMERA_MODE_DC == mode)
		{
			cur_selection = MMIDC_GetPhotoBrightness();
		}
		else
		{
			cur_selection = MMIDC_GetVideoBrightness();
		}
		GUISETLIST_SetCurIndex(ctrl_id, cur_selection);

		//draw button ctrl
		GUIBUTTON_SetRect(MMIDC_BRIGHTNESS_UP_BUTTON_CTRL_ID, &rbtn_rect);
		GUIBUTTON_SetRect(MMIDC_BRIGHTNESS_DOWN_BUTTON_CTRL_ID, &lbtn_rect);
#ifdef TOUCH_PANEL_SUPPORT		
		GUIBUTTON_SetCallBackFunc(MMIDC_BRIGHTNESS_UP_BUTTON_CTRL_ID, AdjustHighCallBack);
		GUIBUTTON_SetCallBackFunc(MMIDC_BRIGHTNESS_DOWN_BUTTON_CTRL_ID, AdjustLowCallBack);
#endif		
		MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
        MMK_SetAtvCtrl(win_id,ctrl_id);

        break;

    case MSG_FULL_PAINT:
	 	LCD_FillRect(&lcd_dev_info, win_rect, MMIDC_CHILDWIN_BACKGROUND);
        break;
			
    case MSG_GET_FOCUS:	
        cur_selection = GUISETLIST_GetCurIndex( ctrl_id );		
		mode = MMIDC_GetCameraMode();
		if ( CAMERA_MODE_DC == mode)
		{
			MMIDC_SetPhotoBrightness(cur_selection);
		}
		else
		{
			MMIDC_SetVideoBrightness(cur_selection);
		}

        break;
        
    case MSG_LOSE_FOCUS:
        
        break;

    case MSG_APP_WEB:
        break;
	
	case MSG_APP_UP:
    case MSG_APP_DOWN:
		break;

    case MSG_APP_LEFT:
	case MSG_APP_RIGHT:
	case MSG_NOTIFY_SETLIST_SWITCH:
		cur_selection = GUISETLIST_GetCurIndex( ctrl_id );
		mode = MMIDC_GetCameraMode();
		if ( CAMERA_MODE_DC == mode)
		{
			MMIDC_SetPhotoBrightness(cur_selection);
		}
		else
		{
			MMIDC_SetVideoBrightness(cur_selection);
		}
		break;

    case MSG_APP_OK:
	case MSG_APP_CANCEL:
	 	LCD_FillRect(&lcd_dev_info, win_rect, MMIDC_GetTransparentColor());    
		
		MMK_CloseWin(win_id);
		MMIDC_ShowAllOSD();
        
        break;

    case MSG_CLOSE_WINDOW:
        
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
	return (result);
}
/*****************************************************************************/
// 	Description : get contrast/brightness child win ctrl rect
//	Global resource dependence : none
//  Author: jinju.ma
//	Note:
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T MMIDC_GetChildwinCtrlRect(
									   MMI_WIN_ID_T      win_id,	//in
									   GUI_RECT_T  win_rect,		//in
									   uint16      bottom,			//in									   
									   GUI_RECT_T  *rbtn_rect_ptr,		//out
									   GUI_RECT_T  *lbtn_rect_ptr		//out			
									   )
{
	GUI_BOTH_RECT_T setlist_rect = {0};
	GUI_RECT_T title_rect = {0};
	GUI_RECT_T  rbtn_rect = {0};
	GUI_RECT_T  lbtn_rect = {0};	
	
	uint16 ctrl_height = 0;
	uint16 hor_space = MMIDC_CHILDWIN_HOR_SPACE;
    uint16 ver_space = MMIDC_CHILDWIN_VER_SPACE;

	uint16 icon_width = 0;
	uint16 icon_height = 0;

	title_rect.bottom = bottom;
	GUIRES_GetImgWidthHeight(&icon_width,&icon_height,IMAGE_COMMON_COUNT_RBTN_UN,win_id);
	if(PNULL != rbtn_rect_ptr && PNULL != lbtn_rect_ptr)
	{
		if (MMITHEME_IsMainScreenLandscape())
		{
			setlist_rect.h_rect.left = win_rect.left + hor_space; 
			setlist_rect.h_rect.top = title_rect.bottom + ver_space / 4 + 1;
			setlist_rect.h_rect.right = win_rect.right - hor_space;
			setlist_rect.h_rect.bottom = win_rect.bottom - ver_space / 4 + 1;

			ctrl_height = setlist_rect.h_rect.bottom - setlist_rect.h_rect.top + 1;

			rbtn_rect.left = setlist_rect.h_rect.right + 1 + 1;
			rbtn_rect.right = rbtn_rect.left + icon_width - 1;

			lbtn_rect.right = setlist_rect.h_rect.left - 1 - 1;
			lbtn_rect.left = lbtn_rect.right - icon_width + 1;

			if(ctrl_height < icon_height)
			{
				rbtn_rect.top = lbtn_rect.top = setlist_rect.h_rect.top - (icon_height - ctrl_height) / 2;
				rbtn_rect.bottom = lbtn_rect.bottom = rbtn_rect.top + icon_height - 1;
			}
			else
			{
				rbtn_rect.top = lbtn_rect.top = setlist_rect.h_rect.top + (ctrl_height - icon_height) / 2;
				rbtn_rect.bottom = lbtn_rect.bottom = rbtn_rect.top + icon_height - 1;
			}
		}
		else
		{
			setlist_rect.v_rect.left = win_rect.left + hor_space; 
			setlist_rect.v_rect.top = title_rect.bottom + ver_space / 4 + 1;
			setlist_rect.v_rect.right = win_rect.right - hor_space;
			setlist_rect.v_rect.bottom = win_rect.bottom - ver_space / 4 + 1;

			ctrl_height = setlist_rect.v_rect.bottom - setlist_rect.v_rect.top + 1;

			rbtn_rect.left = setlist_rect.v_rect.right + 1 + 1;
			rbtn_rect.right = rbtn_rect.left + icon_width - 1;

			lbtn_rect.right = setlist_rect.v_rect.left - 1 - 1;
			lbtn_rect.left = lbtn_rect.right - icon_width + 1;

			if(ctrl_height < icon_height)
			{
				rbtn_rect.top = lbtn_rect.top = setlist_rect.v_rect.top - (icon_height - ctrl_height) / 2;
				rbtn_rect.bottom = lbtn_rect.bottom = rbtn_rect.top + icon_height - 1;
			}
			else
			{
				rbtn_rect.top = lbtn_rect.top = setlist_rect.v_rect.top + (ctrl_height - icon_height) / 2;
				rbtn_rect.bottom = lbtn_rect.bottom = rbtn_rect.top + icon_height - 1;
			}
		}

		*rbtn_rect_ptr = rbtn_rect;
		*lbtn_rect_ptr = lbtn_rect;
	}

	return setlist_rect;
}
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//Description :  adjust higher
//Global resource dependence : none
//Author: jinju.ma
//Note:
/*****************************************************************************/
LOCAL void AdjustHighCallBack()
{
	CAMERA_MODE_E  mode = MMIDC_GetCameraMode();
	uint16 cur_selection = 0;
	
	if(MMK_IsOpenWin(MMIDC_CONTRAST_ADJUST_WIN_ID))
    {		
		cur_selection = GUISETLIST_GetCurIndex( MMIDC_CONTRAST_SET_SETLIST_CTRL_ID );       
		if (cur_selection < CONTRAST_6)
		{
			cur_selection = cur_selection + 1;
		}
		else
		{
			cur_selection = CONTRAST_6;
		}

		if ( CAMERA_MODE_DC == mode)
		{
			MMIDC_SetContrast((CONTRAST_E)cur_selection);
		}
		else
		{
			MMIDC_SetVideoContrast((CONTRAST_E)cur_selection);
		}
		GUISETLIST_SetCurIndex(MMIDC_CONTRAST_SET_SETLIST_CTRL_ID, cur_selection);
		MMK_SendMsg(MMIDC_CONTRAST_ADJUST_WIN_ID,MSG_FULL_PAINT,PNULL);
	}
	else if(MMK_IsOpenWin(MMIDC_BRIGHTNESS_ADJUST_WIN_ID))
	{
		cur_selection = GUISETLIST_GetCurIndex( MMIDC_BRIGHTNESS_SET_SETLIST_CTRL_ID ); 
		if (cur_selection < BRIGHTNESS_LEVEL_6)
		{
			cur_selection = cur_selection + 1;
		}
		else
		{
			cur_selection = BRIGHTNESS_LEVEL_6;
		}

		if ( CAMERA_MODE_DC == mode)
		{
			MMIDC_SetPhotoBrightness((BRIGHTNESS_E)cur_selection);
		}
		else
		{
			MMIDC_SetVideoBrightness((BRIGHTNESS_E)cur_selection);
		}
		GUISETLIST_SetCurIndex(MMIDC_BRIGHTNESS_SET_SETLIST_CTRL_ID, cur_selection);
		MMK_SendMsg(MMIDC_BRIGHTNESS_ADJUST_WIN_ID,MSG_FULL_PAINT,PNULL);
	}
}

/*****************************************************************************/
//Description :  adjust lower
//Global resource dependence : none
//Author: jinju.ma
//Note:
/*****************************************************************************/
LOCAL void AdjustLowCallBack()
{
	CAMERA_MODE_E  mode = MMIDC_GetCameraMode();
	uint16 cur_selection = 0;
	
	if(MMK_IsOpenWin(MMIDC_CONTRAST_ADJUST_WIN_ID))
    {		
		cur_selection = GUISETLIST_GetCurIndex( MMIDC_CONTRAST_SET_SETLIST_CTRL_ID );       
		if (cur_selection > CONTRAST_0)
		{
			cur_selection = cur_selection - 1;
		}
		else
		{
			cur_selection = CONTRAST_0;
		}

		if ( CAMERA_MODE_DC == mode)
		{
			MMIDC_SetContrast((CONTRAST_E)cur_selection);
		}
		else
		{
			MMIDC_SetVideoContrast((CONTRAST_E)cur_selection);
		}
		GUISETLIST_SetCurIndex(MMIDC_CONTRAST_SET_SETLIST_CTRL_ID, cur_selection);
		MMK_SendMsg(MMIDC_CONTRAST_ADJUST_WIN_ID,MSG_FULL_PAINT,PNULL);
	}
	else if(MMK_IsOpenWin(MMIDC_BRIGHTNESS_ADJUST_WIN_ID))
	{
		cur_selection = GUISETLIST_GetCurIndex( MMIDC_BRIGHTNESS_SET_SETLIST_CTRL_ID ); 
		if (cur_selection > BRIGHTNESS_LEVEL_0)
		{
			cur_selection = cur_selection - 1;
		}
		else
		{
			cur_selection = BRIGHTNESS_LEVEL_0;
		}

		if ( CAMERA_MODE_DC == mode)
		{
			MMIDC_SetPhotoBrightness((BRIGHTNESS_E)cur_selection);
		}
		else
		{
			MMIDC_SetVideoBrightness((BRIGHTNESS_E)cur_selection);
		}
		GUISETLIST_SetCurIndex(MMIDC_BRIGHTNESS_SET_SETLIST_CTRL_ID, cur_selection);
		MMK_SendMsg(MMIDC_BRIGHTNESS_ADJUST_WIN_ID,MSG_FULL_PAINT,PNULL);
	}
}
#endif
/*****************************************************************************/
// 	Description : open photo white balance option
//	Global resource dependence : none
//  Author: jinju.ma
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWhiteBalanceOptWinMsg(
                                                     MMI_WIN_ID_T      win_id, 
                                                     MMI_MESSAGE_ID_E  msg_id, 
                                                     DPARAM            param
                                                     )
{
    
	MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIDC_WHITE_BALANCE_OPT_LISTBOX_CTRL_ID;
    
    uint16   cur_selection = 0;  

	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:

		GUILIST_SetMaxItem( ctrl_id, 5, FALSE );
		MMIAPISET_AppendListItemByTextId(TXT_AUTO, TXT_COMMON_OK, ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		MMIAPISET_AppendListItemByTextId(TXT_DC_INCANDESCENCE, TXT_COMMON_OK, ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		MMIAPISET_AppendListItemByTextId(TXT_DC_FLUORESCENT, TXT_COMMON_OK ,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		MMIAPISET_AppendListItemByTextId(TXT_DC_SUN, TXT_COMMON_OK ,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		MMIAPISET_AppendListItemByTextId(TXT_DC_CLOUDY, TXT_COMMON_OK ,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
				
		switch(MMIDC_GetPhotoWhiteBalance())
		{
		case WHITE_BALANCE_AUTO:
			cur_selection = 0;
			break;
			
		case WHITE_BALANCE_INCANDESCENCE:
			cur_selection = 1;
			break;
			
		case WHITE_BALANCE_FLUORESCENT:
			cur_selection = 2;
			break;
			
		case WHITE_BALANCE_SUN:
			cur_selection = 3;
			break;
			
		case WHITE_BALANCE_CLOUDY:
			cur_selection = 4;
			break;
			
		default:
			cur_selection = 0;
			break;
		}	
		
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_FULL_PAINT:
        break;
			
    case MSG_GET_FOCUS:	
        
        break;
        
    case MSG_LOSE_FOCUS:
        
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_CTL_PENOK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
		{	
			cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
			switch(cur_selection)
			{
			case 0:
				MMIDC_SetPhotoWhiteBalance(WHITE_BALANCE_AUTO);
				break;
				
			case 1:
				MMIDC_SetPhotoWhiteBalance(WHITE_BALANCE_INCANDESCENCE);
				break;
				
			case 2:
				MMIDC_SetPhotoWhiteBalance(WHITE_BALANCE_FLUORESCENT);
				break;
				
			case 3:
				MMIDC_SetPhotoWhiteBalance(WHITE_BALANCE_SUN);
				break;
				
			case 4:
				MMIDC_SetPhotoWhiteBalance(WHITE_BALANCE_CLOUDY);
				break;
				
			default:
				MMIDC_SetPhotoWhiteBalance(WHITE_BALANCE_AUTO);
				break;
			}	
			MMK_CloseWin(win_id);
		}
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        
        MMK_CloseWin(win_id);
        
        break;
        
    case MSG_CLOSE_WINDOW:
        
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
	return (result);

}
#endif
/*****************************************************************************/
//  Description : open dc storage option menu
//  Global resource dependence : none
//  Author: robert.wang
//  Date: 09-9-22
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenStorageOption(void)
{
    MMIDC_CreateWinByApplet((uint32*)MMIDC_STORAGE_OPT_WIN_TAB, (ADD_DATA)PNULL);
}
#ifdef MMIDC_MINI_RESOURCE_SIZE
/*****************************************************************************/
//  Description : open effect option menu
//  Global resource dependence : none
//  Author: jinju.ma
//  Date: 2012-4-27
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenEffectOption(void)
{
    MMIDC_CreateWinByApplet((uint32*)MMIDC_EFFECT_OPT_WIN_TAB, (ADD_DATA)PNULL);
}
#endif
#ifdef MMIDC_ADD_SPECIAL_EFFECT
/*****************************************************************************/
//  Description : open contrast adjust win
//  Global resource dependence : none
//  Author: jinju.ma
//  Date: 2012-7-23
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenContrastAdjustWin(void)
{
	MMIDC_GetMenuHandle()->MenuHandleKeyDown(CANCEL_KEY);
	MMIDC_StopTextScrollTimer();
	
	MMK_CreateChildWin(MMIDC_MAIN_WIN_ID, (uint32*)MMIDC_CONTRAST_ADJUST_WIN_TAB, NULL);
}
/*****************************************************************************/
//  Description : open brightness adjust win
//  Global resource dependence : none
//  Author: jinju.ma
//  Date: 2012-7-23
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenBrightnessAdjustWin(void)
{
	MMIDC_GetMenuHandle()->MenuHandleKeyDown(CANCEL_KEY);
	MMIDC_StopTextScrollTimer();
	
	MMK_CreateChildWin(MMIDC_MAIN_WIN_ID, (uint32*)MMIDC_BRIGHTNESS_ADJUST_WIN_TAB, NULL);
}
/*****************************************************************************/
//  Description : open white balance option menu
//  Global resource dependence : none
//  Author: jinju.ma
//  Date: 2012-7-23
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenWhiteBalanceOption(void)
{
    MMIDC_CreateWinByApplet((uint32*)MMIDC_WHITE_BALANCE_OPT_WIN_TAB, (ADD_DATA)PNULL);
}
#endif
/*****************************************************************************/
//  Description : get setting items txt content
//  Global resource dependence : none
//  Author: chunyou
//  Date: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_GetCurModeSettingOption(MMIDC_SETTINGS_SET_E *option_ptr,//in
                                                        uint16 size_in, //in
                                                        uint16 *size_out_ptr //out
                                                        )
{
    uint16 mini_size = 0;
    CAMERA_MODE_E mode = 0;
    MMIDC_SETTINGS_SET_E option_array[MMIDC_SETTINGS_SET_MAX_VALUE] = {0};
    uint16 option_array_size = 0;
    uint16 total_bytes = 0;

    if(PNULL == option_ptr || PNULL == size_out_ptr)
    {
        return;
    }
    
    mode = MMIDC_GetCameraMode();
#if defined(MMIDC_DESKTOP_PDA)
    if(CAMERA_MODE_DC == mode)
    {
#if defined (MMIDC_F_U_CAMERA)
        if(MMIDC_GetUCameraModuleFlag())
        {
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_DC_FLASH;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_SELF_SHOT;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_DC_CORUSCATE;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_SHUTTERVOICE;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_AUTO_SAVE;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_DC_STORAGE;
        }
        else
#endif
        {
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_DC_FLASH;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_QUALITY;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_PHOTO_SIZE;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_PHOTO_DATE;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_ENVIRONMENT;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_PIC_MODE;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_SELF_SHOT;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_DC_CORUSCATE;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_SHUTTERVOICE;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_AUTO_SAVE;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_DC_STORAGE;
        }
    }
    else if(CAMERA_MODE_DV == mode)
    {
        option_array[option_array_size++] = MMIDC_SETTINGS_SET_DV_FLASH;
        option_array[option_array_size++] = MMIDC_SETTINGS_SET_VIDEO_SIZE;
        option_array[option_array_size++] = MMIDC_SETTINGS_SET_DV_CORUSCATE;
        option_array[option_array_size++] = MMIDC_SETTINGS_SET_AUDIO;
        option_array[option_array_size++] = MMIDC_SETTINGS_SET_VIDEOFORMAT;
        option_array[option_array_size++] = MMIDC_SETTINGS_SET_DV_STORAGE;
    }
#else
    if(CAMERA_MODE_DC == mode)
    {
#if defined (MMIDC_F_U_CAMERA)
        if(MMIDC_GetUCameraModuleFlag())
        {
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_DC_FLASH;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_DC_CORUSCATE;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_SHUTTERVOICE;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_AUTO_SAVE;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_DC_STORAGE;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_DC_DISPLAY_TIP;
        }
        else
#endif
        {
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_DC_FLASH;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_PHOTO_SIZE;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_QUALITY;            
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_DC_CORUSCATE;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_SHUTTERVOICE;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_AUTO_SAVE;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_DC_STORAGE;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_PHOTO_DATE;
            option_array[option_array_size++] = MMIDC_SETTINGS_SET_DC_DISPLAY_TIP;
#ifdef MMIDC_ADD_SPECIAL_EFFECT			
			option_array[option_array_size++] = MMIDC_SETTINGS_SET_PIC_MODE;
#endif        
		}
    }
    else if(CAMERA_MODE_DV == mode)
    {
        option_array[option_array_size++] = MMIDC_SETTINGS_SET_DV_FLASH;
        option_array[option_array_size++] = MMIDC_SETTINGS_SET_VIDEO_SIZE;
        option_array[option_array_size++] = MMIDC_SETTINGS_SET_DV_CORUSCATE;
        option_array[option_array_size++] = MMIDC_SETTINGS_SET_AUDIO;
        option_array[option_array_size++] = MMIDC_SETTINGS_SET_VIDEOFORMAT;
        option_array[option_array_size++] = MMIDC_SETTINGS_SET_DV_STORAGE;
        option_array[option_array_size++] = MMIDC_SETTINGS_SET_DV_DISPLAY_TIP;
    }
#endif

    mini_size = MIN(size_in, option_array_size);
    total_bytes = (uint16)mini_size * sizeof(option_array[0]);
    MMI_MEMCPY(option_ptr,
            total_bytes,
            &option_array, 
            total_bytes,
            total_bytes);
        
    *size_out_ptr = option_array_size;

    SCI_TRACE_LOW("[MMIDC] MMIDC_GetCurModeSettingOption option_array_size=%d", option_array_size);
}

/*****************************************************************************/
//  Description : get setting items txt option
//  Global resource dependence : none
//  Author: chunyou
//  Date: 
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetSettingItemsTxtOption(uint16 type,//in
                                                        uint16 text_total_num, //in
                                                        MMI_TEXT_ID_T *text_ptr, //out                                                        
                                                        int8 *max_item_num, //out
                                                        MMI_TEXT_ID_T* win_text_id_ptr //out
                                                        )
{

    MMIDC_SETTING_ITEM_T cur_item = 
        {
            TXT_NULL,
                {TXT_NULL,
                TXT_NULL,
                TXT_NULL
                }
        };
    uint32 cur_item_index = 0;
    uint32 i = 0;
    int8 count = 0;

    cur_item_index = MMIDC_GetSettingItemsTxtContent(type, &cur_item);

    if(PNULL != text_ptr
       && text_total_num >= MAX_SETTING_VAL_SET_NUMBERT)
    {
        MMI_MEMCPY(text_ptr,
            sizeof(cur_item.val_set),
            cur_item.val_set,
            sizeof(cur_item.val_set),
            sizeof(cur_item.val_set));
    }

    if(PNULL != win_text_id_ptr)
    {
        *win_text_id_ptr = cur_item.label;
    }

    for(i=0; i<MAX_SETTING_VAL_SET_NUMBERT; i++)
    {
        if(TXT_NULL != cur_item.val_set[i]
            && 0 != cur_item.val_set[i])
        {
            count++;
        }
    }

    if(PNULL != max_item_num)
    {
        *max_item_num = count;
    }

    SCI_TRACE_LOW("[MMIDC] MMIDC_GetSettingItemsTxtOption count=%d", count);

    return cur_item_index;
}
/*****************************************************************************/
//  Description : get setting items txt content
//  Global resource dependence : none
//  Author: chunyou
//  Date: 
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetSettingItemsTxtContent(uint16 type,//in
                                                        MMIDC_SETTING_ITEM_T *item_ptr //out
                                                        )
{
    uint32 cur_item = 0;

    MMIDC_SETTING_ITEM_T item_quality = 
    {
        TXT_DC_QUALITY,
            {TXT_DC_BRIEF,
            TXT_COMMON_COMMON_NORMAL,
            TXT_COMMON_HIGHQUALITY
            }
    };
    MMIDC_SETTING_ITEM_T item_coruscate = 
    {
        TXT_DC_CORUSCATE,
            {TXT_DC_50HZ, 
            TXT_DC_60HZ,
            TXT_NULL
            }
    };

#ifndef CMCC_UI_STYLE
    MMIDC_SETTING_ITEM_T item_shuttervoice = 
    {
        TXT_DC_SHUTTERVOICE,
            {TXT_ACTIVE_OPERATIVE,
            TXT_CLOSE,
            TXT_NULL
            }
    };
#endif
    MMIDC_SETTING_ITEM_T item_auto_save = 
    {
        TXT_DC_AUTO_SAVE,
            {TXT_ACTIVE_OPERATIVE,
            TXT_CLOSE,
            TXT_NULL
            }
    };
    MMIDC_SETTING_ITEM_T item_storage = 
    {
        TXT_PRIOR_STORE_MEDIUM,
            {TXT_NULL,
            TXT_NULL,
            TXT_NULL
            } 
    };
#if defined(MMI_CAMERA_F_PHOTO_DATE)
    MMIDC_SETTING_ITEM_T item_photo_date = 
    {
        TXT_PHOTO_DATE,
            {TXT_ACTIVE_OPERATIVE,
            TXT_CLOSE,
            TXT_NULL
            }
    };
#endif
#ifndef MMIDC_DESKTOP_ICON_BAR
#ifndef MMIDC_MINI_RESOURCE_SIZE
    MMIDC_SETTING_ITEM_T item_display_tip = 
    {
        TXT_DC_SHOW_SETTING,
            {TXT_ACTIVE_OPERATIVE,
            TXT_CLOSE,
            TXT_NULL
            }  
    };
#endif
#endif

    MMIDC_SETTING_ITEM_T item_audio = 
    {
        TXT_AUDIO,
            {TXT_ACTIVE_OPERATIVE,
            TXT_CLOSE,
            TXT_NULL
            } 
    };
#if defined MMIDC_MINI_DISPLAY_STYLE   
	MMIDC_SETTING_ITEM_T item_videoformat = 
    {
        TXT_SET_VIDEOFORMAT,
            {
			TXT_DV_TYPE_3GP,
#if defined(AVI_ENC_SUPPORT) && !defined(CMCC_TEST_FLAG)
            TXT_DV_TYPE_AVI,
#else               
			TXT_NULL,
#endif             
            TXT_NULL    
            }
    };
#else
	MMIDC_SETTING_ITEM_T item_videoformat = 
    {
        TXT_SET_VIDEOFORMAT,
            {
#if defined(AVI_ENC_SUPPORT) && !defined(CMCC_TEST_FLAG)
            TXT_DV_TYPE_AVI,
#else               
			TXT_DV_TYPE_MP4,
#endif 
            TXT_DV_TYPE_3GP,
            TXT_NULL    
            }
    };
#endif
    MMIDC_SETTING_ITEM_T item_night_mode = 
    {
        TXT_NIGHT_MODE, 
            {
             TXT_ACTIVE_OPERATIVE,
             TXT_CLOSE,
             TXT_NULL
            }
    };

    MMIDC_SETTING_ITEM_T item_multi_shot = 
    {
        TXT_DC_MULTI_SHOT, 
            {
              TXT_COMMON_COMMON_DISABLE,
              TXT_DC_3_MULTI_SHOT,
              TXT_DC_6_MULTI_SHOT
            }
    };

    MMIDC_SETTING_ITEM_T item_self_shot = 
    {
        TXT_DC_SELF_SHOT, 
            {
              TXT_COMMON_COMMON_DISABLE,
              TXT_COMM_3_SECONDS,
              TXT_COMM_10_SECONDS
             }
    };

#ifdef MMIDC_F_FLASH
    MMIDC_SETTING_ITEM_T item_dc_flash = 
    {
        TXT_DC_FLASH, 
            {
              TXT_CLOSE,
              TXT_OPEN,
              TXT_AUTO
             }
    };

    MMIDC_SETTING_ITEM_T item_dv_flash = 
    {
        TXT_DC_FLASH, 
            {
              TXT_CLOSE,
              TXT_OPEN,
              TXT_NULL
             }
    };
#endif

    MMIDC_SETTING_ITEM_T *src_item_ptr = PNULL;    

    switch(type)
    {
    case MMIDC_SETTINGS_SET_QUALITY:
        {
            src_item_ptr = &item_quality;
        }
        break;
    case MMIDC_SETTINGS_SET_DC_CORUSCATE:
    case MMIDC_SETTINGS_SET_DV_CORUSCATE:
        {
            src_item_ptr = &item_coruscate;
        }
        break;
#ifndef CMCC_UI_STYLE
    case MMIDC_SETTINGS_SET_SHUTTERVOICE:
        {
            src_item_ptr = &item_shuttervoice;
        }
        break;
#endif
    case MMIDC_SETTINGS_SET_AUTO_SAVE:
        {
            src_item_ptr = &item_auto_save;
        }                
        break;

    case MMIDC_SETTINGS_SET_DC_STORAGE:
    case MMIDC_SETTINGS_SET_DV_STORAGE:
        {
            int8   cur_dev_item = 0;
            MMI_TEXT_ID_T  dev_str_id[MMI_DEVICE_NUM] = {0};
            MMIFILE_DEVICE_E  dev_list[MMI_DEVICE_NUM] = {0};
            MMIFILE_DEVICE_E dc_dev = 0;
            int8 max_dev_number = 0;
            int i = 0;
            
            max_dev_number = GetStorageDeviceList(dev_list, dev_str_id);
            item_storage.val_set[0] = TXT_NO_SD_CARD_ALERT;
            if(CAMERA_MODE_DC == MMIDC_GetCameraMode())
            {
                dc_dev = MMIDC_GetPrefPhotoStorageDevice();
            }

            else
            {
                dc_dev = MMIDC_GetPrefVideoStorageDevice();
            }
                
            cur_dev_item = 0;
            for (i = 0; i < max_dev_number; i++)
            {
                if (dc_dev == dev_list[i])
                {
                    cur_dev_item = i;
                }
                item_storage.val_set[i] = dev_str_id[i];       
            }
            cur_item = cur_dev_item;
            src_item_ptr = &item_storage;
        }
        break;
 
#if defined(MMI_CAMERA_F_PHOTO_DATE)
    case MMIDC_SETTINGS_SET_PHOTO_DATE:
        {
            src_item_ptr = &item_photo_date;
        }
        break;
#endif
#ifndef MMIDC_DESKTOP_ICON_BAR
#ifndef MMIDC_MINI_RESOURCE_SIZE
    case MMIDC_SETTINGS_SET_DC_DISPLAY_TIP: // display parameter
    case MMIDC_SETTINGS_SET_DV_DISPLAY_TIP:
        {
            src_item_ptr = &item_display_tip;
        }
        break;
#endif
#endif

    case MMIDC_SETTINGS_SET_AUDIO:
        {
            src_item_ptr = &item_audio;
        }
        break;
    case MMIDC_SETTINGS_SET_VIDEOFORMAT:
        {
            src_item_ptr = &item_videoformat;
        }
        break;

    case MMIDC_SETTINGS_SET_ENVIRONMENT:
        {
            src_item_ptr = &item_night_mode;
        }
        break;

    case MMIDC_SETTINGS_SET_PIC_MODE:
        {
            src_item_ptr = &item_multi_shot;
        }
        break;

    case MMIDC_SETTINGS_SET_SELF_SHOT:
        {
            src_item_ptr = &item_self_shot;
        }
        break;

#ifdef MMIDC_F_FLASH
    case MMIDC_SETTINGS_SET_DC_FLASH:
        {
            src_item_ptr = &item_dc_flash;
        }
        break;

    case MMIDC_SETTINGS_SET_DV_FLASH:
        {
            src_item_ptr = &item_dv_flash;
        }
        break;
#endif

    default:
        break;

    }

    if(PNULL != item_ptr
        && PNULL != src_item_ptr)
    {
        MMI_MEMCPY(item_ptr,
            sizeof(MMIDC_SETTING_ITEM_T),
            src_item_ptr, 
            sizeof(MMIDC_SETTING_ITEM_T),
            sizeof(MMIDC_SETTING_ITEM_T));
    }

    SCI_TRACE_LOW("[MMIDC] MMIDC_GetSettingItemsTxtContent cur_item=%d", cur_item);
    return cur_item;
    
}

/*****************************************************************************/
//Description : get dc setting items
//Global resource dependence : 
//Author: robert.wang
//Note:   
/*****************************************************************************/
LOCAL uint16 GetSettingItems(MMIDC_OPTION_ITEM_T *option_item_ptr //out
                                          )
{
    CAMERA_MODE_E   mode = CAMERA_MODE_DC;
    int8    i = 0;
    int8    cur_index = 0;
    MMI_TEXT_ID_T image_id = IMAGE_NULL;
    MMI_TEXT_ID_T text_id = TXT_NULL;
    MMI_TEXT_ID_T text1_id = TXT_NULL;
    int8 max_item_num = 0;
    uint32 user_data = 0;
    int8 selected_item = 0;
    BOOLEAN is_gray = FALSE;
    MMIDC_SETTINGS_SET_E optin_arr[MMIDC_SETTINGS_SET_MAX_VALUE] = {0};
    uint16 arr_size_out = 0;

#ifdef MMIDC_MINI_RESOURCE_SIZE
    int8  cur_size_index = 0;
    SETTING_PHOTO_SIZE_INFO_T *photo_size_ptr = MMIDC_GetSettingPhotoSizeInfo();
    SETTING_VIDEO_SIZE_INFO_T *video_size_ptr = MMIDC_GetSettingVideoSizeInfo();
#endif    

    if(PNULL == option_item_ptr)
    {
        return 0;
    }    

    MMIDC_GetCurModeSettingOption(optin_arr, MMIDC_SETTINGS_SET_MAX_VALUE, &arr_size_out);
    mode = MMIDC_GetCameraMode();

    for(i = 0; i < arr_size_out; i++)
    {
#ifdef MMIDC_MINI_RESOURCE_SIZE
        int16  cur_photo_size = 0;
        int16  j = 0;
#endif
        MMIDC_SETTING_ITEM_T cur_item = 
        {
            TXT_NULL,
                {TXT_NULL,
                TXT_NULL,
                TXT_NULL
                }
        };

        is_gray = FALSE;
        user_data = optin_arr[i];
        
        switch (user_data)
        {
#ifdef MMIDC_MINI_RESOURCE_SIZE
#ifdef MMIDC_F_FLASH
        case MMIDC_SETTINGS_SET_DC_FLASH:
        case MMIDC_SETTINGS_SET_DV_FLASH:
            {
                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetFlashLamp();
                image_id = IMAGE_COMMON_RIGHT_ARROW;
                if(SETTING_SENSOR_FRONT == MMIDC_GetSettingSensorID())
                {
                    is_gray = TRUE;
                }
            }
            break;
#endif

        case MMIDC_SETTINGS_SET_PHOTO_SIZE:
            {
                cur_photo_size = MMIDC_GetPhotoSize();
                cur_item.label = TXT_COMM_SIZE;
                for (j = 0; j < photo_size_ptr->total_num; j++)
                {
                    if (cur_photo_size == photo_size_ptr->photo_size_array[j])
                    {
                        cur_size_index = j;
                    }
                }        
                cur_index = cur_size_index;
                image_id = IMAGE_COMMON_RIGHT_ARROW;
            }
            break;
#endif
        case MMIDC_SETTINGS_SET_QUALITY:
            {
                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetPhotoQuality();
                image_id = IMAGE_COMMON_RIGHT_ARROW;
            }
            break;
        case MMIDC_SETTINGS_SET_DC_CORUSCATE:
            {
                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetPhotoFlicker();
                image_id = IMAGE_COMMON_RIGHT_ARROW;
            }
            break;
#ifndef CMCC_UI_STYLE
        case MMIDC_SETTINGS_SET_SHUTTERVOICE:
            {
                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetShutterVoice();
                if(SHUTTER_VOICE_1 == cur_index)
                {
                    image_id = IMAGE_CHECK_SELECTED_ICON;
                }
                else
                {
                    image_id = IMAGE_CHECK_UNSELECTED_ICON;
                }
            }
            break;
#endif
        case MMIDC_SETTINGS_SET_AUTO_SAVE:
            {
                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetIsAutoSave();
                if(AUTO_SAVE_ON == cur_index)
                {
                    image_id = IMAGE_CHECK_SELECTED_ICON;
                }
                else
                {
                    image_id = IMAGE_CHECK_UNSELECTED_ICON;
                }
#if defined(MMIDC_F_WORK_MODE)
                if(MMIDC_IsWorkMode())
                {
                    is_gray = TRUE;
                }
#endif
            }                
            break;

        case MMIDC_SETTINGS_SET_DC_STORAGE:
        case MMIDC_SETTINGS_SET_DV_STORAGE:
            {
                cur_index = MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                image_id = IMAGE_COMMON_RIGHT_ARROW;
            }
            break;
     
#if defined(MMI_CAMERA_F_PHOTO_DATE)
        case MMIDC_SETTINGS_SET_PHOTO_DATE:
            {
                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetNVPhotoDate();
                if(PHOTO_DATE_ON == cur_index)
                {
                    image_id = IMAGE_CHECK_SELECTED_ICON;
                }
                else
                {
                    image_id = IMAGE_CHECK_UNSELECTED_ICON;
                }
            }
            break;
#endif
#ifndef MMIDC_DESKTOP_ICON_BAR
#ifndef MMIDC_MINI_RESOURCE_SIZE
        case MMIDC_SETTINGS_SET_DC_DISPLAY_TIP: // display parameter
            {
                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetNeedPhotoDisplaySettingTip();
                if(PHOTO_DISPLAY_SETTING_TIP_ON == cur_index)
                {
                    image_id = IMAGE_CHECK_SELECTED_ICON;
                }
                else
                {
                    image_id = IMAGE_CHECK_UNSELECTED_ICON;
                }
            }
            break;
#endif
#endif
#ifdef MMIDC_ADD_SPECIAL_EFFECT
        case MMIDC_SETTINGS_SET_PIC_MODE:
            {
#if defined(MMIDC_F_WORK_MODE)
                //not support burst in work mode
                if (MMIDC_IsWorkMode())
                {
                    is_gray = TRUE;
                }
#endif
				
                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetMultiShootEnum();
                image_id = IMAGE_COMMON_RIGHT_ARROW;
            }
            break;
#endif

#ifdef MMIDC_MINI_RESOURCE_SIZE
        case MMIDC_SETTINGS_SET_VIDEO_SIZE:
            {
                cur_photo_size = MMIDC_GetVideoSize();
                cur_item.label = TXT_COMM_SIZE;
                for (j = 0; j < video_size_ptr->total_num; j++)
                {
                    if (cur_photo_size == video_size_ptr->video_size_array[j])
                    {
                        cur_size_index = j;
                    }
                }        
                cur_index = cur_size_index;
                image_id = IMAGE_COMMON_RIGHT_ARROW;
            }
            break;
#endif
        case MMIDC_SETTINGS_SET_DV_CORUSCATE:
            {
                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetVideoFlicker();
                image_id = IMAGE_COMMON_RIGHT_ARROW;
            }
            break;
        case MMIDC_SETTINGS_SET_AUDIO:
            {
                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetVideoAudioSetting();
                if(AUDIO_SETTING_ON == cur_index)
                {
                    image_id = IMAGE_CHECK_SELECTED_ICON;
                }
                else
                {
                    image_id = IMAGE_CHECK_UNSELECTED_ICON;
                }
            }
            break;
        case MMIDC_SETTINGS_SET_VIDEOFORMAT:
            {
                MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
                cur_index = MMIDC_GetRecordFileTypeOfSetting();
                image_id = IMAGE_COMMON_RIGHT_ARROW; 
            }
            break;

#ifndef MMIDC_DESKTOP_ICON_BAR
#ifndef MMIDC_MINI_RESOURCE_SIZE
        case MMIDC_SETTINGS_SET_DV_DISPLAY_TIP: // display parameter
            MMIDC_GetSettingItemsTxtContent(user_data, &cur_item);
            cur_index = MMIDC_GetNeedVideoDisplaySettingTip();            
            if(VIDEO_DISPLAY_SETTING_TIP_ON == cur_index)
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;
            }
            break;
#endif
#endif          

        default:
            cur_index = 0;
            break;

        }

        if (TXT_NULL != cur_item.label)
        {
            text_id = cur_item.label;
#ifdef MMIDC_MINI_RESOURCE_SIZE
            if(TXT_COMM_SIZE == text_id)
            {
                if(CAMERA_MODE_DC == mode)
                {
                    text1_id = photo_size_ptr->photo_size_array[cur_index];
                }
                else if(CAMERA_MODE_DV == mode)
                {
                    text1_id = video_size_ptr->video_size_array[cur_index];
                }
            }
            else
#endif
            {
                text1_id = cur_item.val_set[cur_index];
            }
#ifdef MMIDC_MINI_RESOURCE_SIZE
 			if(TXT_COMM_SIZE != text_id) 
#endif
		 	{
				if (0 == text1_id)
				{
					text1_id = TXT_NULL;                
				}
		 	}
            option_item_ptr->is_gray[max_item_num] = is_gray;
            option_item_ptr->text1_array[max_item_num] = text1_id;
            option_item_ptr->text_array[max_item_num] = text_id;       
            option_item_ptr->image_array[max_item_num] = image_id;
            option_item_ptr->user_data[max_item_num] = user_data;

            max_item_num++;
        }
    }

    option_item_ptr->max_item_num = max_item_num;
    for (i = 0; i < max_item_num; i++)
    {
        if(s_current_setting_item == option_item_ptr->user_data[i])
        {
            selected_item = i;
        }
    }
    option_item_ptr->cur_item = selected_item;
    option_item_ptr->item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    return max_item_num;
}


/*****************************************************************************/
//Description : get dc setting value items
//Global resource dependence : 
//Author: robert.wang
//Note:   
/*****************************************************************************/
LOCAL uint16 GetDCSettingValueItems(
                            uint32 setting_num, //in
                            MMI_TEXT_ID_T* win_text_id_ptr,//out
                            MMIDC_OPTION_ITEM_T *option_item_ptr //out
                                          )
{
    int8 max_item_num = 0;
    uint16 cur_item = 0;    
    int32 i = 0;

    if(PNULL == option_item_ptr)
    {
        return 0;
    }
    
    switch(setting_num)
    {
#ifdef MMIDC_MINI_RESOURCE_SIZE
#ifdef MMIDC_F_FLASH
        case MMIDC_SETTINGS_SET_DC_FLASH:
        case MMIDC_SETTINGS_SET_DV_FLASH:
            {
                cur_item = MMIDC_GetFlashLamp();
                MMIDC_GetSettingItemsTxtOption(setting_num, 
                                            OPTION_ITEM_MAX, 
                                            option_item_ptr->text_array, 
                                            &max_item_num, 
                                            win_text_id_ptr);                
            }
            break;
#endif
#endif
    case MMIDC_SETTINGS_SET_QUALITY:
        {
            cur_item = MMIDC_GetPhotoQuality();
            MMIDC_GetSettingItemsTxtOption(setting_num, 
                                            OPTION_ITEM_MAX, 
                                            option_item_ptr->text_array, 
                                            &max_item_num, 
                                            win_text_id_ptr);
        }
        break;
    case MMIDC_SETTINGS_SET_DC_CORUSCATE:
        {
            cur_item = MMIDC_GetPhotoFlicker();
            MMIDC_GetSettingItemsTxtOption(setting_num, 
                                            OPTION_ITEM_MAX, 
                                            option_item_ptr->text_array, 
                                            &max_item_num, 
                                            win_text_id_ptr);
        }
        break;
        
	case MMIDC_SETTINGS_SET_DC_STORAGE:
		{
          cur_item = MMIDC_GetSettingItemsTxtOption(setting_num, 
                                            OPTION_ITEM_MAX, 
                                            option_item_ptr->text_array, 
                                            &max_item_num, 
                                            win_text_id_ptr);
        }
        break;        
#ifdef MMIDC_ADD_SPECIAL_EFFECT
    case MMIDC_SETTINGS_SET_PIC_MODE:
        {
            cur_item = MMIDC_GetMultiShootEnum();
            MMIDC_GetSettingItemsTxtOption(setting_num, 
				OPTION_ITEM_MAX, 
				option_item_ptr->text_array, 
				&max_item_num, 
				win_text_id_ptr);
        }
        break;
#endif
    
	default:
        break;
    }

    for(i = 0; i < max_item_num; i++)
    {
        option_item_ptr->user_data[i] = MMIDC_SETTINGS_SET_MAX_VALUE;
        option_item_ptr->is_gray[i] = FALSE;
    }
    
    option_item_ptr->cur_item = cur_item;
    option_item_ptr->max_item_num =  max_item_num;
    option_item_ptr->item_style = GUIITEM_STYLE_ONE_LINE_RADIO_EXT;
    
    return max_item_num;
}

/*****************************************************************************/
//Description : get dv setting value items
//Global resource dependence : 
//Author: robert.wang
//Note:   
/*****************************************************************************/
LOCAL uint16 GetDVSettingValueItems(
                            uint32 setting_num, //in
                            MMI_TEXT_ID_T* win_text_id_ptr,//out
                            MMIDC_OPTION_ITEM_T *option_item_ptr //out
                                          )
{
    int8 max_item_num = 0;
    int8 cur_item = 0;    
    int8 i = 0;

    if(PNULL == option_item_ptr)
    {
        return 0;
    }

    switch(setting_num)
    {
    case MMIDC_SETTINGS_SET_DV_CORUSCATE:
        {
            cur_item = MMIDC_GetVideoFlicker();
            MMIDC_GetSettingItemsTxtOption(setting_num, 
                                            OPTION_ITEM_MAX, 
                                            option_item_ptr->text_array, 
                                            &max_item_num, 
                                            win_text_id_ptr);      
        }
        break;
    case MMIDC_SETTINGS_SET_VIDEOFORMAT:
        {
            cur_item = MMIDC_GetRecordFileTypeOfSetting();
            MMIDC_GetSettingItemsTxtOption(setting_num, 
                                            OPTION_ITEM_MAX, 
                                            option_item_ptr->text_array, 
                                            &max_item_num, 
                                            win_text_id_ptr);
        }
        break;
        
	case MMIDC_SETTINGS_SET_DV_STORAGE:	
        {
            cur_item = MMIDC_GetSettingItemsTxtOption(setting_num, 
                                            OPTION_ITEM_MAX, 
                                            option_item_ptr->text_array, 
                                            &max_item_num, 
                                            win_text_id_ptr);
        }
        break;

    default:
        break;
    }
    
    for(i = 0; i < max_item_num; i++)
    {
        option_item_ptr->user_data[i] = MMIDC_SETTINGS_SET_MAX_VALUE;
        option_item_ptr->is_gray[i] = FALSE;
    }


    option_item_ptr->cur_item = cur_item;
    option_item_ptr->max_item_num =  max_item_num;
    option_item_ptr->item_style = GUIITEM_STYLE_ONE_LINE_RADIO_EXT;
    
    return max_item_num;
}

#ifdef MMIDC_MINI_RESOURCE_SIZE
/*****************************************************************************/
// 	Description : add size item to setlist
//	Global resource dependence : 
//  Author: robert.wang
//	Note:   
/*****************************************************************************/
LOCAL void  AddSizeItemToSetlist(void)
{
    MMI_CTRL_ID_T list_id = MMIDC_SETTING_LIST_CTRL_ID;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    int32    i = 0;
    int16 max_item_num = 0;
    CAMERA_MODE_E   mode = CAMERA_MODE_DC;    
    SETTING_PHOTO_SIZE_INFO_T *photo_size_ptr = MMIDC_GetSettingPhotoSizeInfo();
    SETTING_VIDEO_SIZE_INFO_T *video_size_ptr = MMIDC_GetSettingVideoSizeInfo();
    int16 cur_photo_size = 0;
    int16 cur_size_index = 0;

    mode = MMIDC_GetCameraMode();
    if (CAMERA_MODE_DC == mode)
    {
        max_item_num = photo_size_ptr->total_num;
        cur_photo_size = MMIDC_GetPhotoSize();
    }
    else
    {
        max_item_num = video_size_ptr->total_num;
        cur_photo_size = MMIDC_GetVideoSize();
    }

    GUILIST_SetMaxItem(list_id, max_item_num, FALSE);

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_RADIO_EXT;
    item_t.item_state = 0;
    item_t.item_data_ptr = &item_data;

    for (i = 0; i < max_item_num; i++)
    {
        MMI_STRING_T text_str = {0};
        int16 cur_size = 0;

        if (CAMERA_MODE_DC == mode)
        {
            MMIDC_GetSizeString((uint16)photo_size_ptr->photo_size_array[i], &text_str);
            cur_size = photo_size_ptr->photo_size_array[i];
        }
        else
        {
            MMIDC_GetSizeString((uint16)video_size_ptr->video_size_array[i], &text_str);
            cur_size = video_size_ptr->video_size_array[i];
        }
        
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = text_str.wstr_ptr;
        item_data.item_content[0].item_data.text_buffer.wstr_len = text_str.wstr_len;
        item_t.user_data = MMIDC_SETTINGS_SET_MAX_VALUE;

        GUILIST_AppendItem(list_id, &item_t);

        if (cur_photo_size == cur_size)
        {
            cur_size_index = i;
        }
    }    
	GUILIST_SetSelectedItem(list_id, cur_size_index, TRUE);
    GUILIST_SetCurItemIndex(list_id, cur_size_index);
}
#endif

/*****************************************************************************/
// 	Description : add item to setlist
//	Global resource dependence : 
//  Author: robert.wang
//	Note:   
/*****************************************************************************/
LOCAL void  AddItemToSetlist(MMIDC_OPTION_ITEM_T *option_item_ptr)
{
    MMI_CTRL_ID_T list_id = MMIDC_SETTING_LIST_CTRL_ID;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    uint32 item_index = 0;

    int32    i = 0;

    if(PNULL == option_item_ptr)
    {
        return;
    }

    GUILIST_SetMaxItem(list_id, option_item_ptr->max_item_num, FALSE);

    item_t.item_style = option_item_ptr->item_style;
    item_t.item_state = 0;
    item_t.item_data_ptr = &item_data;

    for (i = 0; i < option_item_ptr->max_item_num; i++)
    {
        if((TXT_NULL != option_item_ptr->text_array[i]) && 
            (0 != option_item_ptr->text_array[i]))
        {
            item_t.user_data = option_item_ptr->user_data[i];

            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[0].item_data.text_id = option_item_ptr->text_array[i];

            if((0 != option_item_ptr->image_array[i]) &&
                (IMAGE_NULL != option_item_ptr->image_array[i])) 
            {
                item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[1].item_data.image_id = option_item_ptr->image_array[i];
            }

            if((0 != option_item_ptr->text1_array[i]) && 
                (TXT_NULL != option_item_ptr->text1_array[i])
				||(TXT_COMM_SIZE == option_item_ptr->text_array[i]))
            {
                if(TXT_COMM_SIZE == option_item_ptr->text_array[i])
                {
                    MMI_STRING_T text_str = {0};
                    
                    MMIDC_GetSizeString((uint16)option_item_ptr->text1_array[i], &text_str);
                    item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                    item_data.item_content[2].item_data.text_buffer.wstr_ptr = text_str.wstr_ptr;
                    item_data.item_content[2].item_data.text_buffer.wstr_len = text_str.wstr_len;
                }
                else
                {
                    item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_ID;
                    item_data.item_content[2].item_data.text_id = option_item_ptr->text1_array[i];
                }
            }

            GUILIST_AppendItem(list_id, &item_t);
            
            if(option_item_ptr->is_gray[i])
            {
                GUILIST_SetItemGray(list_id, item_index, TRUE);
                GUILIST_SetItemInvalid(list_id, item_index, TRUE);
            }

            item_index++;
        }
    }    
    GUILIST_SetSelectedItem(list_id, option_item_ptr->cur_item, TRUE);
    GUILIST_SetCurItemIndex(list_id, option_item_ptr->cur_item);
}

/*****************************************************************************/
//Description : display setting list
//Global resource dependence : 
//Author: robert.wang
//Note:   
/*****************************************************************************/
LOCAL void DisplaySettingList(void)
{
    MMIDC_OPTION_ITEM_T option_item = {0};

    s_current_setting_item = 0;

    GetSettingItems(&option_item);
    AddItemToSetlist(&option_item);
}

/*****************************************************************************/
//Description : replace setting list item
//Global resource dependence : 
//Author: robert.wang
//Note:   
/*****************************************************************************/
LOCAL BOOLEAN ReplaceSettingListItem(uint32 index, uint32 user_data)
{
    MMI_CTRL_ID_T list_id = MMIDC_SETTING_LIST_CTRL_ID; 
    const GUILIST_ITEM_T* item_ptr = PNULL;
    GUILIST_ITEM_T item_t = {0};
    BOOLEAN ret = TRUE;
    MMI_TEXT_ID_T text_id = TXT_NULL;
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    uint16  cur_index = 0;       

    item_ptr = GUILIST_GetItemPtrByIndex(list_id, index);
    if((PNULL == item_ptr) || (PNULL == item_ptr->item_data_ptr))
    {
        return ret;
    }
    MMI_MEMCPY(&item_t, 
        sizeof(GUILIST_ITEM_T),
        item_ptr, 
        sizeof(GUILIST_ITEM_T),
        sizeof(GUILIST_ITEM_T));    

    switch(user_data)
    {
#if !defined(CMCC_UI_STYLE)
    case MMIDC_SETTINGS_SET_SHUTTERVOICE:
        cur_index = MMIDC_GetShutterVoice();
        if(SHUTTER_VOICE_1 == cur_index)
        {
            image_id = IMAGE_CHECK_SELECTED_ICON;                    
            text_id = TXT_ACTIVE_OPERATIVE;
        }
        else
        {
            image_id = IMAGE_CHECK_UNSELECTED_ICON;                    
            text_id = TXT_CLOSE;
        }
        break;
#endif
    case MMIDC_SETTINGS_SET_AUTO_SAVE:
        cur_index = MMIDC_GetIsAutoSave();
        if(AUTO_SAVE_ON == cur_index)
        {
            image_id = IMAGE_CHECK_SELECTED_ICON;                    
            text_id = TXT_ACTIVE_OPERATIVE;
        }
        else
        {
            image_id = IMAGE_CHECK_UNSELECTED_ICON;                    
            text_id = TXT_CLOSE;
        }
        break;
#if defined(MMI_CAMERA_F_PHOTO_DATE)        
    case MMIDC_SETTINGS_SET_PHOTO_DATE:
        cur_index = MMIDC_GetNVPhotoDate();
        if(PHOTO_DATE_ON == cur_index)
        {
            image_id = IMAGE_CHECK_SELECTED_ICON;
            text_id = TXT_ACTIVE_OPERATIVE;
        }
        else
        {
            image_id = IMAGE_CHECK_UNSELECTED_ICON;                    
            text_id = TXT_CLOSE;
        }
        break;
#endif        
    case MMIDC_SETTINGS_SET_AUDIO:

        cur_index = MMIDC_GetVideoAudioSetting();
        if(AUDIO_SETTING_ON == cur_index)
        {
            image_id = IMAGE_CHECK_SELECTED_ICON;                    
            text_id = TXT_ACTIVE_OPERATIVE;
        }
        else
        {
            image_id = IMAGE_CHECK_UNSELECTED_ICON;                    
            text_id = TXT_CLOSE;
        }        
        break;
#ifndef MMIDC_DESKTOP_ICON_BAR
#ifndef MMIDC_MINI_RESOURCE_SIZE
    case MMIDC_SETTINGS_SET_DC_DISPLAY_TIP:
        cur_index = MMIDC_GetNeedPhotoDisplaySettingTip();
        if(PHOTO_DISPLAY_SETTING_TIP_ON == cur_index)
        {
            image_id = IMAGE_CHECK_SELECTED_ICON;                    
            text_id = TXT_ACTIVE_OPERATIVE;
        }
        else
        {
            image_id = IMAGE_CHECK_UNSELECTED_ICON;                    
            text_id = TXT_CLOSE;
        }
        break;
    case MMIDC_SETTINGS_SET_DV_DISPLAY_TIP:
        cur_index = MMIDC_GetNeedVideoDisplaySettingTip();
        if(VIDEO_DISPLAY_SETTING_TIP_ON == cur_index)
        {
            image_id = IMAGE_CHECK_SELECTED_ICON;                    
            text_id = TXT_ACTIVE_OPERATIVE;
        }
        else
        {
            image_id = IMAGE_CHECK_UNSELECTED_ICON;                    
            text_id = TXT_CLOSE;
        }
        break;
#endif
#endif
    default:
        break;
    }

    item_t.item_data_ptr->item_content[1].item_data.image_id = image_id;
    item_t.item_data_ptr->item_content[2].item_data.text_id = text_id;
    ret = GUILIST_ReplaceItem(list_id, &item_t, index);  

    return ret;
}


/*****************************************************************************/
//Description : handle value select
//Global resource dependence : 
//Author: robert.wang
//Note:   
/*****************************************************************************/
LOCAL void HandleValueSelect(uint16 index)
{
    uint32 item_id = s_current_setting_item;
    int8    max_device = 0;
    MMIFILE_DEVICE_E file_dev = 0;
    MMIFILE_DEVICE_E  dev_list[MMI_DEVICE_NUM] = {0};
    max_device = GetStorageDeviceList(dev_list, PNULL);

    switch(item_id)
    {
#ifdef MMIDC_MINI_RESOURCE_SIZE
#ifdef MMIDC_F_FLASH
        case MMIDC_SETTINGS_SET_DC_FLASH:
        case MMIDC_SETTINGS_SET_DV_FLASH:
            MMIDC_SetFlashLamp((DCAMERA_FLASH_MODE_E)index);
            break;
#endif
#endif
    case MMIDC_SETTINGS_SET_QUALITY:
        MMIDC_SetPhotoQuality((PHOTO_QUALITY_E)index);
        break;
    case MMIDC_SETTINGS_SET_DC_CORUSCATE:
        MMIDC_SetPhotoFlicker((FLICKER_E)index);
        break;
    case MMIDC_SETTINGS_SET_DC_STORAGE:
		if(max_device > 0)
        {
            file_dev = dev_list[index];
            if(file_dev != MMIDC_GetPrefPhotoStorageDevice())
            {
                MMIDC_DeleteAllSavedImageFile();
            }
            MMIDC_SetPrefPhotoStorageDevice(file_dev);
        }
        break;

    case MMIDC_SETTINGS_SET_DV_CORUSCATE:
        MMIDC_SetVideoFlicker((FLICKER_E)index);
        break;
    case MMIDC_SETTINGS_SET_VIDEOFORMAT:
        MMIDC_SetRecordFileTypeOfSetting(index);
        break;
    
    case MMIDC_SETTINGS_SET_DV_STORAGE:
        if(max_device > 0)
        {
            file_dev = dev_list[index];
            MMIDC_SetPrefVideoStorageDevice(file_dev);
        }
        break;

#ifdef MMIDC_MINI_RESOURCE_SIZE 	
    case MMIDC_SETTINGS_SET_PHOTO_SIZE:
        {
            SETTING_PHOTO_SIZE_INFO_T *photo_size_ptr = MMIDC_GetSettingPhotoSizeInfo();        
            MMIAPIDC_SetPhotoSize(photo_size_ptr->photo_size_array[index]);
        }
        break;
    case MMIDC_SETTINGS_SET_VIDEO_SIZE:
        {
            SETTING_VIDEO_SIZE_INFO_T *video_size_ptr = MMIDC_GetSettingVideoSizeInfo();
            MMIDC_SetVideoSize(video_size_ptr->video_size_array[index]);
        }
        break;
#endif
#ifdef MMIDC_ADD_SPECIAL_EFFECT
    case MMIDC_SETTINGS_SET_PIC_MODE:
		{
			MMIDC_SetMultiShootEnum((MULTI_SHOOT_E)index);
		}
		break;
#endif    
	default:
        break;
    }

}

/*****************************************************************************/
//Description : handle item select
//Global resource dependence : 
//Author: robert.wang
//Note:   
/*****************************************************************************/
LOCAL void HandleItemSelect(MMI_WIN_ID_T      win_id)
{
    MMI_CTRL_ID_T list_id = MMIDC_SETTING_LIST_CTRL_ID; 
    uint16 index = 0;
    uint32      user_data = 0;
    MMI_TEXT_ID_T window_title = TXT_NULL;
    MMIDC_OPTION_ITEM_T option_item = {0};
    
    index = GUILIST_GetCurItemIndex(list_id);
    GUILIST_GetItemData(list_id, index, &user_data);

    if(MMIDC_SETTINGS_SET_MAX_VALUE != user_data)
    {
        s_current_setting_item = user_data;
    }

    switch((MMIDC_SETTINGS_SET_E)user_data)
    {
    case MMIDC_SETTINGS_SET_MAX_VALUE:
        GUILIST_ChangeDisplayType(
            list_id,
            GUILIST_TEXTLIST_E,
            FALSE
            );
        HandleValueSelect(index);
        GetSettingItems(&option_item);
        GUILIST_RemoveAllItems(list_id);
        AddItemToSetlist(&option_item);
        GUIWIN_SetTitleTextId(win_id, STXT_MAIN_SETTING, TRUE);
        MMK_SendMsg(list_id, MSG_CTL_PAINT, PNULL);
        break;
#ifdef MMIDC_ADD_SPECIAL_EFFECT
    case MMIDC_SETTINGS_SET_PIC_MODE:
#endif    
	case MMIDC_SETTINGS_SET_QUALITY:
    case MMIDC_SETTINGS_SET_DC_CORUSCATE:    
    case MMIDC_SETTINGS_SET_DC_STORAGE:
#ifdef MMIDC_MINI_RESOURCE_SIZE
#ifdef MMIDC_F_FLASH
        case MMIDC_SETTINGS_SET_DC_FLASH:
        case MMIDC_SETTINGS_SET_DV_FLASH:
#endif
#endif
        GUILIST_ChangeDisplayType(
            list_id,
            GUILIST_RADIOLIST_E,
            FALSE
            );
        GetDCSettingValueItems(user_data,&window_title, &option_item);
        GUIWIN_SetTitleTextId(win_id, window_title, TRUE);
        GUILIST_RemoveAllItems(list_id);
        AddItemToSetlist(&option_item);
        MMK_SendMsg(list_id, MSG_CTL_PAINT, PNULL);
        break;

#ifdef MMIDC_MINI_RESOURCE_SIZE
    case MMIDC_SETTINGS_SET_PHOTO_SIZE:        
    case MMIDC_SETTINGS_SET_VIDEO_SIZE:
        GUILIST_ChangeDisplayType(
            list_id,
            GUILIST_RADIOLIST_E,
            FALSE
            );
        GUIWIN_SetTitleTextId(win_id, TXT_COMM_SIZE, TRUE);
        GUILIST_RemoveAllItems(list_id);
        AddSizeItemToSetlist();
        MMK_SendMsg(list_id, MSG_CTL_PAINT, PNULL);
        break;
#endif

    case MMIDC_SETTINGS_SET_SHUTTERVOICE:
        if(SHUTTER_VOICE_1 == MMIDC_GetShutterVoice())
        {
            MMIDC_SetNVShutterVoice(SHUTTER_VOICE_DISABLE);
        }
        else
        {
            MMIDC_SetNVShutterVoice(SHUTTER_VOICE_1);
        }                
        ReplaceSettingListItem(index, user_data);
        MMK_SendMsg(list_id, MSG_CTL_PAINT, PNULL);
        break;
   
    case MMIDC_SETTINGS_SET_AUTO_SAVE:
        if(AUTO_SAVE_ON == MMIDC_GetIsAutoSave())
        {
            MMIDC_SetAutoSave(AUTO_SAVE_OFF);
        }
        else
        {
            MMIDC_SetAutoSave(AUTO_SAVE_ON);
        }                
        ReplaceSettingListItem(index, user_data);
        MMK_SendMsg(list_id, MSG_CTL_PAINT, PNULL);
        break;

#if defined(MMI_CAMERA_F_PHOTO_DATE)        
    case MMIDC_SETTINGS_SET_PHOTO_DATE:
        if(PHOTO_DATE_ON == MMIDC_GetNVPhotoDate())
        {
            MMIDC_SetNVPhotoDate(PHOTO_DATE_OFF);
        }
        else
        {
            MMIDC_SetNVPhotoDate(PHOTO_DATE_ON);
        }
        ReplaceSettingListItem(index, user_data);
        MMK_SendMsg(list_id, MSG_CTL_PAINT, PNULL);
        break;
#endif        

    case MMIDC_SETTINGS_SET_DV_CORUSCATE:
    case MMIDC_SETTINGS_SET_DV_STORAGE:
    case MMIDC_SETTINGS_SET_VIDEOFORMAT:
        GUILIST_ChangeDisplayType(
            list_id,
            GUILIST_RADIOLIST_E,
            FALSE
            );
        GetDVSettingValueItems(user_data,&window_title, &option_item);
        GUIWIN_SetTitleTextId(win_id, window_title, TRUE);
        GUILIST_RemoveAllItems(list_id);
        AddItemToSetlist(&option_item); 
        MMK_SendMsg(list_id, MSG_CTL_PAINT, PNULL);
        break;
    case MMIDC_SETTINGS_SET_AUDIO:
        if(AUDIO_SETTING_ON == MMIDC_GetVideoAudioSetting())
        {
            MMIDC_SetVideoAudioSetting(AUDIO_SETTING_OFF);
        }
        else
        {
            MMIDC_SetVideoAudioSetting(AUDIO_SETTING_ON);
        }                
        ReplaceSettingListItem(index, user_data);
        MMK_SendMsg(list_id, MSG_CTL_PAINT, PNULL);
        break;
#ifndef MMIDC_DESKTOP_ICON_BAR
#ifndef MMIDC_MINI_RESOURCE_SIZE
    case MMIDC_SETTINGS_SET_DC_DISPLAY_TIP:
        if(PHOTO_DISPLAY_SETTING_TIP_ON == MMIDC_GetNeedPhotoDisplaySettingTip())
        {
            MMIDC_SetNeedPhotoDisplaySettingTip(PHOTO_DISPLAY_SETTING_TIP_OFF);
        }
        else
        {
            MMIDC_SetNeedPhotoDisplaySettingTip(PHOTO_DISPLAY_SETTING_TIP_ON);
        }                
        ReplaceSettingListItem(index, user_data);
        MMK_SendMsg(list_id, MSG_CTL_PAINT, PNULL);
        break;
    case MMIDC_SETTINGS_SET_DV_DISPLAY_TIP:
        if(VIDEO_DISPLAY_SETTING_TIP_ON == MMIDC_GetNeedVideoDisplaySettingTip())
        {
            MMIDC_SetNeedVideoDisplaySettingTip(VIDEO_DISPLAY_SETTING_TIP_OFF);
        }
        else
        {
            MMIDC_SetNeedVideoDisplaySettingTip(VIDEO_DISPLAY_SETTING_TIP_ON);
        }                
        ReplaceSettingListItem(index, user_data);
        MMK_SendMsg(list_id, MSG_CTL_PAINT, PNULL);
        break;
#endif
#endif
    default:
        break;
    }
    
    
}
/*****************************************************************************/
// 	Description : handle dc setting windows messages
//	Global resource dependence : 
//  Author: robert.wang
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDCSettingWinMsg(
                                          MMI_WIN_ID_T      win_id, 	
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E     recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        DisplaySettingList();
        MMK_SetAtvCtrl(win_id, MMIDC_SETTING_LIST_CTRL_ID);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK: 
    case MSG_APP_OK:
        HandleItemSelect(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
#if defined MMIDC_MINI_RESOURCE_SIZE		
		{
			uint16 index = 0;
			uint32      user_data = 0;
			MMI_TEXT_ID_T window_title = TXT_NULL;
			MMIDC_OPTION_ITEM_T option_item = {0};
			MMI_CTRL_ID_T list_id = MMIDC_SETTING_LIST_CTRL_ID; 
			
			index = GUILIST_GetCurItemIndex(list_id);
			GUILIST_GetItemData(list_id, index, &user_data);
			
			if(MMIDC_SETTINGS_SET_MAX_VALUE == user_data)
			{
				GUILIST_ChangeDisplayType(
					list_id,
					GUILIST_TEXTLIST_E,
					FALSE
					);
				GetSettingItems(&option_item);
				GUILIST_RemoveAllItems(list_id);
				AddItemToSetlist(&option_item);
				GUIWIN_SetTitleTextId(win_id, STXT_MAIN_SETTING, TRUE);
				MMK_SendMsg(list_id, MSG_CTL_PAINT, PNULL);
			}
			else
			{
				MMK_CloseWin(win_id);
			}
		}       
#else 	    	
		MMK_CloseWin(win_id);
#endif
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
// 	Description : open setting window
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenSettingWin(void)
{
    MMIDC_CreateWinByApplet((uint32*)MMIDC_SETTING_WIN_TAB, (ADD_DATA)PNULL);
}
/*****************************************************************************/
//  Description : Close staroge Menu
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_CloseStorageMenu(void)
{
    MMK_CloseWin(MMIDC_SETTING_WIN_ID);
    MMK_CloseWin(MMIDC_STORAGE_OPT_WIN_ID);
}
#endif  //#ifdef  CAMERA_SUPPORT



/*Edit by script, ignore 1 case. Thu Apr 26 19:00:51 2012*/ //IGNORE9527


/*Edit by script, ignore 4 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527
