/****************************************************************************
** File Name:      mmiautodemo_main.c                                  *
** Author:         chunjie.liu                                                            *
** Date:           3/17/2012                                             *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2/2012         chujie liu         Create
****************************************************************************/
#define _MMIBAIDU_DRV_MAIN_C_

/**-------------------------------------------------------------------------*
**                         Dependencies                                     *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
// #ifdef __cplusplus
// extern   "C"
// {
// #endif
    
#ifdef WIN32
#include "std_header.h"
#endif
#include "os_api.h"
#include "mmk_type.h"
#include "window_parse.h"
#include "guilistbox.h"
#include "mmifmm_internal.h"
#include "mmifmm_export.h"
#include "mmibaidu_id.h"
#include "ctrllabel_export.h"
#include "mmidisplay_data.h"
#include "mmibaidu_export.h"
#include "mmi_appmsg.h"
#include "mmipicview_id.h"
#include "mmipub.h"
#include "watch_commonwin_export.h"
#include "mmiset_image.h"
#include "mmidc_export.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL MMIFILE_HANDLE	s_file_handle								= PNULL; //读取文件handle
LOCAL BOOLEAN			s_is_readfile_over							= FALSE; //文件是否读完
LOCAL BOOLEAN s_get_user_vip_query_confirm = FALSE;
LOCAL BOOLEAN s_is_first_enter_login = TRUE;
LOCAL BOOLEAN s_is_exit_from_red_key = FALSE;
LOCAL  FILEARRAY                 s_baidu_pre_capture_pic_array_lib         = PNULL;

extern MMIAPIBAIDU_USER_INFO_T s_baidu_user_info ;
extern uint8 s_baidu_drv_qr_code_url[BAIDU_DRV_MAX_STRING_LEN+1];
extern FILEARRAY s_baidu_drv_snapshot_upload_file_array;

#ifdef MAINLCD_PANEL_SIZE
#ifdef MAINLCD_PANEL_SIZE_240X240
#define MMI_BAIDU_LABLE_TOP_MARGIN_H 20
#define MMI_BAIDU_LABLE_TEXT_MARGIN_H 5
#define MMI_BAIDU_LOGIN_AVATAR_WIDTH 120
#elif defined (MAINLCD_PANEL_SIZE_360X360)
#define MMI_BAIDU_LABLE_TOP_MARGIN_H 30
#define MMI_BAIDU_LABLE_TEXT_MARGIN_H 15
#define MMI_BAIDU_LOGIN_AVATAR_WIDTH 220
#else
#define MMI_BAIDU_LABLE_TOP_MARGIN_H 5
#define MMI_BAIDU_LABLE_TEXT_MARGIN_H 5
#define MMI_BAIDU_LOGIN_AVATAR_WIDTH 80
#endif
#else
#ifdef MAINLCD_SIZE_128X160
#define MMI_BAIDU_LABLE_TOP_MARGIN_H 5
#define MMI_BAIDU_LABLE_TEXT_MARGIN_H 2
#define MMI_BAIDU_LOGIN_AVATAR_WIDTH 80
#elif defined (MAINLCD_SIZE_240X320) || defined(MAINLCD_SIZE_240X240)
#define MMI_BAIDU_LABLE_TOP_MARGIN_H 20
#define MMI_BAIDU_LABLE_TEXT_MARGIN_H 3
#define MMI_BAIDU_LOGIN_AVATAR_WIDTH 120
#elif defined (MAINLCD_SIZE_360X360)
#define MMI_BAIDU_LABLE_TOP_MARGIN_H 25
#define MMI_BAIDU_LABLE_TEXT_MARGIN_H 5
#define MMI_BAIDU_LOGIN_AVATAR_WIDTH 220
#endif
#endif

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/




/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description : to handle auto demo window message
//  Global resource dependence : 
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/                                           
LOCAL MMI_RESULT_E  HandleBaiduDrvWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );
/*****************************************************************************/
//  Description : to handle auto demo disk opt window message
//  Global resource dependence : s_dir
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBaiduDrvLoginWinMsg(
												MMI_WIN_ID_T    win_id, 
												MMI_MESSAGE_ID_E   msg_id, 
												DPARAM             param
                                             );
/*****************************************************************************/
//  Description : append list item
//  Global resource dependence : 
//  Author: chunjie.liu
//  Note: 
/*****************************************************************************/
LOCAL void AppendListItem(                                    
                          wchar                     *string_ptr,
                          uint16                    string_len,
                          MMI_CTRL_ID_T            ctrl_id,
                          MMI_TEXT_ID_T           mid_softkey_id
                          );
/********************************************************************************
//  Description : append list item
//  Global resource dependence : 
//  Author: chunjie.liu
//  Note:
********************************************************************************/
LOCAL void AppendDiskListItem(
                            MMI_CTRL_ID_T	ctrl_id,
                            MMI_TEXT_ID_T	text_id                          
                            );

/*****************************************************************************/
//  Description : Handle BAIDU Login Window
//  Parameter: [In] win_id
//             [In] msg_id
//             [In] param
//             [Out] None
//             [Return] MMI_RESULT_E
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBaiduVIPWinMsg(MMI_WIN_ID_T       win_id,
                                                MMI_MESSAGE_ID_E   msg_id,
                                                DPARAM             param);



/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                               *
**--------------------------------------------------------------------------*/


WINDOW_TABLE( MMIBAIDU_DRV_WIN_TAB ) = 
{
	WIN_FUNC( (uint32)HandleBaiduDrvWindow ),    
	WIN_ID( MMIBAIDU_DRV_WIN_ID ),
	WIN_HIDE_STATUS,
//	WIN_TITLE(TXT_NULL),    
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIBAIDU_LOGIN_LABEL1_CTRL_ID),
	CREATE_ANIM_CTRL(MMIBAIDU_LOGIN_ANIM_ID, MMIBAIDU_DRV_WIN_ID),
	
//	WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MMIBAIDU_DRV_MENU_WIN_TAB ) = 
{
	WIN_FUNC( (uint32)HandleBaiduDrvLoginWinMsg),
	WIN_ID(MMIBAIDU_DRV_USER_INFO_WIN_ID),
//	WIN_TITLE(TXT_NULL),
//	CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIBAIDU_DRV_OPT_LISTBOX_CTRL_ID),
//	WIN_SOFTKEY(STXT_SOFTKEY_OK_MK,TXT_NULL,STXT_RETURN),
	WIN_HIDE_STATUS,
	CREATE_ANIM_CTRL(MMIBAIDU_USERINFO_AVATAR_CTRL_ID, MMIBAIDU_DRV_USER_INFO_WIN_ID),	
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIBAIDU_USERINFOLABLE1_CTRL_ID),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIBAIDU_USERINFOLABLE2_CTRL_ID),
       CREATE_BUTTON_CTRL(image_watch_waitingwin_1line_tip_bg,  MMIBAIDU_LOGIN_LOGOUT_BTN_CTRL_ID),
	END_WIN
};


WINDOW_TABLE( MMISET_BAIDU_VIP_WIN_TAB ) =
{
	WIN_ID(MMIBAIDU_DRV_VIP_WIN_ID),
	WIN_HIDE_STATUS,
	WIN_FUNC((uint32)HandleBaiduVIPWinMsg),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIBAIDU_VIP_LABEL1_CTRL_ID),
	CREATE_ANIM_CTRL(MMIBAIDU_VIP_ANIM_ID, MMIBAIDU_DRV_VIP_WIN_ID),
    END_WIN
};



/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Handle Baidu Login Window
//  Parameter: [In] win_id
//             [In] msg_id
//             [In] param
//             [Out] None
//             [Return] MMI_RESULT_E
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBaiduVIPWinMsg(MMI_WIN_ID_T       win_id,
                                                MMI_MESSAGE_ID_E   msg_id,
                                                DPARAM             param)
{
    MMI_RESULT_E            result  = MMI_RESULT_TRUE;
    MMI_RESULT_E         recode			= MMI_RESULT_TRUE;
    uint16               index			= 0;
    GUI_LCD_DEV_INFO     lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_RECT_T      rect = {0};//{0,0,240,240};
    GUI_RECT_T      iconRect = {32, 62, 208, 238};
    MMI_CTRL_ID_T   ctrlIcon = MMIBAIDU_LOGIN_ANIM_ID;      //二维码图片
    GUI_RECT_T      labelRect1 =  {0, 178, 240, 203};
    MMI_CTRL_ID_T   ctrlLabel1 = MMIBAIDU_LOGIN_LABEL1_CTRL_ID;     //二维码图片下的第一行字
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_FILE_INFO_T        file_info = {0};
	
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {

			MMK_GetWinRect(win_id , &rect);
    		GUI_FillRect(&lcd_dev_info, rect, MMI_WHITE_COLOR);
    		GUIANIM_SetCtrlRect(ctrlIcon, &iconRect);

    		//CTRLLABEL_SetRect(ctrlLabel1, &labelRect1, FALSE);
    		//CTRLLABEL_SetTextById(MMIBAIDU_LOGIN_LABEL1_CTRL_ID, TXT_BAIDU_TITLE_INIT, FALSE); 	

    		control_info.is_ctrl_id = TRUE;
    		control_info.ctrl_id = MMIBAIDU_VIP_ANIM_ID;

    		display_info.is_update   = TRUE;
    		display_info.is_disp_one_frame = TRUE;
    		display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;

    		data_info.img_id = IMAGE_BAIDU_DRV_VIP_QRCODE; 

    		GUIANIM_SetParam(&control_info, &data_info,PNULL, &display_info);

            break;
        }
        case MSG_FULL_PAINT:
            break;
	 case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_APP_MENU:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        case MSG_CTL_CANCEL:
            // todo: 要不要处理一下
            break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527 ： button click
        case MSG_CTL_PENOK:
        {
            //invoke  the  function

            break;
        }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_APP_CANCEL:
        case MSG_CLOSE_WINDOW:
        {
            MMK_CloseWin(win_id);
            break;
        }
        default:
        {
            result = MMI_RESULT_FALSE;
            break;
        }
    }
    return result;
}


/*****************************************************************************/
//  Description : to handle auto demo window message
//  Global resource dependence : s_dir, s_filename_ptr,s_cmd_buf_ptr, s_cmd_buf_len, s_is_readfile_over
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBaiduDrvWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E         recode			= MMI_RESULT_TRUE;
    uint16               index = 0 , img_width = 0 , img_height = 0;
    GUI_LCD_DEV_INFO     lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_RECT_T      rect = {0};//{0,0,240,240};
    GUI_RECT_T      iconRect = {0};//{32, 62, 208, 238};
    MMI_CTRL_ID_T   ctrlIcon = MMIBAIDU_LOGIN_ANIM_ID;      //二维码图片
    GUI_RECT_T      labelRect1 =  {0};//{0, 178, 240, 203};
    MMI_CTRL_ID_T   ctrlLabel1 = MMIBAIDU_LOGIN_LABEL1_CTRL_ID;     //二维码图片下的第一行字
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_FILE_INFO_T        file_info = {0};


    switch(msg_id)  
    {
    case MSG_OPEN_WINDOW:
	{
              SCI_TRACE_LOW("[baidu]MSG_OPEN_WINDOW !!!" );
              MMISRV_BAIDUDRV_Need_Get_Token();
              //if(MMISRV_BAIDUDRV_Need_Get_Token())
              {
			MMK_GetWinRect(win_id , &rect);
			labelRect1 = rect;
			GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_BAIDU_DRV_LOGIN_ICON, win_id);
        		GUI_FillRect(&lcd_dev_info, rect, MMI_WHITE_COLOR);
			iconRect.top = (rect.bottom - rect.top + 1) / 2 - img_height/2 ; 
			iconRect.bottom = 	iconRect.top + img_height ; 			
			iconRect.left = (rect.right - rect.left + 1) / 2 - img_width/2 ; 
			iconRect.right = 	iconRect.left + img_width ; 			
			labelRect1.top = iconRect.top - WATCH_DEFAULT_NORMAL_FONT_SIZE - MMI_BAIDU_LABLE_TEXT_MARGIN_H ; 
			labelRect1.bottom = labelRect1.top + WATCH_DEFAULT_NORMAL_FONT_SIZE ; 
        		GUIANIM_SetCtrlRect(ctrlIcon, &iconRect);
        		CTRLLABEL_SetRect(ctrlLabel1, &labelRect1, FALSE);
        		CTRLLABEL_SetTextById(MMIBAIDU_LOGIN_LABEL1_CTRL_ID, TXT_BAIDU_TITLE_INIT, FALSE); 	

        		control_info.is_ctrl_id = TRUE;
        		control_info.ctrl_id = MMIBAIDU_LOGIN_ANIM_ID;

        		display_info.is_update   = TRUE;
        		display_info.is_disp_one_frame = TRUE;
        		display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;

        		data_info.img_id = IMAGE_BAIDU_DRV_LOGIN_ICON; 

        		GUIANIM_SetParam(&control_info, &data_info,PNULL, &display_info);
              }
		MMISRV_BAIDUDRV_CreateTask(MMI_MODULE_BAIDU_DRV , BAIDU_DRV_TYPE_LOGIN); 
		
           break;
    	}
    case MSG_BAIDU_DRV_INIT_CNF:
              if(!MMIAPIBAIDU_IsAccountExist())
              {
		    MMISRV_BAIDUDRV_Get_QR_Image();
               }
               else
                {
                    SCI_TRACE_LOW("[baidu]token is exist !!!" );
                    baidu_drv_get_user_info_req();
	             baidu_drv_get_disc_size_info_req();
                }
	break;
    case MSG_GET_FOCUS:
        
        break;
	case MSG_FULL_PAINT:
		{
			MMK_GetWinRect(win_id , &rect);						
        	GUI_FillRect(&lcd_dev_info, rect, MMI_WHITE_COLOR);
		}
		break;
    case MSG_BAIDU_DRV_SHOW_QR:
	{
		GUI_RECT_T win_rect = {0};
		GUI_LCD_DEV_INFO lcd_dev_info = {0};
		uint8 posx=5,posy=5 , i , j ;
		UILAYER_INFO_T layer_info = {0};
		uint16 *mem_buf_ptr = PNULL;
		uint16 *dst_ptr = PNULL;

		GUI_FillRect(&lcd_dev_info, rect, MMI_WHITE_COLOR);
		MMK_GetWinRect(MMIBAIDU_DRV_WIN_ID, &win_rect);			
		MMK_GetWinLcdDevInfo(MMIBAIDU_DRV_WIN_ID, &lcd_dev_info);    
		UILAYER_GetLayerInfo(&lcd_dev_info, &layer_info);     /*lint !e605*/

		if(MMIAPIFMM_IsFileExist(BAIDU_DRV_QRCODE_IMG_FILE_PATH, MMIAPICOM_Wstrlen(BAIDU_DRV_QRCODE_IMG_FILE_PATH)))
		{
			control_info.is_ctrl_id = TRUE;
			control_info.ctrl_id = MMIBAIDU_LOGIN_ANIM_ID;

			display_info.is_syn_decode = TRUE;
			display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
			display_info.is_zoom = TRUE;
			display_info.is_auto_zoom_in = TRUE;
			display_info.is_update = TRUE;


			file_info.full_path_wstr_ptr = BAIDU_DRV_QRCODE_IMG_FILE_PATH;
			file_info.full_path_wstr_len = MMIAPICOM_Wstrlen(BAIDU_DRV_QRCODE_IMG_FILE_PATH);
			GUIANIM_SetParam(&control_info, PNULL,&file_info, &display_info);

		}
		else
		{
			SCI_TRACE_LOW("[baidu]HandleBaiduDrvWindow QR Code image is not existed !!!" );
		}
	}		
        break;

    case MSG_BAIDU_DRV_GET_TOKEN_CNF:
	{
		uint16     frame_num = 0;

		MMK_GetWinRect(win_id , &rect);

		labelRect1 = rect;

		GUI_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

		labelRect1.top = (rect.bottom - rect.top + 1) / 2 - WATCH_DEFAULT_NORMAL_FONT_SIZE/2 ; 
		labelRect1.bottom = labelRect1.top + WATCH_DEFAULT_NORMAL_FONT_SIZE ; 
			
		CTRLLABEL_SetRect(ctrlLabel1, &labelRect1, FALSE);
		CTRLLABEL_SetTextById(MMIBAIDU_LOGIN_LABEL1_CTRL_ID, TXT_BAIDU_GET_USER_INFO, FALSE); 	
		GUILABEL_SetFont(MMIBAIDU_LOGIN_LABEL1_CTRL_ID, WATCH_DEFAULT_NORMAL_FONT_SIZE , 0xffff);

		CTRLANIM_SetVisible(MMIBAIDU_LOGIN_ANIM_ID , FALSE , FALSE);
		MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);			
	}

	break;
    case MSG_BAIDU_DRV_SIGNIN_DONE:
	{
		MMIFMM_FILTER_T         filter                  = {{"*.jpg"}};
		const wchar* dev_path = PNULL;
		int dev_len = MMIAPIFMM_GetDevicePathLen(MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD));	
		MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};	

		dev_path = MMIAPIFMM_GetDevicePath(MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD));
		if(MMIAPIFMM_GetDeviceStatus(dev_path, dev_len))
		{
			MMIPICVIEW_GetFixedFolderPhotoPath(dev_path, dev_len, PICVIEW_SOURCE_CLOUD_UPLOAD,&default_path);     
		}		
		//MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);

		s_baidu_pre_capture_pic_array_lib = MMIAPIFILEARRAY_Create();
		
#if 0
		MMK_CloseWin(win_id);
		//MMIAPIDC_OpenPhotoWin();
		MMK_CreateWin((uint32*)MMIBAIDU_DRV_MENU_WIN_TAB, PNULL);
#endif
		SCI_TRACE_LOW("[baidu]HandleBaiduDrvWindow MSG_BAIDU_DRV_SIGNIN_DONE , start search upload folder !!" );
		MMIAPIFMM_SearchFileInPathAndSort(
		    default_path.path,
		    default_path.path_len,
		    &filter,
		    FALSE,
		    FUNC_FIND_FILE,
		    s_baidu_pre_capture_pic_array_lib,
		    win_id,
		    MSG_BAIDU_DRV_CAPTURED_PIC_LOAD,
		    FILEARRAY_SORT_TIME_DESCEND);
	
	}
		break;
    case MSG_BAIDU_DRV_CAPTURED_PIC_LOAD:
	{
		BOOLEAN ret = FALSE;
		uint16 pic_num = MMIAPIFILEARRAY_GetArraySize(s_baidu_pre_capture_pic_array_lib) ; 
		uint16 i = 0 ;
		FILEARRAY_DATA_T          file_array_data = {0};
		MMK_CloseWin(win_id);
		MMK_CreateWin((uint32*)MMIBAIDU_DRV_MENU_WIN_TAB, PNULL);

		SCI_TRACE_LOW("[baidu]HandleBaiduDrvWindow MSG_BAIDU_DRV_CAPTURED_PIC_LOAD enter  !!" );

		if( 0  < pic_num)
		{			
			SCI_TRACE_LOW("[baidu]HandleBaiduDrvWindow has captured photo , save them to upload list  !!" );
			MMISRV_BAIDUDRV_Load_Upload_File_array();
			ret = MMIAPIFILEARRAY_Combine(s_baidu_drv_snapshot_upload_file_array,s_baidu_pre_capture_pic_array_lib);
			SCI_TRACE_LOW("[baidu]:HandleBaiduDrvWindow ret:%d ,size:%d", ret , MMIAPIFILEARRAY_GetArraySize(s_baidu_drv_snapshot_upload_file_array));			
		}
		
		MMIAPIFILEARRAY_Destroy(&s_baidu_pre_capture_pic_array_lib);		
	}
	break;
    case MSG_CTL_OK:
	case MSG_APP_MENU:
    case MSG_APP_OK:

        break;
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:                   
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

        break;  
		
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
	case MSG_KEYDOWN_RED:
	{
		MMIAPIFMM_SearchFileStop();
        MMK_CloseWin(win_id);
	}
        break;

     case MSG_CLOSE_WINDOW:
	 {
	 	if(!MMIAPIBAIDU_IsAccountExist())
			MMIAPIDC_OpenPhotoWin();

		s_is_first_enter_login = FALSE;
	 }
        break;  
		
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;  
}
/*****************************************************************************/
//  Description : to handle auto demo disk opt window message
//  Global resource dependence : s_dir
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBaiduDrvLoginWinMsg(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E         recode			= MMI_RESULT_TRUE;
    uint16               index = 0 , img_width = 0 , img_height = 0;
    GUI_LCD_DEV_INFO     lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T      rect =  {0};//{0,0,240,240};
    GUI_RECT_T      iconRect =  {0};//{60, 28, 180, 148};
    MMI_CTRL_ID_T   ctrlIcon = MMIBAIDU_USERINFO_AVATAR_CTRL_ID;      //二维码图片
    GUI_RECT_T      labelRect1 =   {0};//{0, 178, 240, 203};
    GUI_RECT_T      labelRect2 =   {0};//{0, 210, 240, 230};	
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_FILE_INFO_T file_path = {0};
    MMI_STRING_T	userinfo_str = {0};
    uint8 disk_info_str[MAX_BAIDU_USER_INFO_STR_SIZE] = {0};
	uint8 disk_info_str1[MAX_BAIDU_USER_INFO_STR_SIZE] = {0};
	uint8 disk_info_str2[MAX_BAIDU_USER_INFO_STR_SIZE] = {0};
    wchar disk_info_wstr[MAX_BAIDU_USER_INFO_STR_SIZE] = {0};
//    wchar *w_test_str = L"this is test ";	
    uint32      gb_size     =   1 << 30;
    uint32      tb_size     =   1 << 8;
	GUI_RECT_T  btnRect = {0};	

    switch(msg_id)  
    {
    case MSG_OPEN_WINDOW:
	{					
		MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
		
		MMK_GetWinRect(win_id , &rect);
		labelRect1 = labelRect2 = btnRect = rect;

		GUI_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

		iconRect.top = MMI_BAIDU_LABLE_TOP_MARGIN_H; 
		iconRect.bottom = 	iconRect.top + MMI_BAIDU_LOGIN_AVATAR_WIDTH -1 ; 			

		iconRect.left = (rect.right - rect.left + 1) / 2 - MMI_BAIDU_LOGIN_AVATAR_WIDTH/2 ; 
		iconRect.right = 	iconRect.left + MMI_BAIDU_LOGIN_AVATAR_WIDTH -1 ; 			

		labelRect1.top = iconRect.bottom + MMI_BAIDU_LABLE_TEXT_MARGIN_H ; 
		labelRect1.bottom = labelRect1.top + WATCH_DEFAULT_NORMAL_FONT_SIZE ; 

		labelRect2.top = labelRect1.bottom + MMI_BAIDU_LABLE_TEXT_MARGIN_H ; 
		labelRect2.bottom = labelRect2.top + WATCH_DEFAULT_NORMAL_FONT_SIZE ; 
		
		GUIRES_GetImgWidthHeight(&img_width, &img_height, image_watch_waitingwin_1line_tip_bg, win_id);

		btnRect.top = labelRect2.bottom + MMI_BAIDU_LABLE_TEXT_MARGIN_H ; 
		btnRect.bottom = btnRect.top + img_height ; 
		
		GUIANIM_SetCtrlRect(ctrlIcon, &iconRect);

		userinfo_str.wstr_len = MMIAPICOM_Wstrlen(s_baidu_user_info.baidu_name);
		userinfo_str.wstr_ptr = s_baidu_user_info.baidu_name;

	//	userinfo_str.wstr_len = MMIAPICOM_Wstrlen(w_test_str/*s_baidu_user_info.baidu_name*/);
	//	userinfo_str.wstr_ptr = w_test_str;/*s_baidu_user_info.baidu_name;*/

		CTRLLABEL_SetRect(MMIBAIDU_USERINFOLABLE1_CTRL_ID, &labelRect1, FALSE);
		CTRLLABEL_SetText(MMIBAIDU_USERINFOLABLE1_CTRL_ID, &userinfo_str, FALSE); 
		CTRLLABEL_SetFont(MMIBAIDU_USERINFOLABLE1_CTRL_ID , SONG_FONT_18 , MMI_WHITE_COLOR);

		GUIBUTTON_SetRect(MMIBAIDU_LOGIN_LOGOUT_BTN_CTRL_ID, &btnRect);
		CTRLBUTTON_SetTextId(MMIBAIDU_LOGIN_LOGOUT_BTN_CTRL_ID , TXT_BAIDU_LOGOUT);

		control_info.is_ctrl_id = TRUE;
		control_info.ctrl_id = MMIBAIDU_USERINFO_AVATAR_CTRL_ID;

		display_info.is_update   = FALSE;
		display_info.is_disp_one_frame = TRUE;
		display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;

		if(strlen(s_baidu_user_info.avatar_url) &&
		 (MMIAPIFMM_IsFileExist(BAIDU_DRV_AVATAR_IMG_FILE_PATH, MMIAPICOM_Wstrlen(BAIDU_DRV_AVATAR_IMG_FILE_PATH)))
		)
		{
			file_path.full_path_wstr_ptr = BAIDU_DRV_AVATAR_IMG_FILE_PATH;
			file_path.full_path_wstr_len = MMIAPICOM_Wstrlen(BAIDU_DRV_AVATAR_IMG_FILE_PATH);
			GUIANIM_SetParam(&control_info, PNULL,&file_path, &display_info);
		}
		else
		{
			data_info.img_id = IMAGE_BAIDU_DRV_LOGIN_ICON; 
			GUIANIM_SetParam(&control_info, &data_info,PNULL, &display_info);
		}

//		s_baidu_user_info.size_used = 14790752306 ; 
//		s_baidu_user_info.size_total = 123480309760 ; 
		
		sprintf(disk_info_str,"%d", s_baidu_user_info.size_used / gb_size );
		sprintf(disk_info_str1,"%d", s_baidu_user_info.size_total / gb_size);
		
		sprintf(disk_info_str2,"%s G / %s G", disk_info_str , disk_info_str1);

		MMI_STRNTOWSTR(disk_info_wstr , MAX_BAIDU_USER_INFO_STR_SIZE , 
						    disk_info_str2 , MAX_BAIDU_USER_INFO_STR_SIZE , 
						    strlen(disk_info_str2));


		userinfo_str.wstr_len = MMIAPICOM_Wstrlen(disk_info_wstr);
		userinfo_str.wstr_ptr = disk_info_wstr;

		CTRLLABEL_SetRect(MMIBAIDU_USERINFOLABLE2_CTRL_ID, &labelRect2, FALSE);
		CTRLLABEL_SetText(MMIBAIDU_USERINFOLABLE2_CTRL_ID, &userinfo_str, FALSE); 	
		CTRLLABEL_SetFont(MMIBAIDU_USERINFOLABLE2_CTRL_ID , SONG_FONT_16 , MMI_WHITE_COLOR);

		s_is_exit_from_red_key = FALSE;
		
           break;
    	}

	case MSG_GET_FOCUS:
	case MSG_FULL_PAINT:
	{
		MMK_GetWinRect(win_id , &rect);
		GUI_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);        
	}
        break;
    case MSG_CTL_OK:
    case MSG_APP_MENU:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:                   
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527	
	{
            if((PNULL != param) && (((MMI_NOTIFY_T*) param)->src_id == MMIBAIDU_LOGIN_LOGOUT_BTN_CTRL_ID))
            {
			MMISRV_BAIDUDRV_OpenBaiduLogoutQueryWin();
            }
	}
		break;		
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

     case MSG_KEYDOWN_RED :
	 {
		s_is_exit_from_red_key = TRUE;
		recode = MMI_RESULT_FALSE;		
     	 }
	 break;
     case MSG_CLOSE_WINDOW:
	 {
	 	if(!s_is_exit_from_red_key)
			MMIAPIDC_OpenPhotoWin();	 	
     	 }
        break;  
		
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;  
}


PUBLIC MMI_RESULT_E MMIAPIBAIDU_DRV_HandleSignal(MMI_MESSAGE_ID_E  msg_id,   DPARAM  param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
        //if initialize finish-->MMIPICVIEW_LIST_WIN_TAB
	SCI_TRACE_LOW("[baidu]MMIAPIBAIDU_DRV_HandleSignal receive %x" , msg_id);

	switch (msg_id)
	{
	case MSG_BAIDU_DRV_DOWNLOAD_PHOTO_NOTIFY :
       case MSG_BAIDU_DRV_DOWNLOAD_PHOTO_CNF:
            if(MMK_IsOpenWin(MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID))
	    {
                SCI_TRACE_LOW("MMIAPIBAIDU_DRV_HandleSignal send msg to MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID");
		MMK_SendMsg(MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID, msg_id, param);
	    }
        break;
	case MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_NOTIFY  :
	case MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_CNF:
	{
            if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID)){
                MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID, msg_id, param);
            }
	}
        break;

       case MSG_BAIDU_DRV_INIT_CNF:
	case MSG_BAIDU_DRV_SHOW_QR:
	case MSG_BAIDU_DRV_GET_TOKEN_CNF:
	case MSG_BAIDU_DRV_SIGNIN_DONE:
    {
        BAIDU_DRV_SIG_T * data =(BAIDU_DRV_SIG_T *)param;
        BAIDU_DRV_TYPE_E fromtype = BAIDU_DRV_TYPE_NONE;
        if(param!=PNULL){
            fromtype = data->baidu_drv_msg_data.init_type; 
        }
	 SCI_TRACE_LOW("[baidu]MMIAPIBAIDU_DRV_HandleSignal :%d, %d", MMK_IsOpenWin(MMIBAIDU_DRV_WIN_ID),  fromtype);
        if(MMK_IsOpenWin(MMIBAIDU_DRV_WIN_ID)&& fromtype==BAIDU_DRV_TYPE_LOGIN)
        {
			SCI_TRACE_LOW("[baidu]MMIAPIBAIDU_DRV_HandleSignal send msg to MMIBAIDU_DRV_WIN_ID msg_id 0x%x =" , msg_id);
			MMK_SendMsg(MMIBAIDU_DRV_WIN_ID, msg_id, param);
        }
        else if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID)&& fromtype==BAIDU_DRV_TYPE_MANUAL_UPLOAD_MULTI_PHOTO)
        {
			SCI_TRACE_LOW("MMIAPIBAIDU_DRV_HandleSignal send msg to MMIPICVIEW_LIST_WIN_ID");
			MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID, msg_id, param);
        }
		else if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID))
		{
			MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID, msg_id, param);
			SCI_TRACE_LOW("MMIAPIBAIDU_DRV_HandleSignal send msg to MMIPICVIEW_LIST_WIN_ID");
		}
		else{
			SCI_TRACE_LOW("[baidu]MMIAPIBAIDU_DRV_HandleSignal error ");
        }
	}
        break;
		
        case MSG_BAIDU_DRV_ERROR_NOTIFY:
	if(MMK_IsOpenWin(MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID))
	{
                SCI_TRACE_LOW("MMIAPIBAIDU_DRV_HandleSignal send msg to MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID");
		MMK_SendMsg(MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID, msg_id, param);
	}
        else if (MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID))
        {
        	SCI_TRACE_LOW("MMIAPIBAIDU_DRV_HandleSignal send msg to MMIPICVIEW_PREVIEW_WIN_ID");
        	MMK_SendMsg(MMIPICVIEW_PREVIEW_WIN_ID, msg_id, param);
        }
        else if (MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID))
	{
		MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID, msg_id, param);
		SCI_TRACE_LOW("MMIAPIBAIDU_DRV_HandleSignal send msg to MMIPICVIEW_LIST_WIN_ID");
        }
	else
	{
            SCI_TRACE_LOW("MMIAPIBAIDU_DRV_HandleSignal send msg to NONE!!! ");
	}
            break;				
	default:
		break;
	}
    return result;
}


/*****************************************************************************/
//  Description : apend list item
//  Global resource dependence : 
//  Author: chunjie.liu
//  Note: 
/*****************************************************************************/
LOCAL void AppendListItem(                                    
                          wchar                     *string_ptr,
                          uint16                    string_len,
                          MMI_CTRL_ID_T            ctrl_id,
                          MMI_TEXT_ID_T           mid_softkey_id
                          )
{
    GUILIST_ITEM_T      item_t                = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data             = {0};/*lint !e64*/
    
    //item_data.softkey_id[0] = TXT_OPTION;
    item_data.softkey_id[1] = mid_softkey_id;
    item_data.softkey_id[2] = STXT_RETURN;    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = string_len;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = string_ptr;
    GUILIST_AppendItem(ctrl_id, &item_t);      
}

/********************************************************************************
//  Description : apend list item
//  Global resource dependence : 
//  Author: chunjie.liu
//  Note:
********************************************************************************/
LOCAL void AppendDiskListItem(
                            MMI_CTRL_ID_T	ctrl_id,
                            MMI_TEXT_ID_T	text_id                          
                            )
{
    GUILIST_ITEM_T      item_t    = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id= text_id;
    GUILIST_AppendItem(ctrl_id, &item_t); 
    
    /*
    item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_ENG_OK;
    item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
    item_data.softkey_id[2] = (MMI_TEXT_ID_T)TXT_ENG_RETURN;    */
    
}
/*****************************************************************************/
//  Description : read file to cmd
//  Global resource dependence : s_cmd_buf_ptr, s_cmd_buf_len, s_is_readfile_over,s_file_cur_pos
//  Author: chunjie.liu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBAIDU_DRV_LoadCmd(
                                    uint8* cmd_buf,
									uint32 *cmd_buf_len_ptr
                                 )
{
    uint32			bytes_to_read								= 1000; //将要从脚本文件中读取到cmd的数据大小
    MMIFILE_ERROR_E file_read_error								= SFS_NO_ERROR;  //脚本文件是否读取完成
    BOOLEAN         is_load										= FALSE;  //本次读取是否成功
    uint32			data_str_index								= 0;   //读入数据有行数据时往前查找换行的下标
    uint32			bytes_read									= 0;   //本次读取到的数据大小
    uint32			cmd_buf_len								    = 1000;  //从脚本文件中读入数据的大小
    LOCAL uint32	s_file_cur_pos								= 0; //读取文件到cmd时不足一行时回退的数据大小
    LOCAL uint8		s_cmd_buf_ptr[1000]    ={0}; //读取文件到cmd时的buffer
    //把上次读取数据时不足整行的数据拷贝到本次读取数据的buffer中    
    //SCI_PASSERT(PNULL != cmd_buf && PNULL != cmd_buf_len_ptr, ("MMIAPIAUTODEMO_LoadCmd, PNULL != cmd_buf && PNULL != cmd_buf_len_ptr"));
    if(PNULL == cmd_buf)
    {
        return FALSE;
    }
    SCI_TRACE_LOW("MMIAPIBAIDU_DRV_LoadCmd, cmd_buf = %x, cmd_buf_len_ptr = %d", cmd_buf, *cmd_buf_len_ptr);

    if (0 != s_file_cur_pos)
    {
        MMIAPICOM_CopyString(
                                                    (uint8*)&s_file_cur_pos,       //OUT:
            cmd_buf,     //OUT:
            s_file_cur_pos,             //IN:
            s_file_cur_pos + 1,            //IN:
            s_cmd_buf_ptr       //IN:
            );
    }
    
    bytes_to_read -= s_file_cur_pos;
    //读取脚本数据到buffer
    file_read_error = MMIAPIFMM_ReadFile(
                                        s_file_handle,
                                        cmd_buf + s_file_cur_pos,
                                        bytes_to_read,
                                        &bytes_read,
                                        PNULL
                                        );
    
    if (SFS_NO_ERROR == file_read_error)
    {
		//if分支是对读完脚本文件的处理
        if (bytes_read < bytes_to_read)
        {
            s_is_readfile_over = TRUE;
            SCI_MEMSET(cmd_buf+ s_file_cur_pos+bytes_read, 0, (bytes_to_read-bytes_read))
            cmd_buf_len = bytes_read + s_file_cur_pos;

                
        }
        //此分支是对未读完脚本文件的处理
        else
        {
            data_str_index = bytes_to_read + s_file_cur_pos - 1;
            //此循环追踪不足整行数据的最近的换行符
            while (cmd_buf[data_str_index] != '\n')
            {
                data_str_index--;
            }
            //去掉不足整行数据后的相关变量的设置
            bytes_read =  1 + data_str_index;
            cmd_buf_len = 1 + data_str_index;		
            if (bytes_read != bytes_to_read + s_file_cur_pos)
            {
                s_file_cur_pos += (bytes_to_read - bytes_read);
            }
			//把不足整行的数据拷贝到内存中，保留下次读取时使用
            MMIAPICOM_CopyString(
                                                            (uint8*)&s_file_cur_pos,       //OUT:
                                s_cmd_buf_ptr,     //OUT:
                                s_file_cur_pos,             //IN:
                                s_file_cur_pos + 1,            //IN:
                                cmd_buf + cmd_buf_len
                                );
        }

        is_load = TRUE;
        *cmd_buf_len_ptr = cmd_buf_len;
        if(s_is_readfile_over)
        {
            s_file_cur_pos = 0;
        }
        SCI_TRACE_LOW("MMIAPIBAIDU_DRV_LoadCmd, SFS_NO_ERROR == file_read_error cmd_buf = %x, cmd_buf_len_ptr = %d", cmd_buf, *cmd_buf_len_ptr);
    }
    
    SCI_TRACE_LOW("MMIAPIBAIDU_DRV_LoadCmd, file_read_error = %d, data_str_index = %d", file_read_error, data_str_index);

    return is_load;
}

/*****************************************************************************/
//  Description : to open autodemo main menu window
//  Global resource dependence : none
//  Author: chunjie.liu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBAIDU_DRV_OpenQRImgWindow(void)
{

     if(s_is_first_enter_login)
         return MMK_CreateWin((uint32*)MMIBAIDU_DRV_WIN_TAB, PNULL);
//         return MMK_CreateWin((uint32*)MMIBAIDU_DRV_MENU_WIN_TAB, PNULL);
     else
	  return MMIAPIDC_OpenPhotoWin();
}

/*****************************************************************************/
//  Description : to open vip window
//  Global resource dependence : none
//  Author: chunjie.liu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBAIDU_DRV_OpenVIPWindow(void)
{
     return MMK_CreateWin((uint32*)MMISET_BAIDU_VIP_WIN_TAB, PNULL);
}


/*****************************************************************************/
//  Description : to open DiskOpt menu window
//  Global resource dependence : none
//  Author: chunjie.liu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBAIDU_DRV_OpenDiskOptWindow(void)
{
	return MMK_CreateWin((uint32*)MMIBAIDU_DRV_MENU_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : is read  the file over
//  Global resource dependence : s_is_readfile_over
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/     
PUBLIC BOOLEAN MMIAPIBAIDU_DRV_IsFileOver(void)
{
    return s_is_readfile_over;
}    

/*****************************************************************************/
//  Description : get file handle
//  Global resource dependence : s_file_handle
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/  
PUBLIC MMIFILE_HANDLE MMIAPIBAIDU_DRV_GetFileHandle(void)
{
    return s_file_handle;
}    

/*****************************************************************************/
//  Description : set file handle
//  Global resource dependence : s_file_handle
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/  
PUBLIC void MMIAPIBAIDU_DRV_SetFileHandle(MMIFILE_HANDLE file_handle)
{
    s_file_handle = file_handle;
}       


/*****************************************************************************/
//  Description : to handle  baidu vip account query window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBaiduVIPAccountQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMIPUB_INFO_T   *win_info_ptr = PNULL;
	
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:
	MMIAPIBAIDU_DRV_OpenVIPWindow();
	s_get_user_vip_query_confirm = TRUE;
	MMIPUB_CloseQuerytWin(&win_id);

	break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
	s_get_user_vip_query_confirm = TRUE;		
	MMIPUB_CloseQuerytWin(&win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : to handle  baidu login query window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBaiduLoginQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMIPUB_INFO_T   *win_info_ptr = PNULL;
	
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:
	s_is_first_enter_login = TRUE;
	MMIAPIBAIDU_DRV_OpenQRImgWindow();
	MMIPUB_CloseQuerytWin(&win_id);

	break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
	MMIPUB_CloseQuerytWin(&win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

LOCAL MMI_RESULT_E HandleBaiduLogoutDoneMsg(
    MMI_WIN_ID_T      winId,
    MMI_MESSAGE_ID_E    msgId,
    DPARAM                param
)
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;
    switch(msgId)
    {
        case MSG_CLOSE_WINDOW:
            MMK_CloseWin(MMIBAIDU_DRV_USER_INFO_WIN_ID);
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}


/*****************************************************************************/
//  Description : to handle  baidu logout query window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBaiduLogoutQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMIPUB_INFO_T   *win_info_ptr = PNULL;
    MMI_TEXT_ID_T 	text_id = TXT_NULL;
    MMI_WIN_ID_T	alert_win_id = MMIBAIDU_DRV_ALERT_WIN_ID;
    MMI_STRING_T  txt_error = {0};
    WATCH_SOFTKEY_TEXT_ID_T softkey = {TXT_NULL,TXT_NULL,TXT_NULL};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
	{
		s_is_exit_from_red_key = FALSE;
    	}
		break;
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:
	{		
		s_is_first_enter_login = TRUE;
		MMIPUB_CloseQuerytWin(&win_id);		
		if(MMIAPIDC_IsOpened())
		{
			MMIAPIDC_Exit();
		}
		recode = MMISRV_BAIDUDRV_Logout();
		if(MMI_RESULT_TRUE == recode)
		{
		        text_id = TXT_COMPLETE;
		}
		else
		{
		        text_id = TXT_ERROR;	
		}

		MMI_GetLabelTextByLang( text_id, &txt_error );
		WatchCOM_NoteWin_1Line_Enter(alert_win_id,&txt_error,0,softkey,HandleBaiduLogoutDoneMsg);						
    	}
	break;        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
	MMIPUB_CloseQuerytWin(&win_id);
	break;
     case MSG_KEYDOWN_RED :
	 {
		s_is_exit_from_red_key = TRUE;
		recode = MMI_RESULT_FALSE;		
     	 }
	 break;	
    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : MMIAPIBaidu_CheckVIPAccount
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BAIDU_DRIVE_VIP_STATUS MMISRV_BAIDUDRV_CheckVIPAccount(void)
{
	MMIAPIBAIDU_USER_INFO_T *p_user_info	= MMISRV_BAIDUDRV_get_user_info();

	if(p_user_info->vip_type >=1)
	{
		return BAIDU_DRV_VIP_DONE;
	}
	else  if (0 == p_user_info->vip_type && s_get_user_vip_query_confirm)
	{
		return BAIDU_DRV_VIP_DENY;
	}
	else
	{
		
		return BAIDU_DRV_VIP_NONE;
	}
}

/*****************************************************************************/
//  Description : MMISRV_BAIDUDRV_OpenVIPQueryWin
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_OpenVIPQueryWin(void)
{		 
	MMI_STRING_T            queryTipStr = { 0 };
	WATCH_SOFTKEY_TEXT_ID_T softkey = { STXT_RETURN, PNULL, STXT_OK };
	MMI_WIN_ID_T win_id = MMIBAIDU_DRV_WAITING_WIN_ID;



    MMI_GetLabelTextByLang( TXT_BAIDU_NOT_VIP_USER, &queryTipStr );
    WatchCOM_QueryWin_2Btn_Enter(
                                    MMIBAIDU_DRV_WAITING_WIN_ID,
                                    &queryTipStr,
                                    PNULL,
                                    PNULL,
                                    IMAGE_SET_COM_BTN_FIXED,
                                    IMAGE_SET_COM_BTN_CANCEL,
                                    softkey,
                                    HandleBaiduVIPAccountQueryWinMsg
                                );
}


PUBLIC void MMISRV_BAIDUDRV_OpenBaiduLoginQueryWin(void)
{		 
	MMI_STRING_T            queryTipStr = { 0 };
	WATCH_SOFTKEY_TEXT_ID_T softkey = { STXT_RETURN, PNULL, STXT_OK };
	MMI_WIN_ID_T win_id = MMIBAIDU_DRV_WAITING_WIN_ID;



	MMI_GetLabelTextByLang( TXT_BAIDU_DISK_FULL_LOGIN_BAIDU, &queryTipStr );
	WatchCOM_QueryWin_2Btn_Enter(
	                                MMIBAIDU_DRV_WAITING_WIN_ID,
	                                &queryTipStr,
	                                PNULL,
	                                PNULL,
	                                IMAGE_SET_COM_BTN_FIXED,
	                                IMAGE_SET_COM_BTN_CANCEL,
	                                softkey,
	                                HandleBaiduLoginQueryWinMsg
	                            );
}


/*****************************************************************************/
//  Description : MMISRV_BAIDUDRV_OpenBaiduLogOutQueryWin
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMISRV_BAIDUDRV_OpenBaiduLogoutQueryWin(void)
{		 
	MMI_STRING_T            queryTipStr = { 0 };
	WATCH_SOFTKEY_TEXT_ID_T softkey = { STXT_RETURN, PNULL, STXT_OK };
	MMI_WIN_ID_T win_id = MMIBAIDU_DRV_WAITING_WIN_ID;



	MMI_GetLabelTextByLang( TXT_BAIDU_LOGOUT_QUERY, &queryTipStr );
	WatchCOM_QueryWin_2Btn_Enter(
	                                MMIBAIDU_DRV_WAITING_WIN_ID,
	                                &queryTipStr,
	                                PNULL,
	                                PNULL,
	                                IMAGE_SET_COM_BTN_FIXED,
	                                IMAGE_SET_COM_BTN_CANCEL,
	                                softkey,
	                                HandleBaiduLogoutQueryWinMsg
	                            );
}

